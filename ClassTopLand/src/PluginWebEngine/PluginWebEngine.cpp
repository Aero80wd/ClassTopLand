#include "./PluginWebEngine.h"
PluginWebEngine::PluginWebEngine(QWidget* parent) : QWebEngineView(parent)
{
}
QWebEngineView* PluginWebEngine::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type);
    return this;
}