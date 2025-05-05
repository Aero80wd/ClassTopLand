#pragma once
#include<QUrl>
#define MAKE_EXAPI_URL(apiname,method) (QUrl(QString("http://aero8m.cloud:8765/exapi/%1/%2").arg(apiname, method)))
namespace CloudAPIUrl {
	static QUrl MASTER = QUrl("http://aero8m.cloud:8765");
	static QString MASTER_STRING = "http://aero8m.cloud:8765";
	// user api
	static QUrl REGISTER = QUrl(MASTER_STRING + "/register");
    static QUrl GET_TOKEN = QUrl(MASTER_STRING + "/get_token");
	static QUrl ISTOKEN = QUrl(MASTER_STRING + "/is_true_token");
	static QUrl GET_USERINFO = QUrl(MASTER_STRING + "/get_userinfo");
	// class mange api
    static QUrl GET_CLASS_LIST = QUrl(MASTER_STRING + "/get_class");
    static QUrl GET_CLASS_INFO = QUrl(MASTER_STRING + "/get_class_info");
    static QUrl ADD_CLASS = QUrl(MASTER_STRING + "/add_class");
    static QUrl DELETE_CLASS = QUrl(MASTER_STRING + "/delete_class");
	static QUrl GET_EXAPI_LIST = QUrl(MASTER_STRING + "/exapi/list");
};