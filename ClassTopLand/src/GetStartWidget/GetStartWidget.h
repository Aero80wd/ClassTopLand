#ifndef GETSTARTWIDGET_H
#define GETSTARTWIDGET_H

#include <QWidget>
#include<QMouseEvent>
#include<QDialog>
#include<QGraphicsDropShadowEffect>
#include<QPropertyAnimation>
#include "qt_windows.h"
#include<QTranslator>
#include"../AppLog/AppLog.h"
namespace Ui {
class GetStartWidget;
}

class GetStartWidget : public QDialog
{
    Q_OBJECT

public:
    explicit GetStartWidget(QWidget *parent = nullptr);
    ~GetStartWidget();
    void close();
    void initSignal();
    QPropertyAnimation* back_show;
    QPropertyAnimation* ani_opty;
    bool eventFilter(QObject *watched,QEvent* e) override;
    void setWidgetBlur(QWidget* widget);
    QPoint startPos;
    bool tracking = false;
    bool showed = false;
    QPoint endPos;
private:
    Ui::GetStartWidget *ui;

signals:
    void toSettings();
};

#endif // GETSTARTWIDGET_H
