#ifndef IMPULSE_SECOND_FORM_H
#define IMPULSE_SECOND_FORM_H

#include <QWidget>
#include <QDebug>
#include <QProxyStyle>
#include "view.h"
#include "basewindow.h"

namespace Ui {
class impulse_second_form;
}

class impulse_second_form : public BaseWindow
{
    Q_OBJECT

public:
    explicit impulse_second_form(QWidget *parent = 0);
    ~impulse_second_form();
        //QVector<InterPulse> million_data_vector_;


private:
    Ui::impulse_second_form *ui;
    void initTitleBar();
    //void closeEvent(QCloseEvent *event);
signals:
        //void impulse_Clustering_close();
private slots:
    //void AddMouseSelectedPulseInfoIntoListSlots(const QPointF &point);
    //void AddKeyPressSelectedPulseInfoIntoListSlots(Chart *chart, QKeyEvent *event);
    //void AddRubberBandSelectedPulseInfoIntoListSlots(QVector<int> rubberband_points_x);
};

#endif // IMPULSE_SECOND_FORM_H
