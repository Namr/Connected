#include "keylistener.h"

KeyListener::KeyListener(GLWidget *GL)
{
    gl = GL;
}


KeyListener::~KeyListener()
{
}

bool KeyListener::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_A)
        {
            gl->leftKeyDown = 1;
        }
        else if (key->key() == Qt::Key_D)
        {
            gl->rightKeyDown = 1;
        }
        if (key->key() == Qt::Key_W)
        {
            gl->upKeyDown = 1;
        }
        else if (key->key() == Qt::Key_S)
        {
            gl->downKeyDown = 1;
        }
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_A)
        {
            gl->leftKeyDown = 0;
        }
        else if (key->key() == Qt::Key_D)
        {
            gl->rightKeyDown = 0;
        }
        if (key->key() == Qt::Key_W)
        {
            gl->upKeyDown = 0;
        }
        else if (key->key() == Qt::Key_S)
        {
            gl->downKeyDown = 0;
        }
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* key = static_cast<QMouseEvent*>(event);
        if (key->button() == Qt::MouseButton::RightButton)
        {
            gl->rightMouseDown = 1;
        }
        if(key->button() == Qt::MouseButton::LeftButton)
        {
            gl->leftMouseDown = 1;
        }
        if (key->button() == Qt::MouseButton::MiddleButton)
        {
            gl->flipView();
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* key = static_cast<QMouseEvent*>(event);
        if (key->button() == Qt::MouseButton::RightButton)
        {
            gl->rightMouseDown = 0;
        }
        if(key->button() == Qt::MouseButton::LeftButton)
        {
            gl->leftMouseDown = 0;
        }
    }
    else
    {
        return QObject::eventFilter(obj, event);
    }
    return false;
}
