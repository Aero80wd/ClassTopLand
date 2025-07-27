#include "CloudSettingsTab.h"
#include "../Utils/Utils.h"
CloudSettingsTab::CloudSettingsTab(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    readConfig();
    initUserInfo();
}

CloudSettingsTab::~CloudSettingsTab()
{}

void CloudSettingsTab::initUi()
{
    ui.login_but->disconnect();
    ui.register_but->disconnect();
    //ui.addClass->disconnect();
    //ui.deleteClass->disconnect();
	loadStyleSheet(this, ":/qss/CloudSettingsTab.qss");
    connect(ui.register_but, &QPushButton::clicked, this, &CloudSettingsTab::registerUser);
    //connect(ui.addClass, &QPushButton::clicked, this, &CloudSettingsTab::addClass);
    //connect(ui.deleteClass, &QPushButton::clicked, this, &CloudSettingsTab::deleteClass);
    qDebug() << "initUi";
    if (m_isLogin) {
        ui.login_but->setText("退出登录");
        
        connect(ui.login_but, &QPushButton::clicked, this, &CloudSettingsTab::exitLogin);
    }
    else {
        ui.login_but->setText("登录");
        ui.login_but->disconnect();
        connect(ui.login_but, &QPushButton::clicked, this, &CloudSettingsTab::loginUser);
    }
    
}

void CloudSettingsTab::exitLogin()
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("退出登录");
    msgBox->setText("确定退出登录？");
    msgBox->setIcon(QMessageBox::Question);
    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox->setDefaultButton(QMessageBox::Yes);
    msgBox->button(QMessageBox::Yes)->setText("退出");
    msgBox->button(QMessageBox::No)->setText("取消");
    if (msgBox->exec() == QMessageBox::Yes){
        m_isLogin = false;
        Config.remove("ClassTopLand.Cloud.UserToken");
        QFile file(QDir::currentPath() + "/config.json");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream(&file);
        stream << QJsonDocument(Config).toJson();
        file.close();
        //ui.classWidget->clear();
        
        initUserInfo();
        initUi();
        QMessageBox::information(this, "提示", "退出成功！");
    }
}

