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
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 5);
    shaderProgram.enableAttributeArray(0);
    shaderProgram.setAttributeBuffer(1, GL_FLOAT, sizeof(float) * 3, 2, sizeof(float) * 5);
    shaderProgram.enableAttributeArray(1);

    texture1.create();
    texture1.setData(QImage("/home/knewhow/dev/QTWorkspace/CombineOpenGLOnQT/Resources/texture/openglLogo.jpg").mirrored());
    texture1.setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    texture1.setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    texture1.setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);

    texture2.create();
    texture2.setData(QImage("/home/knewhow/dev/QTWorkspace/CombineOpenGLOnQT/Resources/texture/sea.jpg").mirrored());
    texture2.setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    texture2.setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    texture2.setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);

    this->glEnable(GL_DEPTH_TEST);

    camera.init();
}

void Widget::resizeGL(int w, int h) {
    this->glViewport(0, 0, w, h);
}

void Widget::paintGL() {
    this->glClearColor(0.1f, 0.5f, 0.7f, 1.0f);
    this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram.bind();
    {
        float millSeconds = QTime::currentTime().msecsSinceStartOfDay() / 1000.0f;
        QOpenGLVertexArrayObject::Binder{&VAO};
        texture1.bind(0);
        shaderProgram.setUniformValue("texture_color1", 0);
        texture2.bind(1);
        shaderProgram.setUniformValue("texture_color2", 1);

        shaderProgram.setUniformValue("view", camera.getView());

        QMatrix4x4 projection;
        projection.perspective(45.0, width()/(float)height(), 0.1, 100);
        shaderProgram.setUniformValue("projection", projection);

        for(int i = 0; i < cubePositions.size(); i++) {
            QMatrix4x4 model;
            model.translate(cubePositions[i]);
            model.rotate(180*millSeconds + i * 20, QVector3D(1.0, 0.5, 0.3));
            shaderProgram.setUniformValue("model", model);
            this->glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        qInfo() << "millSeconds: " << millSeconds;
    }
}

bool Widget::event(QEvent *e) {
    camera.handle(e);
    return QWidget::event(e);
}

