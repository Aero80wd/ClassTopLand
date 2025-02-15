#include "./toolbox.h"
#include "ui_toolbox.h"
ToolBox::ToolBox(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ToolBox)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowType::FramelessWindowHint | Qt::WindowType::Tool | Qt::WindowType::WindowStaysOnBottomHint);
    setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
    LoadPlugins();
    setWidgetBlur(this,0x11000000);
    this->setAcceptDrops(true);
    this->installEventFilter(this);

}

ToolBox::~ToolBox()
{
    delete ui;
}

void ToolBox::LoadPlugins(){
    for (auto x:findChildren<ClickLabel*>()){
        delete x;
    }
    QDir plugin_dir(QDir::currentPath() + "/plugins");
    if (!plugin_dir.exists()){
        plugin_dir.mkdir(QDir::currentPath() + "/plugins");
        return;
    }
    QFileInfoList fileList = plugin_dir.entryInfoList();
    ClickLabel *PluginBut = new ClickLabel(this);
    ui->verticalLayout->addWidget(PluginBut);
    connect(PluginBut,&ClickLabel::clicked,this,[=]
    {
        QFile file(QDir::currentPath() + "/config.json");
        file.open(QIODevice::ReadWrite | QIODevice::Text);

        QTextStream stream(&file);
        QString file_str = stream.readAll();
        file.close();
        QJsonParseError jsonError;
        QJsonDocument jsondoc = QJsonDocument::fromJson(file_str.toUtf8(),&jsonError);
        if (jsonError.error != QJsonParseError::NoError && !jsondoc.isNull()) {
            showLog("Config.json is Error!",LogStatus::ERR);
            return true;
        }
        bool zuanyanisopen = false;
        if (jsondoc.object().contains("zuan_status"))
        {
            zuanyanisopen = jsondoc.object().value("zuan_status").toBool();
        }
        QJsonObject Config = jsondoc.object();
        yiyanDialog *yiyan = new yiyanDialog(this);
        yiyan->setConfig(Config);
        yiyan->setZuanYanOpen(zuanyanisopen);
        QScreen *scr = qApp->primaryScreen();
        int scr_w = scr->size().width();
        int scr_h = scr->size().height();
        yiyan->move((scr_w - yiyan->width()) / 2, (scr_h - yiyan->height()) / 3);
        yiyan->setModal(false);
        yiyan->show();
    });
    PluginBut->setMinimumSize(QSize(50,50));
    PluginBut->setAniOpen(true);
    PluginBut->setCursor(Qt::PointingHandCursor);
    PluginBut->setStyleSheet("border-image: url(:/res/speak.png);");
    adjustSize();
    for (auto x : fileList){
        QString JsonPath = x.filePath() + "/pluginConfig.json";
        if (x.filePath()[x.filePath().size() -1] == '.'){
            continue;
        }
        QFile file(x.filePath() + "/pluginConfig.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString value = file.readAll();
        file.close();

        QJsonParseError parseJsonErr;
        QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);
        QJsonObject PluginObject = document.object();
        ClickLabel *PluginBut = new ClickLabel(this);
        ui->verticalLayout->addWidget(PluginBut);

        PluginBut->setMinimumSize(QSize(50,50));
        PluginBut->setAniOpen(true);
        PluginBut->setCursor(Qt::PointingHandCursor);
        adjustSize();

        PluginBut->setStyleSheet(QString("image: url(\"%1\");").arg(x.filePath() + "/" + PluginObject["icon"].toString()));
        if (PluginObject["type"].toString() == "win32"){

            connect(PluginBut,&ClickLabel::clicked,this,[=]{
                ShellExecute(NULL,L"open",QString("%1/%2").arg(x.filePath()).arg(PluginObject["execPath"].toString()).toStdWString().c_str(),NULL,NULL,SW_SHOW);
            });
        }
        if (PluginObject["type"].toString() == "url"){

            connect(PluginBut,&ClickLabel::clicked,this,[=]{
                PluginWebEngine* web_window = new PluginWebEngine();
                web_window->setWindowTitle(PluginObject["name"].toString());
                web_window->setUrl(QUrl(PluginObject["url"].toString()));
                
                web_window->page()->profile()->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/127.0.0.0 Safari/537.36 Edg/127.0.0.0");
                web_window->show();
            });
        }
        if (PluginObject["type"].toString() == "html"){

            connect(PluginBut,&ClickLabel::clicked,this,[=]{
                PluginWebEngine *web_window = new PluginWebEngine();
                web_window->setWindowTitle(PluginObject["name"].toString());
                web_window->setUrl(QUrl(x.filePath() + "/" + PluginObject["index"].toString()));
           
                web_window->page()->profile()->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/127.0.0.0 Safari/537.36 Edg/127.0.0.0");
                web_window->show();
            });
        }
        if (PluginObject["type"].toString() == "link"){

            connect(PluginBut,&ClickLabel::clicked,this,[=]{
                QFileInfo info(PluginObject["linkPath"].toString());
                if (info.exists() && info.isSymLink()){
                    ShellExecute(NULL,L"open",info.symLinkTarget().toStdWString().c_str(),NULL,NULL,SW_SHOW);
                }
            });
        }

    }
}
