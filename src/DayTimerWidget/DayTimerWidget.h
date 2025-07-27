//
// Created by JH182 on 2024/8/21.
//

#ifndef SCHOOLTOOLS_DAYTIMERWIDGET_H
#define SCHOOLTOOLS_DAYTIMERWIDGET_H

#include <QWidget>
#include<QTimer>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonDocument>
#include<QFile>
#include<QDir>
#include<QFont>
#include<qfontdatabase.h>
#include"../AppLog/AppLog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class DayTimerWidget; }
QT_END_NAMESPACE

class DayTimerWidget : public QWidget {
Q_OBJECT

public:
    explicit DayTimerWidget(QWidget *parent = nullptr);
    QTimer* timer_timer;
    QFontMetrics *fm = nullptr;
    void readTimeJson();
    QJsonObject TimeJson;
    ~DayTimerWidget() override;
public slots:
    void reload_timer();
private:
    Ui::DayTimerWidget *ui;
};


#endif //SCHOOLTOOLS_DAYTIMERWIDGET_H
