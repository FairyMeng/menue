#ifndef FILELOADER_H
#define FILELOADER_H

#include"databaseoperator.h"
#include"structdefine.h"
#include<QFile>
#include<QFileInfo>
#include<QDataStream>
#include <QMessageBox>


/*将.txt文件载入数据库：
*将.txt文件内的数据，存入数据库
*/
//解决中文乱码
#pragma execution_character_set("utf-8")
class PulseFileManager
{
public:
    PulseFileManager();
    static void LoadInterPulseToDatabase(QFileInfo file_info);
    //加载第一次给定的数据
    static QVector<InterPulse> LoadInterPulseFileDataIntoMemVector(QFile *file);
    static QVector<int>LoadInternalPulseFileDataIntoMemVector(QFile *file);
private:
    //struct interpulse ip;
};

#endif // FILELOADER_H
