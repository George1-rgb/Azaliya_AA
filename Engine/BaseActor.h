#ifndef BASEACTOR_H
#define BASEACTOR_H
#include "Engine/Transformational.h"
class Camera;
class ObjectEngine3D;
class Group;
class QOpenGLShaderProgram;
class BaseActor : public Transformational
{
public:
    BaseActor();
    ~BaseActor();
    virtual void rotate(const QQuaternion &r);
    virtual void translate(const QVector3D &t);
    virtual void scale(const float &s);
    virtual void setGlobalTransform(const QMatrix4x4 &g);
    virtual void drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
private:
    Group *ActorGroup;
    Camera * ActorCamera;
    ObjectEngine3D *ActorObject;
};

#endif // BASEACTOR_H
