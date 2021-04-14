#include "matchsearch.h"
#include "ui_matchsearch.h"

matchsearch::matchsearch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::matchsearch)
{
    ui->setupUi(this);
}

matchsearch::~matchsearch()
{
    delete ui;
}
