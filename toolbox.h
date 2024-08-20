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
#include"clicklabel.h"
#include <dwmapi.h>
#pragma comment(lib,"Dwmapi.lib")
#include<QColorDialog>
#include"AppLog.h"
namespace Ui {
class ToolBox;
}

class ToolBox : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBox(QWidget *parent = nullptr);
    ~ToolBox();

    void setWidgetBlur(QWidget* widget);
public slots:
    void LoadPlugins();
private:
    Ui::ToolBox *ui;
};

#endif // TOOLBOX_H
