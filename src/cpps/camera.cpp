#include "../includes/camera.h"

#include <QtMath>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTime>

Camera::Camera(QWidget* widget)
    :widget(widget),
      yaw(0),
      pitch(0),
      sensitivity(0.005),
      position(-0.5, 0.0, 0.0),
      direction(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch)),
      right(QVector3D::crossProduct({0.0, 1.0, 0.0}, direction)),
      up(QVector3D::crossProduct(direction, right)),
      moveSpeed(0.5),
      timeId(0)
{}


float Camera::getMoveSpeed() const {
    return  moveSpeed;
}

void Camera::setMoveSpeed(const float speed) {
    this->moveSpeed = speed;
}

float Camera::getSensitivity() const {
    return sensitivity;
}

void Camera::setSensitivity(const float sensitivity) {
    this->sensitivity = sensitivity;
}

float Camera::getYaw() const {
    return yaw;
}

void Camera::setYaw(const float yaw) {
    this->yaw = yaw;
}

float Camera::getPitch() const {
    return  pitch;
}

void Camera::setPitch(const float pitch) {
    this->pitch = pitch;
}

QVector3D Camera::getPosition() const {
    return position;
}

void Camera::setPosition(const QVector3D &position) {
    this->position = position;
}

QMatrix4x4 Camera::getView() const {
    return view;
}

void Camera::handle(QEvent *event) {
    if(event->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        float xOffset = mouseEvent->position().x() - widget->rect().center().x();
        float yOffset = widget->rect().center().y() - mouseEvent->position().y(); //注意这里是相反的，因为y坐标是从底部往顶部依次增大的
        xOffset *= sensitivity;
        yOffset *= sensitivity;
        yaw += xOffset;
        pitch += yOffset;
        if(pitch > 1.55) { //将俯视角限制到[-89°,89°]，89°约等于1.55
            pitch = 1.55;
        }
        if(pitch < -1.55) {
            pitch = -1.55;
        }
        direction.setX(cos(yaw) * cos(pitch));
        direction.setY(sin(pitch));
        direction.setZ(sin(yaw) * cos(pitch));
        view.setToIdentity();
        view.lookAt(position, position + direction, up);
        QCursor::setPos(widget->geometry().center()); // move mouse to window center
    } else if(event->type() == QEvent::Timer) {
        float speed = moveSpeed * deltaTime;
        if(keys.contains(Qt::Key_W)) {
            position += direction * speed;
        }
        if(keys.contains(Qt::Key_S)) {
            position -= direction * speed;
        }

        if(keys.contains(Qt::Key_A)) {
            position -= QVector3D::crossProduct(position, up).normalized() * speed;
        }
        if(keys.contains(Qt::Key_D)) {
            position += QVector3D::crossProduct(position, up).normalized() * speed;
        }

        if(keys.contains(Qt::Key_Space)) {
            position.setY(position.y() + speed);
        }
        if(keys.contains(Qt::Key_Shift)) {
            position.setY(position.y() - speed);
        }

        view.setToIdentity();
        view.lookAt(position, position + direction, up);
    } else if(event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent =static_cast<QKeyEvent*>(event);
        keys.insert(keyEvent->key()); // add pressed key
        if(!keyEvent->isAutoRepeat() && timeId == 0) {// judge long press
            timeId = widget->startTimer(1);
        }

    } else if(event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent =static_cast<QKeyEvent*>(event);
        keys.remove(keyEvent->key()); // add pressed key
        if(!keyEvent->isAutoRepeat() && timeId != 0 && keys.empty()) {// judge long press
            widget->killTimer(timeId);
            timeId = 0;
        }
    } else if(event->type() == QEvent::UpdateRequest) {
        float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0f;
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;
    } else if(event->type() == QEvent::FocusIn) {
        widget->setCursor(Qt::BlankCursor);             //隐藏鼠标光标
        QCursor::setPos(widget->geometry().center());   //将鼠标移动窗口中央
        widget->setMouseTracking(true);                 //开启鼠标追踪
    } else if(event->type() == QEvent::FocusOut) {
        widget->setCursor(Qt::ArrowCursor);   //恢复鼠标光标
        widget->setMouseTracking(false);      //关闭鼠标追踪
    }
}

void Camera::init() {
    view.lookAt(position, position + direction, up);
    widget->activateWindow();
    widget->setFocus();
}




