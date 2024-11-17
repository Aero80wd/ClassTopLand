#include "../headers/clicklabel.h"

ClickLabel::ClickLabel(QWidget* parent) {

}
void ClickLabel::mouseClickEvent(QMouseEvent* e){
    setWindowFlags(Qt::Widget);
    emit clicked();
    if (aniopen)
    {
        jump_animation->start();
    }
}
void ClickLabel::mouseDoubleClickEvent(QMouseEvent* e){
    setWindowFlags(Qt::Widget);
    emit DoubleClicked();
}
bool ClickLabel::event(QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
        if(mouseEvent->button() == Qt::LeftButton)
        {
            emit clicked();
            return true;
        }
    }
    if (e->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
        if(mouseEvent->button() == Qt::LeftButton)
        {
            emit DoubleClicked();
            return true;
        }
    }
    if (e->type() == QEvent::Enter and aniopen){
        resize_animation->setDirection(QAbstractAnimation::Forward);
        resize_animation->start();
        return true;
    }
    if (e->type() == QEvent::Leave and aniopen){
        resize_animation->setDirection(QAbstractAnimation::Backward);
        resize_animation->start();
        return true;
    }
    if (e->type() == QEvent::Resize and !anisettinged){
        resize_animation = new QPropertyAnimation(this,"geometry");
        resize_animation->setStartValue(QRect(pos().x(),pos().y(),width(),height()));
        resize_animation->setEndValue(QRect(pos().x()-((width()*1.2-width())/2),pos().y()-((height()*1.2-height())/2),width()*1.2,height()*1.2));
        resize_animation->setDuration(200);
        resize_animation->setEasingCurve(QEasingCurve::InOutQuad);
        jump_animation = new QPropertyAnimation(this,"pos");
        jump_animation->setKeyValueAt(0,pos());
        jump_animation->setKeyValueAt(0.5,QPoint(x(),y()+30));
        jump_animation->setKeyValueAt(1,pos());
        jump_animation->setDuration(500);
        jump_animation->setEasingCurve(QEasingCurve::InOutQuad);
        anisettinged = true;
        return true;
    }
    return QLabel::event(e);
}
