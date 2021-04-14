#ifndef MATCHSEARCH_H
#define MATCHSEARCH_H

#include <QMainWindow>

namespace Ui {
    class matchsearch;
}

class matchsearch : public QMainWindow
{
    Q_OBJECT

public:
    explicit matchsearch(QWidget *parent = 0);
    ~matchsearch();

private:
    Ui::matchsearch *ui;
};

#endif // MATCHSEARCH_H
