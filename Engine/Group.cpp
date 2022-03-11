#include "Group.h"

Group::Group() : Scale(1.0f)
{

}

void Group::drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    for (int i = 0; i < Objects.size(); ++i)
        Objects[i]->drawModel(program, functions);
}

void Group::rotate(const QQuaternion &r)
{
    Rotation = r * Rotation;

    QMatrix4x4 localModelMatrix;
    localModelMatrix.setToIdentity();
    localModelMatrix.translate(Translation);
    localModelMatrix.rotate(Rotation);
    localModelMatrix.scale(Scale);
    localModelMatrix = GlobalTransform * localModelMatrix;

    for (int i = 0; i < Objects.size(); ++i)
        Objects[i]->setGlobalTransform(localModelMatrix);
}

    void Group::translate(const QVector3D &t)
    {
        Translation += t;

        QMatrix4x4 localModelMatrix;
        localModelMatrix.setToIdentity();
        localModelMatrix.translate(Translation);
        localModelMatrix.rotate(Rotation);
        localModelMatrix.scale(Scale);
        localModelMatrix = GlobalTransform * localModelMatrix;

        for (int i = 0; i < Objects.size(); ++i)
            Objects[i]->setGlobalTransform(localModelMatrix);
    }

    void Group::scale(const float &s)
    {
        Scale *= s;

        QMatrix4x4 localModelMatrix;
        localModelMatrix.setToIdentity();
        localModelMatrix.translate(Translation);
        localModelMatrix.rotate(Rotation);
        localModelMatrix.scale(Scale);
        localModelMatrix = GlobalTransform * localModelMatrix;

        for (int i = 0; i < Objects.size(); ++i)
            Objects[i]->setGlobalTransform(localModelMatrix);
    }

    void Group::setGlobalTransform(const QMatrix4x4 &g)
    {
        GlobalTransform = g;

        QMatrix4x4 localModelMatrix;
        localModelMatrix.setToIdentity();
        localModelMatrix.translate(Translation);
        localModelMatrix.rotate(Rotation);
        localModelMatrix.scale(Scale);
        localModelMatrix = GlobalTransform * localModelMatrix;

        for (int i = 0; i < Objects.size(); ++i)
            Objects[i]->setGlobalTransform(localModelMatrix);
    }

    void Group::addObject(Transformational* obj)
    {
        Objects.append(obj);

        QMatrix4x4 localModelMatrix;
        localModelMatrix.setToIdentity();
        localModelMatrix.translate(Translation);
        localModelMatrix.rotate(Rotation);
        localModelMatrix.scale(Scale);
        localModelMatrix = GlobalTransform * localModelMatrix;
        Objects[Objects.size() - 1]->setGlobalTransform(localModelMatrix);
    }

    void Group::delObject(Transformational *obj)
    {
        Objects.removeAll(obj);
    }

    void Group::delObject(const int &index)
    {
        Objects.remove(index);
    }
