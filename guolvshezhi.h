#ifndef GUOLVSHEZHI_H
#define GUOLVSHEZHI_H

#include <QWidget>
#include "guolvshezhi2.h"

namespace Ui {
    class guolvshezhi;
}

class guolvshezhi : public QWidget
{
    Q_OBJECT

public:
    explicit guolvshezhi(QWidget *parent = 0);
    ~guolvshezhi();

private:
    Ui::guolvshezhi *ui;
    guolvshezhi2 *guolv2;
private slots:
    void openguolvshezhi2Slots();
};

#endif // GUOLVSHEZHI_H
