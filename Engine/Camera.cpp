#include "Camera.h"
#include <QOpenGLShaderProgram>
#include "Models/Material.h"

Camera::Camera() : Scale(1.0f)
{
    GlobalTransform.setToIdentity();
}

void Camera::drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if (functions != 0) return;
    program->setUniformValue("u_viewMatrix", viewMatrix);
}

void Camera::rotate(const QQuaternion &r)
{
    Rotation = r * Rotation;

    viewMatrix.setToIdentity();
    viewMatrix.translate(Translation);

    viewMatrix.rotate(Rotation);
    viewMatrix.scale(Scale);
    //GlobalTransform.rotate(Rotation);
    viewMatrix = viewMatrix * GlobalTransform.inverted();
}

void Camera::translate(const QVector3D &t)
{
    Translation += t;

    viewMatrix.setToIdentity();
    viewMatrix.translate(Translation);
    viewMatrix.rotate(Rotation);
    viewMatrix.scale(Scale);
    viewMatrix = viewMatrix * GlobalTransform.inverted();
}

void Camera::scale(const float &s)
{
    Scale *= s;

    viewMatrix.setToIdentity();
    viewMatrix.translate(Translation);
    viewMatrix.rotate(Rotation);
    viewMatrix.scale(Scale);
    viewMatrix = viewMatrix * GlobalTransform.inverted();
}

void Camera::setGlobalTransform(const QMatrix4x4 &g)
{
    GlobalTransform = g;

    viewMatrix.setToIdentity();
    viewMatrix.translate(Translation);
    viewMatrix.rotate(Rotation);
    viewMatrix.scale(Scale);
    viewMatrix = viewMatrix * GlobalTransform.inverted();
}
