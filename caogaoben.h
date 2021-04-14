#ifndef CAOGAOBEN_H
#define CAOGAOBEN_H

#include <QMainWindow>

namespace Ui {
    class caogaoben;
}

class caogaoben : public QMainWindow
{
    Q_OBJECT

public:
    explicit caogaoben(QWidget *parent = 0);
    ~caogaoben();

private:
    Ui::caogaoben *ui;
};

#endif // CAOGAOBEN_H
