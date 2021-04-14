#include "basewindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>
#include <QDebug>
#include "qlayout.h"

#define STRETCH_RECT_HEIGHT 4		// 拉伸小矩形的高度;
#define STRETCH_RECT_WIDTH 4		// 拉伸小矩形的宽度;

BaseWindow::BaseWindow(QWidget *parent)
    : QWidget(parent)
    , m_titleBar(NULL)
    , m_isSupportStretch(false)
    , m_isWindowMax(false)
    , m_stretchRectState(NO_SELECT)
    , m_windowMinWidth(150)
    , m_windowMinHeight(70)
{
    // FramelessWindowHint属性设置窗口去除边框;
    // WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    // 设置窗口背景透明;
    //setAttribute(Qt::WA_TranslucentBackground);
    // 初始化标题栏;
    initTitleBar();
    initStretchRect();

}

BaseWindow::~BaseWindow()
{

}

void BaseWindow::initTitleBar()
{
    m_titleBar = new MyTitleBar(this);
    m_titleBar->move(0, 0);

    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
    connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

}

void BaseWindow::initStretchRect()
{
    m_stretchRectState = NO_SELECT;
    m_leftTopRect     = QRect(0, 0, 0, 0);
    m_leftBottomRect  = QRect(0, 0, 0, 0);
    m_rightTopRect    = QRect(0, 0, 0, 0);
    m_rightBottomRect = QRect(0, 0, 0, 0);

    m_topBorderRect   = QRect(0, 0, 0, 0);
    m_rightBorderRect = QRect(0, 0, 0, 0);
    m_bottomBorderRect = QRect(0, 0, 0, 0);
    m_leftBorderRect = QRect(0, 0, 0, 0);
}

// 设置当前窗口是否支持拉伸;
// 此方法需要在调用完initTitleBar方法之后调用，因为m_titleBar在initTitleBar方法中创建;
void BaseWindow::setSupportStretch(bool isSupportStretch)
{
    m_isSupportStretch = isSupportStretch;
    // 这里加了非空判断，防止m_titleBar未创建;
    if (m_titleBar != NULL)
    {
        m_titleBar->setSupportStretch(isSupportStretch);
    }

    this->setMouseTracking(isSupportStretch);
    QList<QWidget*> widgetList = this->findChildren<QWidget*>();
    foreach (QWidget* widget, widgetList)
    {
        widget->setMouseTracking(isSupportStretch);
    }
}

// 获取当前窗口是否支持拉伸;
bool BaseWindow::getSupportStretch()
{
    return m_isSupportStretch;
}

void BaseWindow::paintEvent(QPaintEvent* event)
{
//    Q_UNUSED(event);
//    QStyleOption opt;
//    opt.init(this);
//    QPainter painter(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    //设置背景色;
//    QPainter painter(this);
//    QPainterPath pathBack;
//    pathBack.setFillRule(Qt::WindingFill);
//    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
//    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
//    painter.fillPath(pathBack, QBrush(QColor(238, 223, 204)));

//    return QWidget::paintEvent(event);
}

void BaseWindow::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/Resources/" + sheetName + ".css");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void BaseWindow::onButtonMinClicked()
{
    if (Qt::Tool == (windowFlags() & Qt::Tool))
    {
        hide();    //设置了Qt::Tool 如果调用showMinimized()则窗口就销毁了？？？
    }
    else
    {
        showMinimized();
    }
}

void BaseWindow::onButtonRestoreClicked()
{
    m_isWindowMax = false;
    QPoint windowPos;
    QSize windowSize;
    m_titleBar->getRestoreInfo(windowPos, windowSize);
    this->setGeometry(QRect(windowPos, windowSize));
}

void BaseWindow::onButtonMaxClicked()
{
    m_isWindowMax = true;
    m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
    QRect desktopRect = QApplication::desktop()->availableGeometry();
    QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
    setGeometry(FactRect);
}

void BaseWindow::onButtonCloseClicked()
{
    close();
}

