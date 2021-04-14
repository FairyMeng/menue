#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QDirModel>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QTreeView>
#include "basewindow.h"
#include "message_cancel.h"
#include "databaseconnector.h"
#include "pulsefilemanager.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
namespace Ui {
class mywindow;
}
class mywindow : public BaseWindow
{
    Q_OBJECT
public:
     explicit mywindow(QWidget *parent = 0);
     ~mywindow();
     Ui::mywindow *ui;
     Message_cancel *m_cancel;
     int fileCounts;//右侧tableWidget显示的文件总数
     QString filePathForQuickLoadFile(int row);//mainwindow界面快速载入文件,获得文件路径
     int pulseFileNumber;//表示选中当前文件的序号
     QString pulseFileName;//表示当前文件的名字
     QString pulseFilePath;//表示当前文件的路径
     bool myWindowIsClose;//判断当前窗口是否关闭
     bool pulseFileTypeDeterminite(QFileInfo fileInfo);//判断文件类型
protected:
     void initTitleBar();//初始化标题栏
     void closeEvent(QCloseEvent *event);//重写该窗口的关闭事件
private:
    QDirModel *model;
    QTreeView *tree;
    bool isCtrlSelected;//判断是进行单选还是多选
    bool itemSelected;//是否选中状态标识


    QStringList specifiedColumnItem;//单击某行获得指定的列的内容
    QStringList fileNameList;//右侧列表文件名
    QStringList filePathList;//右侧列表路径名
    QString fileName;//左侧列表文件名
    QString filePath;//左侧列表文件路径



    void setTableAttributes();//设置表格属性
    void AddHardDiskItemsToCombobox();//添加硬盘列表到下拉列表中
    void getFileNamesInDiskInDefault(QString path);//显示TXT文件
    void ExecuteSearchFile(const QDir &dir,const QString &pulseFileName);//查询指定文件夹的文件
    void ShowFiles(const QDir &dir,const QStringList &files);//显示文件到文件列表于tableWidget中
    void AddFileInTablewidget2WithClick();//将单击选中的文件添加到右侧列表
    void AddFileInTablewidget2WithCtrl();//将多选选中的文件添加到右侧列表
    QList<int> saveDifferentFileName(int index,QString strFileName,QString filePath,QStringList fileList,QStringList pathList);//返回不同的文件下标

signals:
    void openFile(QFileInfo fileInfo);
    void closeMyWindow();

//    void left_TableWidget_doubleClicked();
//    void right_TableWidget_doubleClicked();
private slots:
    void currentTextChangedSLotInComboBox(QString str);//下拉选目录发生改变
    void SearchDirs(QDir dir);//显示当前目录下的子目录和文件
    //void openPulseFileSlots();//载入文件槽函数
    void openMessage_cancelSlots();
    void pushButton_findFileClicked();//查找按钮槽函数
    //void on_pushButton_onBrowse_clicked();
    void ClearContentsSLot();//清空上次查找记录

    void TableWidget_clickedItem(QTableWidgetItem *item);//鼠标单击左侧列表
    void TableWidget_doubleClickedItem(QTableWidgetItem *item);//鼠标双击左侧列表

    void TableWidget2_doubleClickedItem(QTableWidgetItem *item);//鼠标双击右侧列表，移除文件

    void pushButton_ChoosenSlot();//添加文件按钮
    void pushButton_RemoveSlot();//移除选中文件
    void pushButton_RemoveAllSlot();//移除全部文件
    void pushButton_loadFileSlot();//加载文件按钮
};

#endif // MYWINDOW_H