void CloudSettingsTab::readConfig()
{
    QFile file(QDir::currentPath() + "/config.json");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream stream(&file);
    QString file_str = stream.readAll();
    QJsonParseError jsonError;
    QJsonDocument jsondoc = QJsonDocument::fromJson(file_str.toUtf8(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        showLog("Config.json is Error!", LogStatus::ERR);
        return; // 添加返回语句以避免后续代码执行
    }
    Config = jsondoc.object();
    if (Config.contains("ClassTopLand.Cloud.UserToken")) {
        m_priReq = new NetworkRequests(POST, CloudAPIUrl::ISTOKEN);
        QJsonObject data = {
            {"token", Config["ClassTopLand.Cloud.UserToken"].toString()}
        };
        connect(m_priReq, &NetworkRequests::finished, [=](QJsonObject json, QString reply_string, QString error_string) {
            if (!error_string.isEmpty())
            {
                QMessageBox::critical(this, "错误", "请求失败！");
                return;
            }
            if (json["code"].toInt() == 200) {
                m_isLogin = true;
                
                initUi();
                initUserInfo(); // 登录成功后初始化用户信息
                //getUserClasses();
            }
            });
        m_priReq->start(data); // 修正了这里的数据传递方式
    }
    initUi();
}

void CloudSettingsTab::initUserInfo()
{
    if (m_isLogin) {
        QNetworkRequest request;
        m_networkManager = new QNetworkAccessManager(this);
        request.setUrl(QUrl(CloudAPIUrl::GET_USERINFO));
        request.setRawHeader("token", Config["ClassTopLand.Cloud.UserToken"].toString().toUtf8());
        connect(m_networkManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply* reply) {
            QByteArray bytes = reply->readAll();
            QJsonObject json = QJsonDocument::fromJson(bytes).object();
            qDebug() << json;
            if (json["code"].toInt() == 200) {

                ui.userName->setText(json["name"].toString());
                ui.userRole->setText(QString("用户身份: %1").arg(json["role_name"].toString()));
                QNetworkRequest request;
                m_networkManager2 = new QNetworkAccessManager(this);
                request.setUrl(QUrl(QString("https://q1.qlogo.cn/g?b=qq&nk=%1&s=140").arg(json["qq"].toString())));
                qDebug() << QUrl(QString("https://q1.qlogo.cn/g?b=qq&nk=%1&s=140").arg(json["qq"].toString()));
                connect(m_networkManager2, &QNetworkAccessManager::finished, this, [=](QNetworkReply* reply) {
                    QPixmap pixmap;
                    pixmap.loadFromData(reply->readAll());
                    qDebug() << "加载头像成功";
                    if (pixmap.isNull()) {
                        qDebug() << "Failed to load pixmap!";
                    }
                    ui.avatar->setStyleSheet("");
                    ui.avatar->setAlignment(Qt::AlignCenter);
                    ui.avatar->setPixmap(pixmap);
                    ui.avatar->update();
                });
                m_networkManager2->get(request);
            }
        });
        m_networkManager->get(request);
    }
    else {
        
        ui.userName->setText("未登录");
        ui.userRole->setText("用户身份: guest");
        ui.avatar->setPixmap(QPixmap());
        ui.avatar->setAlignment(Qt::AlignCenter);
        ui.avatar->setStyleSheet("image:url(\":/res/default_user.png\")");
    }
}
void CloudSettingsTab::loginUser() {
    if (m_isLogin) {
        QMessageBox::information(this, "提示", "将会覆盖原先登录账号！");
    }
    QWidget* login_window = new QWidget();
    login_window->setWindowTitle("登录");
    QLabel* title_label = new QLabel(login_window);
    title_label->setText("登录");
    title_label->setAlignment(Qt::AlignCenter);
    title_label->setStyleSheet("QLabel{font-size:20px;}");
    QLineEdit* userNameInput = new QLineEdit(login_window);
    userNameInput->setPlaceholderText("用户名");
    userNameInput->setMinimumHeight(27);
    
    QLineEdit* userPwdInput = new QLineEdit(login_window);
    userPwdInput->setPlaceholderText("密码");
    userPwdInput->setMinimumHeight(27);
    userPwdInput->setEchoMode(QLineEdit::Password);
    QPushButton* login_btn = new QPushButton(login_window);
    login_btn->setText("登录");
    login_btn->setMinimumHeight(27);
    QVBoxLayout* layout = new QVBoxLayout(login_window);
    layout->addWidget(title_label);
    layout->addWidget(userNameInput);
    layout->addWidget(userPwdInput);
    layout->addWidget(login_btn);
    layout->setStretch(0, 1);
    layout->setStretch(1, 0);
    layout->setStretch(2, 0);
    layout->setStretch(3, 0);
    login_window->setLayout(layout);
    connect(userNameInput, &QLineEdit::returnPressed, this, [=] {
        userPwdInput->setFocus();
    });
    connect(userPwdInput, &QLineEdit::returnPressed, this, [=] {
        login_btn->click();
    });

    connect(login_btn, &QPushButton::clicked, [=]() {
        if (userNameInput->text().isEmpty() || userPwdInput->text().isEmpty()) {
            QMessageBox::critical(this, "错误", "用户名或密码不能为空！");
            return;
        }
        m_priReq = new NetworkRequests(POST, CloudAPIUrl::GET_TOKEN);
        QJsonObject data = {
            {"name", userNameInput->text()},
            {"password", userPwdInput->text()},
        };
        connect(m_priReq, &NetworkRequests::finished, [=](QJsonObject json, QString reply_string, QString error_string) {
            if (!error_string.isEmpty())
            {

                QMessageBox::critical(this, "错误", "请求失败！");
                initUi();
                return;
            }
            if (json["code"].toInt() == 200) {
                Config["ClassTopLand.Cloud.UserToken"] = json["token"].toString();
                QFile file(QDir::currentPath() + "/config.json");
                file.open(QIODevice::ReadWrite | QIODevice::Text);
                QTextStream stream(&file);
                stream << QJsonDocument(Config).toJson();
                file.close();
                QMessageBox::information(this, "成功", "登录成功！");
                m_isLogin = true;
                login_window->close();
                initUi();
                initUserInfo();
                //getUserClasses();
            }
            else {
                QMessageBox::critical(this, "错误", json["msg"].toString());
            }
        });
        m_priReq->start(data);


    });
    login_window->show();
}
void CloudSettingsTab::registerUser() {
    QWidget* register_window = new QWidget();
    register_window->setWindowTitle("注册");
    QLabel* title_label = new QLabel(register_window);
    title_label->setText("注册");
    title_label->setAlignment(Qt::AlignCenter);
    title_label->setStyleSheet("QLabel{font-size:20px;}");
    QLineEdit* userNameInput = new QLineEdit(register_window);
    userNameInput->setPlaceholderText("用户名");
    userNameInput->setMinimumHeight(27);
    QLineEdit* userPwdInput = new QLineEdit(register_window);
    userPwdInput->setPlaceholderText("密码");
    userPwdInput->setMinimumHeight(27);
    userPwdInput->setEchoMode(QLineEdit::Password);
    QLineEdit* userQQInput = new QLineEdit(register_window);
    userQQInput->setPlaceholderText("QQ号(请勿乱填)");
    userQQInput->setMinimumHeight(27);
    QPushButton* register_btn = new QPushButton(register_window);
    register_btn->setText("注册");
    register_btn->setMinimumHeight(27);
    QVBoxLayout* layout = new QVBoxLayout(register_window);
    layout->addWidget(title_label);
    layout->addWidget(userNameInput);
    layout->addWidget(userPwdInput);
    layout->addWidget(userQQInput);
    layout->addWidget(register_btn);
    layout->setStretch(0, 1);
    layout->setStretch(1, 0);
    layout->setStretch(2, 0);
    layout->setStretch(3, 0);
    layout->setStretch(4, 0);
    register_window->setLayout(layout);
    connect(userNameInput, &QLineEdit::returnPressed, this, [=] {
        userPwdInput->setFocus();
    });
    connect(userPwdInput, &QLineEdit::returnPressed, this, [=] {
        userQQInput->setFocus();
    });
    connect(userQQInput, &QLineEdit::returnPressed, this, [=] {
        register_btn->click();
    });
    register_window->show();
    connect(register_btn, &QPushButton::clicked, [=]() {
        if (userNameInput->text().isEmpty() || userPwdInput->text().isEmpty() || userQQInput->text().isEmpty()) {
            QMessageBox::critical(this, "错误", "请填写完整信息！");
            return;
        }
        m_priReq = new NetworkRequests(POST, CloudAPIUrl::REGISTER);
        QJsonObject data = {
            {"name", userNameInput->text()},
            {"password", userPwdInput->text()},
            {"qq", userQQInput->text()}
        };
        connect(m_priReq, &NetworkRequests::finished, [=](QJsonObject json, QString reply_string, QString error_string) {
            if (!error_string.isEmpty())
            {
                QMessageBox::critical(this, "错误", "请求失败！");
                return;
            }
            qDebug() << json;
            if (json["code"].toInt() == 200) {
                QMessageBox::information(this, "成功", "注册成功！请查看您QQ号下属QQ邮箱，打开验证邮件，对账号进行验证。");
                register_window->close();
            }
            else {
                QMessageBox::critical(this, "错误", json["msg"].toString());
            }
            });
        m_priReq->start(data); // 修正了这里的数据传递方式
        });
}