// 计算拉伸区域Rect位置;
void BaseWindow::calculateCurrentStrechRect()
{
    m_leftTopRect = QRect(0, 0, STRETCH_RECT_WIDTH, STRETCH_RECT_HEIGHT);
    m_leftBottomRect = QRect(0, this->height() - STRETCH_RECT_HEIGHT, STRETCH_RECT_WIDTH, STRETCH_RECT_WIDTH);
    m_rightTopRect = QRect(this->width() - STRETCH_RECT_WIDTH, 0, STRETCH_RECT_WIDTH, STRETCH_RECT_HEIGHT);
    m_rightBottomRect = QRect(this->width() - STRETCH_RECT_WIDTH, this->height() - STRETCH_RECT_HEIGHT, STRETCH_RECT_WIDTH, STRETCH_RECT_HEIGHT);

    m_topBorderRect = QRect(STRETCH_RECT_WIDTH, 0, this->width() - STRETCH_RECT_WIDTH * 2, STRETCH_RECT_HEIGHT);
    m_rightBorderRect = QRect(this->width() - STRETCH_RECT_WIDTH, STRETCH_RECT_HEIGHT, STRETCH_RECT_WIDTH, this->height() - STRETCH_RECT_HEIGHT * 2);
    m_bottomBorderRect = QRect(STRETCH_RECT_WIDTH, this->height() - STRETCH_RECT_HEIGHT, this->width() - STRETCH_RECT_WIDTH * 2, STRETCH_RECT_HEIGHT);
    m_leftBorderRect = QRect(0, STRETCH_RECT_HEIGHT, STRETCH_RECT_WIDTH, this->height() - STRETCH_RECT_HEIGHT * 2);
}

// 在窗口第一次show完之后需要计算拉伸区域Rect位置;
void BaseWindow::showEvent(QShowEvent *event)
{
    calculateCurrentStrechRect();
    return QWidget::showEvent(event);
}

// 根据当前鼠标位置，定位鼠标在具体哪一块拉伸区域;
WindowStretchRectState BaseWindow::getCurrentStretchState(QPoint cursorPos)
{
    WindowStretchRectState stretchState;
    if (m_leftTopRect.contains(cursorPos))
    {
        stretchState = LEFT_TOP_RECT;
    }
    else if (m_rightTopRect.contains(cursorPos))
    {
        stretchState = RIGHT_TOP_RECT;
    }
    else if (m_rightBottomRect.contains(cursorPos))
    {
        stretchState = RIGHT_BOTTOM_RECT;
    }
    else if (m_leftBottomRect.contains(cursorPos))
    {
        stretchState = LEFT_BOTTOM_RECT;
    }
    else if (m_topBorderRect.contains(cursorPos))
    {
        stretchState = TOP_BORDER;
    }
    else if (m_rightBorderRect.contains(cursorPos))
    {
        stretchState = RIGHT_BORDER;
    }
    else if (m_bottomBorderRect.contains(cursorPos))
    {
        stretchState = BOTTOM_BORDER;
    }
    else if (m_leftBorderRect.contains(cursorPos))
    {
        stretchState = LEFT_BORDER;
    }
    else
    {
        stretchState = NO_SELECT;
    }

    return stretchState;
}

// 根据getCurrentStretchState返回状态进行更新鼠标样式;
void BaseWindow::updateMouseStyle(WindowStretchRectState stretchState)
{
    switch (stretchState)
    {
    case NO_SELECT:
        setCursor(Qt::ArrowCursor);
        break;
    case LEFT_TOP_RECT:
    case RIGHT_BOTTOM_RECT:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TOP_BORDER:
    case BOTTOM_BORDER:
        setCursor(Qt::SizeVerCursor);
        break;
    case RIGHT_TOP_RECT:
    case LEFT_BOTTOM_RECT:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case LEFT_BORDER:
    case RIGHT_BORDER:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }

}

void BaseWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 如果窗口最大化是不能拉伸的;
    // 也不用更新鼠标样式;
    if (m_isWindowMax)
    {
        return QWidget::mouseMoveEvent(event);
    }

    // 如果当前鼠标未按下，则根据当前鼠标的位置更新鼠标的状态及样式;
    if (!m_isMousePressed)
    {
        QPoint cursorPos = event->pos();
        // 根据当前鼠标的位置显示不同的样式;
        m_stretchRectState = getCurrentStretchState(cursorPos);
        updateMouseStyle(m_stretchRectState);
    }
    else
    {
        m_endPoint = this->mapToGlobal(event->pos());
        updateWindowSize();
    }

    return QWidget::mouseMoveEvent(event);
}

