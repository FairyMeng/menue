#ifndef SHUZHIJISUAN1_H
#define SHUZHIJISUAN1_H

#include <QWidget>

namespace Ui {
    class shuzhijisuan1;
}

class shuzhijisuan1 : public QWidget
{
    Q_OBJECT

public:
    explicit shuzhijisuan1(QWidget *parent = 0);
    ~shuzhijisuan1();

private:
    Ui::shuzhijisuan1 *ui;
};

#endif // SHUZHIJISUAN1_H
