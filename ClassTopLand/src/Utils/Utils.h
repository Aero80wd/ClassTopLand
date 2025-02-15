#ifndef UTILS_H
#define UTILS_H
#include<QFile>
#include<QWidget>
#include<QSysInfo>
#include"windows.h"
enum SystemType {
	Windows10p,
	Windows10d
};

void loadStyleSheet(QWidget *widget, const QString &styleSheetFile);
SystemType getWindowsVersion();
QString getStyleSheet(const QString &styleSheetFile);
void setWidgetBlur(QWidget* widget,int alpha=0x66000000);
#endif