#include "./src/MainTableWidget/MainTableWidget.h"
#include "./src/ToolBox/ToolBox.h"
#include"./src/DayTimerWidget/DayTimerWidget.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include<QDir>
#include<QDateTime>
#include<QProcess>
#include<QFontDatabase>
#include<QStyleFactory>
#include <iostream>
#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
# pragma execution_character_set("utf-8")    
#endif
#include"./src/AppLog/AppLog.h"
#include"./src/NetworkRequests/NetworkRequests.h"
#include"./src/SyncingDialog/SyncingDialog.h"
#include "./src/Utils/Utils.h"
bool ToolBoxisOpen(){
    QFile file(QDir::currentPath() + "/config.json");
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    QTextStream stream(&file);
    QString file_str = stream.readAll();
    file.close();
    QJsonParseError jsonError;
    QJsonDocument jsondoc = QJsonDocument::fromJson(file_str.toUtf8(),&jsonError);
    if (jsonError.error != QJsonParseError::NoError && !jsondoc.isNull()) {
        showLog("Config.json is Error!",LogStatus::ERR);
        return true;
    }
    QJsonObject Config = jsondoc.object();
    if (Config.contains("toolbox_status")){
        if (Config["toolbox_status"].toBool()){
            showLog("ToolBox is Not Show",LogStatus::INFO);
            return false;
        }else{
            showLog("ToolBox is Show",LogStatus::INFO);
            return true;
        }

    }
    showLog("ToolBox is Show",LogStatus::INFO);
    return true;
}
bool TimerisOpen(){
    QFile file(QDir::currentPath() + "/config.json");
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    QTextStream stream(&file);
    QString file_str = stream.readAll();
    file.close();
    QJsonParseError jsonError;
    QJsonDocument jsondoc = QJsonDocument::fromJson(file_str.toUtf8(),&jsonError);
    if (jsonError.error != QJsonParseError::NoError && !jsondoc.isNull()) {
        showLog("Config.json is Error!",LogStatus::ERR);
        return true;
    }
    QJsonObject Config = jsondoc.object();
    if (Config.contains("disable_timer")){

        if (Config["disable_timer"].toBool()){
            showLog("Timer is Not Show",LogStatus::INFO);
            return false;
        }else{
            showLog("Timer is Show",LogStatus::INFO);
            return true;
        }
    }
    showLog("Timer is Show",LogStatus::INFO);
    return true;
}
bool SyncisOpen(){
    QFile file(QDir::currentPath() + "/config.json");
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    QTextStream stream(&file);
    QString file_str = stream.readAll();
    file.close();
    QJsonParseError jsonError;
    QJsonDocument jsondoc = QJsonDocument::fromJson(file_str.toUtf8(),&jsonError);
    if (jsonError.error != QJsonParseError::NoError && !jsondoc.isNull()) {
        showLog("Config.json is Error!",LogStatus::ERR);
        return true;
    }
    QJsonObject Config = jsondoc.object();
    if (Config.contains("sync_open")){
        if (!Config["sync_open"].toBool()){
            showLog("Sync is Not Open",LogStatus::INFO);
            return false;
        }else{
            showLog("Sync is open",LogStatus::INFO);
            return true;
        }
    }
    showLog("Sync is not open",LogStatus::INFO);
    return false;
}
void printLogo() {
    QFile file(":/res/logo.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&file);
    QString file_str = stream.readAll();
    file.close();
    std::cout << file_str.toStdString() << std::endl;
    std::cout << "---------------------------------------------------------------------------" << std::endl;
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    printLogo();
    a.setApplicationDisplayName("ClassTopLand");
    a.setStyleSheet(getStyleSheet(":/qss/global.qss"));
    QApplication::setQuitOnLastWindowClosed(false);
    showLog("MainWindow is Show",LogStatus::INFO);
    SyncingDialog *syncDialog = new SyncingDialog();

    if (SyncisOpen())
    {
        syncDialog->syncRemoteJson();
    }
    MainTableWidget *w = new MainTableWidget();
    w->show();

    QScreen *scr = a.primaryScreen();
    int scr_w = scr->size().width();
    int scr_h = scr->size().height();
    w->move((scr_w - w->width()) / 2, 0);
    ToolBox *tb = new ToolBox();
    tb->move((scr_w - tb->width()) * 0.9975, (scr_h - tb->height()) / 2*0.9);
    if (ToolBoxisOpen()){
        tb->show();
    }
    DayTimerWidget *dtw = new DayTimerWidget();
    dtw->move((scr_w - dtw->width()),(scr_h - dtw->height()) * 0.95);
    if (TimerisOpen()) {
        dtw->show();
    }
    QObject::connect(w->EditWindow,&TableEditWidget::refechToolBar_signal,tb,&ToolBox::LoadPlugins);
    return a.exec();
}
