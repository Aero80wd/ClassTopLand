#pragma once

#include <QWidget>
#include "ui_ExportAPISettingsTab.h"
#include "../CloudAPI.h"
#include "../NetworkRequests/NetworkRequests.h"
#include<QMessageBox>
#include<QDir>
QT_BEGIN_NAMESPACE
namespace Ui { class ExportAPISettingsTabClass; };
QT_END_NAMESPACE

class ExportAPISettingsTab : public QWidget
{
	Q_OBJECT

public:
	ExportAPISettingsTab(QWidget *parent = nullptr);
	~ExportAPISettingsTab();
	
private:
	Ui::ExportAPISettingsTabClass *ui;
	void initAPIList();
	NetworkRequests* apilist_req;
	NetworkRequests* exapitb_req;
	NetworkRequests* exapitk_req;
private slots:
	void SyncExAPITable();
};
