#include "./mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QApplication::setQuitOnLastWindowClosed(false);
    this->setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    QTranslator translator;
    QLocale::Language lab = QLocale::system().language();
    if(QLocale::Chinese == lab)
    {
        translator.load(":/lang/lang_cn.qm");
        qApp->installTranslator(&translator);
        ui->retranslateUi(this);
    }else if(QLocale::English== lab){
        translator.load(":/language/lang_en.qm");
        qApp->installTranslator(&translator);
        ui->retranslateUi(this);
    }
    readTodos();
    connect(ui->min_up,&QToolButton::clicked,this,&MainWindow::on_time_changed);
    connect(ui->min_down,&QToolButton::clicked,this,&MainWindow::on_time_changed);
    connect(ui->sec_up,&QToolButton::clicked,this,&MainWindow::on_time_changed);
    connect(ui->sec_down,&QToolButton::clicked,this,&MainWindow::on_time_changed);
    connect(ui->timer_start,&QPushButton::clicked,this,&MainWindow::on_startTimer_Click);
    connect(ui->pushButton,&QPushButton::clicked,this,[=]
    {
        ui->time_label->setText("00:00");
    });
    startTimer(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_reText()
{
    ui->time_label->setText("00:00");
}
void MainWindow::timerEvent(QTimerEvent *event)
{
    bool timerStart;
    emit timerisStart(timerStart);
    if (timerStart)
    {
        int min,sec;
        emit getTimer(min,sec);
        ui->time_label->setText(QString("%1:%2").arg(min, 2, 10, QLatin1Char('0')).arg(sec, 2, 10, QLatin1Char('0')));
        ui->timer_start->setText("停止");
        ui->pushButton->setEnabled(false);
    }else{

        ui->timer_start->setText("开始");
        ui->pushButton->setEnabled(true);
    }

}
void MainWindow::showEvent(QShowEvent* event){
    smsManager = new QNetworkAccessManager(this);
    smsManager->setProxy(QNetworkProxy::NoProxy);
    QString url = QString("https://devapi.qweather.com/v7/weather/now?key=b7f4a7d1bfca4f13b6f265ea8676c297&location=%1").arg(Config["weather_localtion_id"].toString());
    Request = new QNetworkRequest(QUrl(url));
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    connect(smsManager , &QNetworkAccessManager::finished, this,[=](QNetworkReply *reply)
    {
        showLog(QString("Status Code:%1").arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()),INFO);
        if (reply->error())
        {
            showLog("Request Error" + reply->errorString(),ERR);
            return;
        }else
        {
            showLog(QString("Request ok, Reading..."),INFO);
            QByteArray data = reply->readAll();
            QJsonParseError error;
            QJsonDocument document = QJsonDocument::fromJson(data,&error);
            if(document.isNull())
            {
                showLog(QString("JSON Parse Error" + error.errorString()));
                return;
            }else
            {
                QJsonObject weather = document.object();
                qDebug()<<weather;
                ui->weather_icon_show->setStyleSheet(QString("image:url(:/res/qweather_icons/%1.svg)").arg(weather["now"].toObject()["icon"].toString()));
                ui->weather_show->setText(QString("天气：%1\n温度：%2  体感温度：%3").arg(weather["now"].toObject()["text"].toString()).arg(weather["now"].toObject()["temp"].toString()).arg(weather["now"].toObject()["feelsLike"].toString()));
            }

        }
    });
    initReply = smsManager ->get(*Request);
}

