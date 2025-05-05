#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QDir>
#include <QJsonObject>
#include <QJsonParseError>
#include<QJsonDocument>
#include <QMessageBox>
#include<QVBoxLayout>
#include <QInputDialog>
#include <QPixmap>
#include "../AppLog/AppLog.h"
#include "ui_CloudSettingsTab.h"
#include "../Utils/Utils.h"
#include "../NetworkRequests/NetworkRequests.h"
#include "../CloudAPI.h"

class CloudSettingsTab : public QWidget
{
	Q_OBJECT

public:
	CloudSettingsTab(QWidget *parent = nullptr);
	~CloudSettingsTab();
private:
	void initUi();
	void initUserInfo();
	void readConfig();
	void registerUser();
	void loginUser();
	void exitLogin();
	//void getUserClasses();
private slots:
	//void addClass();
	//void deleteClass();
private:
	NetworkRequests* m_priReq;
	bool m_isLogin = false;
	QJsonObject Config;
	QNetworkAccessManager* m_networkManager;
	QNetworkAccessManager* m_networkManager2;
private:
	Ui::CloudSettingsTabClass ui;
};
