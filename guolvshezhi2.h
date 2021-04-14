#ifndef GUOLVSHEZHI2_H
#define GUOLVSHEZHI2_H

#include <QWidget>

namespace Ui {
    class guolvshezhi2;
}

class guolvshezhi2 : public QWidget
{
    Q_OBJECT

public:
    explicit guolvshezhi2(QWidget *parent = 0);
    ~guolvshezhi2();

private:
    Ui::guolvshezhi2 *ui;
};

#endif // GUOLVSHEZHI2_H
