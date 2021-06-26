#include "widget.h"

Widget::Widget(QWidget *parent)
: QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setAlphaBufferSize(24);
    format.setVersion(3, 3);
    format.setSamples(10);
    this->setFormat(format);
}

Widget::~Widget()
{
}

void Widget::initializeGL() {
    this->initializeOpenGLFunctions();
}

void Widget::resizeGL(int w, int h) {
    this->glViewport(0, 0, w, h);
}

void Widget::paintGL() {
    this->glClearColor(0.1f, 0.5f, 0.7f, 1.0f);
    this->glClear(GL_COLOR_BUFFER_BIT);
}

