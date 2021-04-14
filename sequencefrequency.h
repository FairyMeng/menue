#ifndef SEQUENCEFREQUENCY_H
#define SEQUENCEFREQUENCY_H

#include <QMainWindow>

namespace Ui {
    class SequenceFrequency;
}

class SequenceFrequency : public QMainWindow
{
    Q_OBJECT

public:
    explicit SequenceFrequency(QWidget *parent = 0);
    ~SequenceFrequency();

private:
    Ui::SequenceFrequency *ui;
};

#endif // SEQUENCEFREQUENCY_H
