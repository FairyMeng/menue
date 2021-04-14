#ifndef PIONTLINEMEASURING_H
#define PIONTLINEMEASURING_H

#include <QMainWindow>

namespace Ui {
    class PiontLineMeasuring;
}

class PiontLineMeasuring : public QMainWindow
{
    Q_OBJECT

public:
    explicit PiontLineMeasuring(QWidget *parent = 0);
    ~PiontLineMeasuring();

private:
    Ui::PiontLineMeasuring *ui;
};

#endif // PIONTLINEMEASURING_H
