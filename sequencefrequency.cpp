#include "sequencefrequency.h"
#include "ui_sequencefrequency.h"

SequenceFrequency::SequenceFrequency(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SequenceFrequency)
{
    ui->setupUi(this);
}

SequenceFrequency::~SequenceFrequency()
{
    delete ui;
}
