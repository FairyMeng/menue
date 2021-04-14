#ifndef PRIJINGCE_H
#define PRIJINGCE_H

#include <QMainWindow>

namespace Ui {
    class PRIjingce;
}

class PRIjingce : public QMainWindow
{
    Q_OBJECT

public:
    explicit PRIjingce(QWidget *parent = 0);
    ~PRIjingce();

private:
    Ui::PRIjingce *ui;
};

#endif // PRIJINGCE_H
