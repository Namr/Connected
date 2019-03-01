#ifndef KEYLISTENER_H
#define KEYLISTENER_H

#include <qwidget.h>
#include "GLWidget.h"
class KeyListener : public QObject
{
    Q_OBJECT
public:
    KeyListener(GLWidget *GL);
    ~KeyListener();
protected:
    GLWidget *gl;
    bool eventFilter(QObject* obj, QEvent* event);
};



#endif // KEYLISTENER_H
