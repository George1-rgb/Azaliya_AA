#ifndef SKYBOX_H
#define SKYBOX_H

#include "Transformational.h"

class SimpleObject3D;
class QImage;

class SkyBox : public Transformational
{
public:
    SkyBox(float width, const QImage &texture);
    ~SkyBox();
    virtual void drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;
    virtual void rotate(const QQuaternion &r) override;
    virtual void translate(const QVector3D &t) override;
    virtual void scale(const float &s) override;
    virtual void setGlobalTransform(const QMatrix4x4 &g) override;

private:
    SimpleObject3D* cube;
};

#endif // SKYBOX_H