void BaseWindow::mousePressEvent(QMouseEvent *event)
{
    // 当前鼠标进入了以上指定的8个区域，并且是左键按下时才开始进行窗口拉伸;
    if (m_stretchRectState != NO_SELECT && event->button() == Qt::LeftButton)
    {
        m_isMousePressed = true;
        // 记录下当前鼠标位置，为后面计算拉伸位置;
        m_startPoint = this->mapToGlobal(event->pos());
        // 保存下拉伸前的窗口位置及大小;
        m_windowRectBeforeStretch = this->geometry();
    }

    return QWidget::mousePressEvent(event);
}

void BaseWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_isMousePressed = false;
    calculateCurrentStrechRect();

    return QWidget::mouseReleaseEvent(event);
}

// 拉伸窗口过程中，根据记录的坐标更新窗口大小;
void BaseWindow::updateWindowSize()
{
    // 拉伸时要注意设置窗口最小值;
    QRect windowRect = m_windowRectBeforeStretch;
    int delValue_X = m_startPoint.x() - m_endPoint.x();
    int delValue_Y = m_startPoint.y() - m_endPoint.y();
    if (m_stretchRectState == LEFT_BORDER)
    {
        QPoint topLeftPoint = windowRect.topLeft();
        topLeftPoint.setX(topLeftPoint.x() - delValue_X);
        windowRect.setTopLeft(topLeftPoint);
    }
    else if (m_stretchRectState == RIGHT_BORDER)
    {
        QPoint bottomRightPoint = windowRect.bottomRight();
        bottomRightPoint.setX(bottomRightPoint.x() - delValue_X);
        windowRect.setBottomRight(bottomRightPoint);
    }
    else if (m_stretchRectState == TOP_BORDER)
    {
        QPoint topLeftPoint = windowRect.topLeft();
        topLeftPoint.setY(topLeftPoint.y() - delValue_Y);
        windowRect.setTopLeft(topLeftPoint);
    }
    else if (m_stretchRectState == BOTTOM_BORDER)
    {
        QPoint bottomRightPoint = windowRect.bottomRight();
        bottomRightPoint.setY(bottomRightPoint.y() - delValue_Y);
        windowRect.setBottomRight(bottomRightPoint);
    }
    else if (m_stretchRectState == LEFT_TOP_RECT)
    {
        QPoint topLeftPoint = windowRect.topLeft();
        topLeftPoint.setX(topLeftPoint.x() - delValue_X);
        topLeftPoint.setY(topLeftPoint.y() - delValue_Y);
        windowRect.setTopLeft(topLeftPoint);
    }
    else if (m_stretchRectState == RIGHT_TOP_RECT)
    {
        QPoint topRightPoint = windowRect.topRight();
        topRightPoint.setX(topRightPoint.x() - delValue_X);
        topRightPoint.setY(topRightPoint.y() - delValue_Y);
        windowRect.setTopRight(topRightPoint);
    }
    else if (m_stretchRectState == RIGHT_BOTTOM_RECT)
    {
        QPoint bottomRightPoint = windowRect.bottomRight();
        bottomRightPoint.setX(bottomRightPoint.x() - delValue_X);
        bottomRightPoint.setY(bottomRightPoint.y() - delValue_Y);
        windowRect.setBottomRight(bottomRightPoint);
    }
    else if (m_stretchRectState == LEFT_BOTTOM_RECT)
    {
        QPoint bottomLeftPoint = windowRect.bottomLeft();
        bottomLeftPoint.setX(bottomLeftPoint.x() - delValue_X);
        bottomLeftPoint.setY(bottomLeftPoint.y() - delValue_Y);
        windowRect.setBottomLeft(bottomLeftPoint);
    }

    // 避免宽或高为零窗口显示有误，这里给窗口设置最小拉伸高度、宽度;
    if (windowRect.width() < m_windowMinWidth)
    {
        windowRect.setLeft(this->geometry().left());
        windowRect.setWidth(m_windowMinWidth);
    }
    if (windowRect.height() < m_windowMinHeight)
    {
        windowRect.setTop(this->geometry().top());
        windowRect.setHeight(m_windowMinHeight);
    }

    this->setGeometry(windowRect);
}
