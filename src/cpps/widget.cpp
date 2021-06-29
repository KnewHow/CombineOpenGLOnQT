#include "../includes/widget.h"
#include <QSurfaceFormat>
#include <QtMath>
#include <QtDebug>


Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent),
      VBO(QOpenGLBuffer::VertexBuffer),
      texture1(QOpenGLTexture::Target2D),
      texture2(QOpenGLTexture::Target2D),
      camera(this)
{

    vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    cubePositions = {
         { 0.0f,  0.0f,  0.0f  },
         { 2.0f,  5.0f, -15.0f },
         {-1.5f, -2.2f, -2.5f  },
         {-3.8f, -2.0f, -12.3f },
         { 2.4f, -0.4f, -3.5f  },
         {-1.7f,  3.0f, -7.5f  },
         { 1.3f, -2.0f, -2.5f  },
         { 1.5f,  2.0f, -2.5f  },
         { 1.5f,  0.2f, -1.5f  },
         {-1.3f,  1.0f, -1.5f  }
    };

    timer.setInterval(18);
    connect(&timer,&QTimer::timeout,this,static_cast<void (Widget::*)()>(&Widget::update));
    timer.start();
}

Widget::~Widget(){
    texture1.destroy();
    texture2.destroy();
}

void Widget::initializeGL() {
    this->initializeOpenGLFunctions();
    QString objVerterShaderPath = "/home/knewhow/dev/QTWorkspace/CombineOpenGLOnQT/Resources/shader/object/vertex.glsl";
    QString objFragmentShaderPath = "/home/knewhow/dev/QTWorkspace/CombineOpenGLOnQT/Resources/shader/object/fragment.glsl";
    compileShader(shaderProgram, objVerterShaderPath, objFragmentShaderPath);

    QString lightVerterShaderPath = "/home/knewhow/dev/QTWorkspace/CombineOpenGLOnQT/Resources/shader/light/vertex.glsl";
    QString lightFragmentShaderPath = "/home/knewhow/dev/QTWorkspace/CombineOpenGLOnQT/Resources/shader/light/fragment.glsl";
    compileShader(lightShaderProgram, lightVerterShaderPath, lightFragmentShaderPath);


    QOpenGLVertexArrayObject::Binder{&VAO};

    VBO.create();
    VBO.bind();
    VBO.allocate(vertices.data(), sizeof(float) * vertices.size());

    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 6);
    shaderProgram.enableAttributeArray(0);
    shaderProgram.setAttributeBuffer(1, GL_FLOAT, sizeof(float) * 3, 3, sizeof(float) * 6);
    shaderProgram.enableAttributeArray(1);

    lightShaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 6);
    lightShaderProgram.enableAttributeArray(0);

    this->glEnable(GL_DEPTH_TEST);

    camera.init();
}

void Widget::resizeGL(int w, int h) {
    this->glViewport(0, 0, w, h);
}

void Widget::paintGL() {
    this->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();
    float millSeconds = QTime::currentTime().msecsSinceStartOfDay() / 1000.0f;
    QOpenGLVertexArrayObject::Binder{&VAO};

    QMatrix4x4 model;
    shaderProgram.setUniformValue("model", model);

    QMatrix4x4 projection;
    projection.perspective(45.0, width()/(float)height(), 0.1, 100);
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("view", camera.getView());

    QVector3D lightColor(1.0f,1.0f,1.0f);
    QVector3D objectColor(1.0f,0.5f,0.31f);
    QVector3D lightPos(-4.0f,0.0f,0.5f);

    shaderProgram.setUniformValue("viewerPos", camera.getPosition());
    shaderProgram.setUniformValue("lightPos", lightPos);
    shaderProgram.setUniformValue("objectColor", objectColor);
    shaderProgram.setUniformValue("lightColor", lightColor);
    this->glDrawArrays(GL_TRIANGLES, 0, 36);

    lightShaderProgram.bind();
    model.translate(lightPos);
    model.scale(0.2);
    lightShaderProgram.setUniformValue("model", model);
    lightShaderProgram.setUniformValue("projection", projection);
    lightShaderProgram.setUniformValue("view", camera.getView());
    this->glDrawArrays(GL_TRIANGLES, 0, 36);
}

bool Widget::event(QEvent *e) {
    camera.handle(e);
    return QWidget::event(e);
}

void Widget::compileShader(QOpenGLShaderProgram& program, const QString &vertexShaderPath, const QString &fragmentShaderPath) {
    if(!program.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath)) {
        qDebug() << "Compile vertex shader Error: " << shaderProgram.log();
    }

    if(!program.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath)) {
        qDebug() << "Compile fragment shader Error: " << shaderProgram.log();
    }

    if(!program.link()) {
        qDebug() << "Link shader Error: " << shaderProgram.log();
    }
}

