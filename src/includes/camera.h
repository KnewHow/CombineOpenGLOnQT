#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include <QVector3D>
#include <QSet>
#include <QMatrix4x4>
#include <QEvent>

class Camera {
public:
    Camera(QWidget* widget);

    float getMoveSpeed() const;
    void setMoveSpeed(const float speed);

    float getSensitivity() const;
    void setSensitivity(const float sensitivity);

    float getYaw() const;
    void setYaw(const float yaw);

    float getPitch() const;
    void setPitch(const float pitch);

    QVector3D getPosition() const;
    void setPosition(const QVector3D& position);

    QMatrix4x4 getView() const;

    void init();

    void handle(QEvent* event);

private:
    QWidget *widget;
    float yaw;
    float pitch;
    float sensitivity;

    QVector3D position;
    QVector3D direction;
    QVector3D right;
    QVector3D up;

    float moveSpeed;
    QSet<int> keys; //record press keys

    int timeId;
    float deltaTime; // delta time between two frame
    float lastFrameTime; // the last frame time, compared to current frame to calculate delta time

    QMatrix4x4 view;

};

#endif // CAMERA_H
