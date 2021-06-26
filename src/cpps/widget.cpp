#include "../includes/widget.h"
#include <QSurfaceFormat>


Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent),
      VBO(QOpenGLBuffer::VertexBuffer)
{

//    QSurfaceFormat format;
//    format.setAlphaBufferSize(24);
//    format.setMajorVersion(4);
//    format.setMinorVersion(5);
//    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile);
//    format.setSamples(10);
//    this->setFormat(format);

    vertices = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

}

Widget::~Widget(){}

void Widget::initializeGL() {
    this->initializeOpenGLFunctions();
    QString verterShaderPath = "/home/knewhow/dev/QTWorkspace/CombineOpenGLOnQT/Resources/shader/vertex.glsl";
    QString fragmentShaderPath = "/home/knewhow/dev/QTWorkspace/CombineOpenGLOnQT/Resources/shader/fragment.glsl";
    if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, verterShaderPath)) {
        qDebug() << "Compile vertex shader Error: " << shaderProgram.log();
    }

    if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath)) {
        qDebug() << "Compile fragment shader Error: " << shaderProgram.log();
    }

    if(!shaderProgram.link()) {
        qDebug() << "Link shader Error: " << shaderProgram.log();
    }

   QOpenGLVertexArrayObject::Binder{&VAO};

    VBO.create();
    VBO.bind();
    VBO.allocate(vertices.data(), sizeof(float) * vertices.size());

    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
    shaderProgram.enableAttributeArray(0);

    VAO.release();
}

void Widget::resizeGL(int w, int h) {
    this->glViewport(0, 0, w, h);
}

void Widget::paintGL() {
    this->glClearColor(0.1f, 0.5f, 0.7f, 1.0f);
    this->glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram.bind();
    {
        QOpenGLVertexArrayObject::Binder{&VAO};
        this->glDrawArrays(GL_TRIANGLES, 0, 3);
    }

}

