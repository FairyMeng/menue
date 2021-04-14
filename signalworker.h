#ifndef SIGNALWORKER_H
#define SIGNALWORKER_H
#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QMetaType>
class SignalWorker : public QObject
{
    Q_OBJECT
public:
    SignalWorker();
signals:

    //将脉冲文件存入数据库中
    void Sig_GetDatabasesName(QFileInfo fileInfo);

public slots:
    void emitSig_GetDatabasesName(QFileInfo fileInfo);

};
#endif // SIGNALWORKER_H
