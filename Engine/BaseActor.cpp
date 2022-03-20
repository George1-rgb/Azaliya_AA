#include "BaseActor.h"
#include "Engine/Group.h"
#include "Models/ObjectEngine3D.h"
#include "Engine/Camera.h"
#include <QOpenGLShaderProgram>
BaseActor::BaseActor()
{
    ActorGroup = new Group;
    ActorCamera = new Camera;
    ActorCamera->translate(QVector3D(0.0f, 0.0f, -20.0f));
    ActorObject = new ObjectEngine3D;
    ActorObject->loadObjectFromFile(":/Sourse/Models/defaultMaterial.obj");
    ActorGroup->addObject(ActorCamera);
    ActorGroup->addObject(ActorObject);
}

BaseActor::~BaseActor()
{
    delete ActorGroup;
}

void BaseActor::rotate(const QQuaternion &r)
{
    ActorGroup->rotate(r);
}

void BaseActor::translate(const QVector3D &t)
{
    ActorGroup->translate(t);
}

void BaseActor::scale(const float &s)
{
    ActorGroup->scale(s);
}

void BaseActor::setGlobalTransform(const QMatrix4x4 &g)
{
    ActorGroup->setGlobalTransform(g);
}

void BaseActor::drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    ActorGroup->drawModel(program, functions);
}

