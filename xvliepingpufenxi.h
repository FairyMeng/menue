#ifndef XVLIEPINGPUFENXI_H
#define XVLIEPINGPUFENXI_H

#include <QWidget>
#include <QApplication>

namespace Ui {
    class xvliepingpufenxi;
}

class xvliepingpufenxi : public QWidget
{
    Q_OBJECT

public:
    explicit xvliepingpufenxi(QWidget *parent = 0);
    ~xvliepingpufenxi();

    void setupPlot();

    private slots:
      void horzScrollBarChanged(int value);
      void vertScrollBarChanged(int value);
      //void xAxisChanged(QCPRange range);
      //void yAxisChanged(QCPRange range);

private:
    Ui::xvliepingpufenxi *ui;
};

#endif // XVLIEPINGPUFENXI_H
