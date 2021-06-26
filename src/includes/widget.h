#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

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

private:
    QOpenGLShaderProgram shaderProgram;
    QVector<float> vertices;
    QVector<unsigned int> indices;
    QOpenGLBuffer VBO;
    QOpenGLVertexArrayObject VAO;
};

#endif // WIDGET_H
