#-------------------------------------------------
#
# Project created by QtCreator 2019-02-28T14:34:03
#
#-------------------------------------------------

QT       += core opengl gui widgets openglextensions

TARGET = Connected
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        connected.cpp \
    GLWidget.cpp \
    brain.cpp \
    camera.cpp \
    keylistener.cpp \
    mri.cpp \
    model.cpp \
    nifti.cpp \
    colorsettings.cpp \
    networksettings.cpp

HEADERS += \
        connected.h \
    GLWidget.h \
    brain.h \
    camera.h \
    keylistener.h \
    mri.h \
    model.h \
    nifti.h \
    tiny_obj_loader.h \
    colorsettings.h \
    networksettings.h

FORMS += \
        connected.ui \
    colorsettings.ui \
    networksettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -ltiff -lGL

win32:INCLUDEPATH += "C:/Program Files (x86)/glm/include/" \
                     "C:/Program Files/boost/boost_1_67_0/" \
                     "C:/Program Files (x86)/tiff/include"

win32: LIBS += "-LC:/Program Files (x86)/tiff/lib/" -ltiff
