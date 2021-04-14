#ifndef RADARPERFORM_H
#define RADARPERFORM_H

#include <QMainWindow>

namespace Ui {
    class RadarPerform;
}

class RadarPerform : public QMainWindow
{
    Q_OBJECT

public:
    explicit RadarPerform(QWidget *parent = 0);
    ~RadarPerform();

private:
    Ui::RadarPerform *ui;
};

#endif // RADARPERFORM_H
