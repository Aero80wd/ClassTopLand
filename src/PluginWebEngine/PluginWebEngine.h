#ifndef PLUGINWE_H
#define PLUGINWE_H
#include <QWidget>
#include <QWebEngineView>
class PluginWebEngine : public QWebEngineView
{
    Q_OBJECT
public:
    explicit PluginWebEngine(QWidget* parent = nullptr);
protected:
    /**
     * @brief createWindow 在鼠标左键点击的时候会触发这个方法
     * @param type
     * @return
     */
    QWebEngineView* createWindow(QWebEnginePage::WebWindowType type);
};
#endif // SWEBENGINEVIEW_H