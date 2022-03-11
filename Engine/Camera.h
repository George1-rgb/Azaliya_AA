#ifndef CAMERA_H
#define CAMERA_H
#include "Transformational.h"
#include <QQuaternion>
#include <QVector3D>
#include <QMatrix4x4>

class Camera : public Transformational
{
public:
    Camera();
    virtual void drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions = 0) override;
    virtual void rotate(const QQuaternion &r) override;
    virtual void translate(const QVector3D &t) override;
    virtual void scale(const float &s) override;
    virtual void setGlobalTransform(const QMatrix4x4 &g) override;

private:
    QQuaternion Rotation;
    QVector3D Translation;
    float Scale;
    QMatrix4x4 GlobalTransform;
    QMatrix4x4 viewMatrix;
};

#endif // CAMERA_H
