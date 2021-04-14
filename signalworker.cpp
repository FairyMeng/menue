#include "signalworker.h"
SignalWorker::SignalWorker():QObject(){
        qRegisterMetaType<QFileInfo>("QFileInfo");
}
void SignalWorker::emitSig_GetDatabasesName(QFileInfo fileInfo){
    emit Sig_GetDatabasesName(fileInfo);
}
