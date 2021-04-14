#ifndef SLOTWORKER_H
#define SLOTWORKER_H
#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QMetaType>
class SlotWorker : public QObject
{
    Q_OBJECT
public:
    SlotWorker();
public slots:
    void Slot_GetDatabasesName(QFileInfo fileInfo);
};
#endif // SLOTWORKER_H
