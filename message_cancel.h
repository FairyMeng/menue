#ifndef MESSAGE_CANCEL_H
#define MESSAGE_CANCEL_H

#include <QMainWindow>
#include <QFileInfo>
namespace Ui {
    class Message_cancel;
}

class Message_cancel : public QMainWindow
{
    Q_OBJECT

public:
    explicit Message_cancel(QWidget *parent = 0);
    ~Message_cancel();

    bool isClicked=false;
    bool isClose=false;
private:
    Ui::Message_cancel *ui;
signals:
     void openFile(QFileInfo fileInfo);
private slots:
    void pushButton_enterSlot();
    void pushButton_cancelSlot();
};

#endif // MESSAGE_CANCEL_H
