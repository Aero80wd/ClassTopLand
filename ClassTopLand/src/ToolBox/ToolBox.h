#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QWidget>
#include <QtTest/QTest>
#include<QKeyEvent>
#include <windows.h>
#include<QCoreApplication>
#include<QDir>
#include<QLibrary>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonParseError>
#include<QtWebEngineWidgets>
#include"../ClickLabel/ClickLabel.h"
#include <dwmapi.h>
#pragma comment(lib,"Dwmapi.lib")
#include<QColorDialog>
#include"../AppLog/AppLog.h"
#include "../YiyanDialog/YiyanDialog.h"
#include"../PluginWebEngine/PluginWebEngine.h"
#include"../Utils/Utils.h"
namespace Ui {
class ToolBox;
}

class ToolBox : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBox(QWidget *parent = nullptr);
    ~ToolBox();
public slots:
    void LoadPlugins();
signals:
    void reMove();
private:
    Ui::ToolBox *ui;
};

#endif // TOOLBOX_H
