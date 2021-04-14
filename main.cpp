#include <QApplication>
#include <QDesktopWidget>
#include <QTime>
#include <QDebug>
#include <QThread>
#include "mainwindow.h"
#include "slotworker.h"
#include "signalworker.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //解决中文乱码

    //设置窗口图标
    a.setWindowIcon(QIcon(":/new/icon/images/app_icon.png"));

     MainWindow main_window;
     QTime time;
     time.start();

     QPixmap pixmap(":/new/icon/images/start_background.jpg");
     QSplashScreen screen(pixmap);
     screen.show();
     screen.showMessage("loading....", Qt::AlignCenter, Qt::red);

     int delayTime=1;
     QElapsedTimer timer;
     timer.start();
     while(timer.elapsed() < (delayTime * 1000))
     {
          a.processEvents();
     }
     main_window.SetAllWindowsSizeAndShow();
     screen.finish(&main_window);
     qDebug()<<"all windows show up time is"<< time.elapsed()/1000<<"s";
     return a.exec();
}