void MainWindow::on_butAdd_clicked()
{
    QInputDialog dialog{this, Qt::WindowCloseButtonHint};
    dialog.setWindowTitle(tr("新建待办"));
    dialog.setInputMode(QInputDialog::InputMode::TextInput);
    dialog.setTextEchoMode(QLineEdit::Normal);
    dialog.setLabelText(tr("请输入待办内容"));
    dialog.setOkButtonText(QObject::tr("确定"));
    dialog.setCancelButtonText(QObject::tr("取消"));
    dialog.setFixedSize(350,250);
    dialog.setTextValue("");
    // 设置了 QPushButtuon 的三态， QLineEdit 的样式， QLable 和 整体的背景色
    QString style = "QPushButton{background:#1191d3; color:rgb(255,255,255); border-radius:3px; min-height:30px; min-width:60px; font:13px \"Microsoft YaHei\";}"
                    "QPushButton:hover{background:#1191d0;}"
                    "QPushButton:pressed{background:rgb(32,75,148);}"
                    "QLineEdit{border:2px solid rgb(229,230,231);background:#ffffff;padding:4px;padding-left:10px;border-radius:3px;color:rgb(105,105,105);font:13px \"Microsoft YaHei\";}"
                    "QLineEdit:focus{border:2px solid #1191d3;}"
                    "QLineEdit:disabled{background-color:rgb(238,238,238);}"
                    "QLabel{color:rgb(85,85,85); background:#ffffff;font:12px \"Microsoft YaHei\"; font-weight:bold;}"
                    "QInputDialog{background-color:rgb(255,255,255); }";
    dialog.setStyleSheet(style);
    dialog.exec();
    if (!dialog.textValue().isEmpty()){
        newTodo(dialog.textValue(),true);
    }


}
void MainWindow::newTodo(QString name,bool n){
    QString todoText = name;
    QWidget* itemWidget = new QWidget;
    QHBoxLayout* hlayout = new QHBoxLayout;
    QCheckBox* okbox = new QCheckBox(itemWidget);
    todos.append(todoText);
    if (n){
        Config["todos"] = todos;
        QFile config_file(QDir::currentPath() + "/config.json");
        config_file.open(QFile::WriteOnly);
        QJsonDocument temp_doc;
        temp_doc.setObject(Config);
        config_file.write(temp_doc.toJson(QJsonDocument::Indented));
        config_file.close();
    }
    QLabel* textLabel = new QLabel(itemWidget);
    textLabel->setObjectName("textLabel");
    textLabel->setText(todoText);
    textLabel->setStyleSheet("color:#000000;");
    hlayout->addWidget(okbox);
    hlayout->addWidget(textLabel);
    hlayout->setStretch(0,1);
    hlayout->setStretch(1,100);
    itemWidget->setLayout(hlayout);
    connect(okbox,&QCheckBox::clicked,this,[=]{
        qDebug("Clicked");
        QListWidgetItem *item = ui->listWidget->currentItem();
        QWidget *widget = ui->listWidget->itemWidget(item);
        QLabel* curLabel = textLabel;
        QCheckBox* box = okbox;
        box->setDisabled(true);

        QJsonArray temparray;
        for (auto x:todos){

            if (x == textLabel->text()){
                qDebug("Continue");
                continue;
            }
            temparray.append(x);
        }
        curLabel->setText(QString("<s>%1</s>").arg(curLabel->text()));
        todos = temparray;

        Config["todos"] = temparray;
        QFile config_file(QDir::currentPath() + "/config.json");
        config_file.open(QFile::WriteOnly);
        QJsonDocument temp_doc;
        temp_doc.setObject(Config);
        config_file.write(temp_doc.toJson(QJsonDocument::Indented));
        config_file.close();

    });
    QListWidgetItem* item = new QListWidgetItem;

    ui->listWidget->addItem(item);

    ui->listWidget->setItemWidget(item,itemWidget);
    itemWidget->adjustSize();
    item->setSizeHint(itemWidget->size());
}
void MainWindow::readTodos(){
    QFileInfo fi(QDir::currentPath() + "/config.json");
    if (!fi.isFile()){
        QFile file(QDir::currentPath() + "/config.json");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }else{
        QFile file(QDir::currentPath() + "/config.json");
        file.open(QIODevice::ReadWrite | QIODevice::Text);

        QTextStream stream(&file);
        QString file_str = stream.readAll();
        file.close();
        QJsonParseError jsonError;
        QJsonDocument jsondoc = QJsonDocument::fromJson(file_str.toUtf8(),&jsonError);
        if (jsonError.error != QJsonParseError::NoError && !jsondoc.isNull()) {
            showLog("Config.json is Error!",LogStatus::ERR);
            return;
        }
        todos = jsondoc.object()["todos"].toArray();
        Config = jsondoc.object();
        ui->listWidget->clear();
        for (auto x : todos){
            newTodo(x.toString());
        }
    }
}

void MainWindow::on_btnhk_clicked()
{
    QStringList items;
    items << "星期一" << "星期二" << "星期三" << "星期四" << "星期五";
    QMap<QString,QString> en_cnday;
    en_cnday["星期一"] = "Mon";
    en_cnday["星期二"] = "Tue";
    en_cnday["星期三"] = "Wed";
    en_cnday["星期四"] = "Thu";
    en_cnday["星期五"] = "Fri";
    QFile file(QDir::currentPath() + "/tables.json");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream stream(&file);
    QString file_str = stream.readAll();
    file.close();
    QJsonParseError jsonError;
    QJsonDocument jsondoc = QJsonDocument::fromJson(file_str.toUtf8(),&jsonError);
    if (jsonError.error != QJsonParseError::NoError && !jsondoc.isNull()) {
        showLog("table.json is Error!",LogStatus::ERR);
        return;
    }
    QJsonObject appendixTables = jsondoc.object()["appendixTables"].toObject();
    for (auto iter = appendixTables.begin();iter != appendixTables.end();iter++){
        items << "附加课表：" + iter.key();
        en_cnday["附加课表：" + iter.key()] = "APPEND_" + iter.key();
    }
    bool ok;
    QString day = QInputDialog::getItem(this,"换课","选择使用的星期或附加课表",items,0,false,&ok);
    qDebug() << day;
    if (ok)
    {
        emit hk(en_cnday[day]);
    }

}

void MainWindow::on_time_changed()
{
    QObject *sender_button = sender();
    QStringList times = ui->time_label->text().split(":");
    QList<qint64> times_int;
    foreach (QString t, times)
    {
        times_int.append(t.toLongLong());
    }
    if (sender_button->objectName() == "min_up")
    {

        times_int[0]++;
    }
    else if (sender_button->objectName() == "min_down" && times_int[0] > 0)
    {

        times_int[0]--;
    }
    else if (sender_button->objectName() == "sec_up")
    {
        times_int[1]++;
    }
    else if (sender_button->objectName() == "sec_down" && times_int[1] > 0)
    {
        times_int[1]--;
    }
    ui->time_label->setText(QString("%1:%2").arg(times_int[0], 2, 10, QLatin1Char('0')).arg(times_int[1], 2, 10, QLatin1Char('0')));
}

void MainWindow::on_startTimer_Click()
{
    bool timerStart;
    emit timerisStart(timerStart);
    if (!timerStart)
    {
        emit startTm(ui->time_label->text());
        ui->timer_start->setText("停止");
        ui->pushButton->setEnabled(false);
    }else{
        emit stopTm();
        ui->timer_start->setText("开始");
        ui->pushButton->setEnabled(true);
    }

}