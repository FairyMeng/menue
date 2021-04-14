#ifndef MAICHONGCUNCHU_H
#define MAICHONGCUNCHU_H

#include <QWidget>

namespace Ui {
    class maichongcunchu;
}

class maichongcunchu : public QWidget
{
    Q_OBJECT

public:
    explicit maichongcunchu(QWidget *parent = 0);
    ~maichongcunchu();

private:
    Ui::maichongcunchu *ui;
};

#endif // MAICHONGCUNCHU_H
