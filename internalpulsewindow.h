#ifndef INTERNALPULSEWINDOW_H
#define INTERNALPULSEWINDOW_H


#include <QWidget>
#include "internalpulseview.h"
#include "basewindow.h"

namespace Ui {
class InternalPulseWindow;
}

class InternalPulseWindow :public BaseWindow
{
    Q_OBJECT

public:
    explicit InternalPulseWindow(QWidget *parent = 0);
    ~InternalPulseWindow();
    InternalPulseView internalView;//脉内视图
    QRect clientRect;//设置一个存放屏幕分辨率的矩形框

private: 
    Ui::InternalPulseWindow *ui;
    void initTitleBar();
    void closeEvent(QCloseEvent *event);
signals:
    void InternalPulseWindow_close();
    //当窗口状态变为激活状态时，发射信号
    void AllWindowActivate(QObject *obj);
    //鼠标在窗口松开时，发射信号
    void MouseRelease(QObject *obj);
private slots:

protected:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // INTERNALPULSEWINDOW_H
