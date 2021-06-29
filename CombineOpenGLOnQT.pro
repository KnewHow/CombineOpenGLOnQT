QT       += core gui
QT       += opengl
QT       += openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/cpps/camera.cpp \
    src/cpps/main.cpp \
    src/cpps/widget.cpp

HEADERS += \
    src/includes/camera.h \
    src/includes/widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Resources/shader/light/fragment.glsl \
    Resources/shader/light/vertex.glsl
