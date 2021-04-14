#ifndef BASEWINDOW_H
#define BASEWINDOW_H
#include <QWidget>
#include <QMainWindow>
#include "mytitlebar.h"

// 当前鼠标所处状态;
enum WindowStretchRectState
{
    NO_SELECT = 0,				// 鼠标未进入下方矩形区域;
    LEFT_TOP_RECT,				// 鼠标在左上角区域;
    TOP_BORDER,			        // 鼠标在上边框区域;
    RIGHT_TOP_RECT,				// 鼠标在右上角区域;
    RIGHT_BORDER,				// 鼠标在右边框区域;
    RIGHT_BOTTOM_RECT,			// 鼠标在右下角区域;
    BOTTOM_BORDER,				// 鼠标在下边框区域;
    LEFT_BOTTOM_RECT,			// 鼠标在左下角区域;
    LEFT_BORDER			     	// 鼠标在左边框区域;
};

class BaseWindow : public QWidget
{
    Q_OBJECT

public:
    BaseWindow(QWidget *parent = 0);
    ~BaseWindow();
    // 设置当前窗口是否支持拉伸;
    void setSupportStretch(bool isSupportStretch);
    // 获取当前窗口是否支持拉伸;
    bool getSupportStretch();

private:
    //初始化标题栏
    void initTitleBar();
    //初始化拖放矩形
    void initStretchRect();

    void paintEvent(QPaintEvent *event);

    //加载QSS样式文件
    void loadStyleSheet(const QString &sheetName);

    // 计算拉伸区域Rect位置;
    void calculateCurrentStrechRect();
    // 根据当前鼠标位置，定位鼠标在具体哪一块拉伸区域;
    WindowStretchRectState getCurrentStretchState(QPoint cursorPos);
    // 根据getCurrentStretchState返回状态进行更新鼠标样式;
    void updateMouseStyle(WindowStretchRectState stretchState);

    // 拉伸窗口过程中，根据记录的坐标更新窗口大小;
    void updateWindowSize();

private:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();

protected:
    MyTitleBar* m_titleBar;

    // 窗口四个角小矩形区域;
    QRect m_leftTopRect;
    QRect m_leftBottomRect;
    QRect m_rightTopRect;
    QRect m_rightBottomRect;

    // 窗口四个边框小矩形区域;
    QRect m_topBorderRect;
    QRect m_rightBorderRect;
    QRect m_bottomBorderRect;
    QRect m_leftBorderRect;

    // 当前鼠标状态;
    WindowStretchRectState m_stretchRectState;

    // 当前窗口是否支持拉伸;
    bool m_isSupportStretch;
    // 当前窗口是否最大化;
    // 如果已经最大化是不支持拉伸的,这一点需要注意;
    bool m_isWindowMax;
    // 鼠标是否按下，用于拉伸功能;
    bool m_isMousePressed;

    // 拉伸窗口的鼠标坐标位置，用于计算拉伸之后的窗口大小;
    QPoint m_startPoint;
    QPoint m_endPoint;

    // 窗口的最小宽度和高度,避免拉伸导致窗口过小或者不可见;
    int m_windowMinWidth;
    int m_windowMinHeight;

    // 拉伸之前的窗口Rect;
    QRect m_windowRectBeforeStretch;
};

#endif // BASEWINDOW_H
