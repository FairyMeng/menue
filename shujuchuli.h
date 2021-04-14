#ifndef SHUJUCHULI_H
#define SHUJUCHULI_H

#include <QMainWindow>

namespace Ui {
    class shujuchuli;
}

class shujuchuli : public QMainWindow
{
    Q_OBJECT

public:
    explicit shujuchuli(QWidget *parent = 0);
    ~shujuchuli();

private:
    Ui::shujuchuli *ui;
};

#endif // SHUJUCHULI_H
