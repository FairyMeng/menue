#ifndef DATAFIT_H
#define DATAFIT_H

#include <QMainWindow>

namespace Ui {
    class Datafit;
}

class Datafit : public QMainWindow
{
    Q_OBJECT
public:
    explicit Datafit(QWidget *parent = 0);
    ~Datafit();

private:
    Ui::Datafit *ui;
};

#endif // DATAFIT_H

