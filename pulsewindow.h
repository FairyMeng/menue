#ifndef IMPULSE_SEPARATION_H
#define IMPULSE_SEPARATION_H

#include <QWidget>
#include <QProxyStyle>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>

#include "basewindow.h"
#include "view.h"
#include "ui_PulseWindow.h"


namespace Ui {
class PulseWindow;
}

class PulseWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit PulseWindow(QWidget *parent = 0);
    ~PulseWindow();

    Ui::PulseWindow *ui;
    View view_;
    QRect clientRect;//设置一个存放屏幕分辨率的矩形框

private:
       void initTitleBar();
       
signals:
      void PulseWindow_close();
      //当窗口状态变为激活状态时，发射信号
      void AllWindowActivate(QObject *obj);
      //鼠标在窗口松开时，发射信号
      void MouseRelease(QObject *obj);

private Q_SLOTS:


protected:
    bool eventFilter(QObject *watched, QEvent *event);
	void closeEvent(QCloseEvent *event);
};

#endif // IMPULSE_SEPARATION_H