//void CloudSettingsTab::getUserClasses() {
//    ui.classWidget->clear();
//    if (m_isLogin) {
//        ui.classWidget->clear();
//        m_priReq = new NetworkRequests(POST, CloudAPIUrl::GET_CLASS_LIST);
//        connect(m_priReq, &NetworkRequests::finished, [=](QJsonObject json, QString reply_string, QString error_string) {
//            if (!error_string.isEmpty())
//            {
//                QMessageBox::critical(this, "错误", "请求失败！");
//                return;
//            }
//            if (json["code"].toInt() == 200) {
//                QJsonArray classes = json["data"].toArray();
//                for (int i = 0; i < classes.size(); i++) {
//                    QJsonObject class_obj = classes[i].toObject();
//                    QString class_name = class_obj["name"].toString();
//                    QString class_byuser = class_obj["byusername"].toString();
//                    QListWidgetItem * item = new QListWidgetItem();
//                    item->setData(Qt::UserRole, class_obj["id"].toInteger());
//                    item->setText(QString("%1 (By %2)").arg(class_name, class_byuser));
//                    ui.classWidget->addItem(item);
//                }
//            }
//            });
//        QJsonObject headers = { {"token",Config["ClassTopLand.Cloud.UserToken"].toString()} };
//        m_priReq->start(QJsonObject(), headers);
//    }
//    
//
//}
//
//void CloudSettingsTab::deleteClass()
//{
//    QListWidgetItem * item = ui.classWidget->currentItem();
//    if (item == nullptr) {
//        QMessageBox::critical(this, "错误", "请选择要删除的班级！");
//        return;
//    }
//    if (QMessageBox::warning(this, "警告", "确定要删除该班级吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
//        return;
//    }
//    m_priReq = new NetworkRequests(POST, CloudAPIUrl::DELETE_CLASS);
//    connect(m_priReq, &NetworkRequests::finished, [=](QJsonObject json, QString reply_string, QString error_string) {
//        if (!error_string.isEmpty())
//        {
//            QMessageBox::critical(this, "错误", "请求失败！");
//            return;
//        }
//        if (json["code"].toInt() == 200) {
//            QMessageBox::information(this, "成功", "删除成功！");
//            ui.classWidget->removeItemWidget(item);
//            delete item;
//        }
//        else {
//            QMessageBox::critical(this, "错误", json["msg"].toString());
//        }
//        getUserClasses();
//
//
//    });
//    QJsonObject data = {
//        {"class", item->data(Qt::UserRole).toInt()}
//    };
//    QJsonObject headers = { {"token",Config["ClassTopLand.Cloud.UserToken"].toString()} };
//    m_priReq->start(data,headers);
//
//
//}
//
//void CloudSettingsTab::addClass()
//{
//    QInputDialog* input_dialog = new QInputDialog(this);
//    input_dialog->setWindowTitle("添加班级");
//    input_dialog->setLabelText("请输入班级名称：");
//    input_dialog->setTextValue("");
//    input_dialog->setOkButtonText("确定");
//    input_dialog->setCancelButtonText("取消");
//    connect(input_dialog, &QInputDialog::accepted, [=]() {
//        if (input_dialog->textValue().isEmpty()) {
//            QMessageBox::critical(this, "错误", "班级名称不能为空！");
//            return;
//        }
//        m_priReq = new NetworkRequests(POST, CloudAPIUrl::ADD_CLASS);
//        connect(m_priReq, &NetworkRequests::finished, [=](QJsonObject json, QString reply_string, QString error_string) {
//            if (!error_string.isEmpty())
//            {
//                QMessageBox::critical(this, "错误", "请求失败！");
//                
//                return;
//            }
//            if (json["code"].toInt() == 200) {
//                QMessageBox::information(this, "成功", "添加成功！");
//                getUserClasses();
//                showLog("added class!", INFO);
//            }
//            else {
//                QMessageBox::critical(this, "错误", json["msg"].toString());
//            }
//        });
//        QJsonObject data = {
//            {"name", input_dialog->textValue()}
//        };
//        QJsonObject headers = { {"token",Config["ClassTopLand.Cloud.UserToken"].toString()} };
//        m_priReq->start(data, headers);
//        
//
//
//    });
//    input_dialog->exec();
//}
