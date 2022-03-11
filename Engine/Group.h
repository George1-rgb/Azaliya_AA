#ifndef GROUP_H
#define GROUP_H

#include "Transformational.h"
#include <QQuaternion>
#include <QVector3D>
#include <QVector>
#include <QMatrix4x4>

class Group : public Transformational
{
public:
    Group();

    virtual void drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;
    virtual void rotate(const QQuaternion &r) override;
    virtual void translate(const QVector3D &t) override;
    virtual void scale(const float &s) override;
    virtual void setGlobalTransform(const QMatrix4x4 &g) override;

    void addObject(Transformational *obj);
    void delObject(Transformational *obj);
    void delObject(const int &index);
private:
    QQuaternion Rotation;
    QVector3D Translation;
    float Scale;
    QMatrix4x4 GlobalTransform;

    QVector<Transformational*> Objects;
};

#endif // GROUP_H
