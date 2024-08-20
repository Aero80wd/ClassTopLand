#ifndef YIYANDIALOG_H
#define YIYANDIALOG_H

#include <QDialog>
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonValue>
#include<QJsonObject>
#include<QJsonParseError>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QFile>
#include<QGraphicsDropShadowEffect>
#include<QDir>
#include<QPainter>
#include<QPropertyAnimation>
#include<QSizeGrip>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "qt_windows.h"
#include<QTranslator>
#include"AppLog.h"
namespace Ui {
class yiyanDialog;
}

class yiyanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit yiyanDialog(QWidget *parent = nullptr);
    ~yiyanDialog();
    bool eventFilter(QObject *watched,QEvent* e) override;
    void reject() override;
    QJsonObject Config;
    void setConfig(QJsonObject config){
        Config = config;
    }
    void paintEvent(QPaintEvent *event) override;
    QString getToken();
    QString CONFIG_JSON = QDir::currentPath() + "/config.json";

    QPropertyAnimation* back_show;
    QPropertyAnimation* ani_opty;
    void refechZuanyan();
    QPoint startPos;
    QSqlDatabase db;
    QPoint endPos;
    void setWidgetBlur(QWidget* widget);
    bool ZuanYanisOpen = false;
    void setZuanYanOpen(bool v){
        ZuanYanisOpen = v;
        readConfig();
    }
    bool tracking = false;
    virtual void closeEvent(QCloseEvent* event) override;
    void readConfig();
public slots:
    void refechYiYan();
private:
    Ui::yiyanDialog *ui;
};

#endif // YIYANDIALOG_H
