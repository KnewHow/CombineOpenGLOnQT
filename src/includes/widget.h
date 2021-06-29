#ifndef WIDGET_H
#define WIDGET_H

#include "camera.h"

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QTime>
#include <QTimer>


class Widget : public QOpenGLWidget,public QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w,int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent* e) override;

private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLShaderProgram lightShaderProgram;
    QVector<float> vertices;
    QVector<unsigned int> indices;
    QVector<QVector3D> cubePositions;
    QOpenGLBuffer VBO;
    QOpenGLVertexArrayObject VAO;
    QOpenGLTexture texture1;
    QOpenGLTexture texture2;
    QTimer timer;

    Camera camera;

    void compileShader(QOpenGLShaderProgram& program, const QString& vertexShaderPath, const QString& fragmentShaderPath);

};

#endif // WIDGET_H
