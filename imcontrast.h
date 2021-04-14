#ifndef IMCONTRAST_H
#define IMCONTRAST_H

#include <QMainWindow>

namespace Ui {
    class Imcontrast;
}

class Imcontrast : public QMainWindow
{
    Q_OBJECT

public:
    explicit Imcontrast(QWidget *parent = 0);
    ~Imcontrast();

private:
    Ui::Imcontrast *ui;
};

#endif // IMCONTRAST_H
