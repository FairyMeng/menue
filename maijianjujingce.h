#ifndef MAIJIANJUJINGCE_H
#define MAIJIANJUJINGCE_H

#include <QMainWindow>

namespace Ui {
    class maijianjujingce;
}

class maijianjujingce : public QMainWindow
{
    Q_OBJECT

public:
    explicit maijianjujingce(QWidget *parent = 0);
    ~maijianjujingce();

private:
    Ui::maijianjujingce *ui;
};

#endif // MAIJIANJUJINGCE_H
