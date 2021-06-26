#include "../includes/widget.h"
#include <QSurfaceFormat>


Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent),
      VBO(QOpenGLBuffer::VertexBuffer),
      IBO(QOpenGLBuffer::IndexBuffer)
{

//    QSurfaceFormat format;
//    format.setAlphaBufferSize(24);
//    format.setMajorVersion(4);
//    format.setMinorVersion(5);
//    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile);
//    format.setSamples(10);
//    this->setFormat(format);

    vertices = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
       -0.5f, -0.5f, 0.0f, // 左下角
       -0.5f, 0.5f, 0.0f   // 左上角
    };

    indices = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
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

    IBO.create();
    IBO.bind();
    IBO.allocate(indices.data(), sizeof(unsigned int) * indices.size());

    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
    shaderProgram.enableAttributeArray(0);
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
        this->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

}

