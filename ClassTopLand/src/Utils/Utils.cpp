#include "Utils.h"
void loadStyleSheet(QWidget *widget, const QString &styleSheetFile)
{
    QFile file(styleSheetFile);
    if (file.open(QFile::ReadOnly))
    {
        QString qss = QString(file.readAll());
        widget->setStyleSheet(qss);
        file.close();
    }
}
QString getStyleSheet(const QString &styleSheetFile)
{
    QFile file(styleSheetFile);
    if (file.open(QFile::ReadOnly))
    {
        QString qss = QString(file.readAll());
        file.close();
        return qss;
    }
    return QString();
}
enum AccentState {
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,
    ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
    ACCENT_INVALID_STATE = 5
};

struct AccentPolicy {
    AccentState AccentState;
    int AccentFlags;
    int GradientColor;
    int AnimationId;
};
struct WindowCompositionAttributeData {
    int Attribute;
    PVOID Data;
    ULONG DataSize;
};
typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WindowCompositionAttributeData*);
pSetWindowCompositionAttribute SetWindowCompositionAttribute = nullptr;
void setWidgetBlur(QWidget* widget,int alpha) {
    if (getWindowsVersion() == Windows10p) {
        if (!SetWindowCompositionAttribute) {
            HMODULE hModule = LoadLibrary(TEXT("user32.dll"));
            if (hModule) {
                SetWindowCompositionAttribute =
                    (pSetWindowCompositionAttribute)GetProcAddress(hModule, "SetWindowCompositionAttribute");
            }
        }
        HWND hWnd = HWND(widget->winId());
        AccentPolicy policy = { ACCENT_ENABLE_ACRYLICBLURBEHIND, 0, int(0x00FFFFFF + alpha), 0 };
        WindowCompositionAttributeData data = { 19, &policy, sizeof(AccentPolicy) };
        SetWindowCompositionAttribute(hWnd, &data);
    }
    else {
        widget->setStyleSheet("background: rgba(255,255,0.7)");
    }
    
}
SystemType getWindowsVersion() {
    double major = QSysInfo::productVersion().toDouble();
    if (major >= 10.0) {
        return Windows10p;
    }
    else {
        return Windows10d;
    }

}