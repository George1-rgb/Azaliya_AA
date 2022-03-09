#ifndef OBJECTENGINE3D_H
#define OBJECTENGINE3D_H
#include <QVector>
#include "MaterialLibrary.h"
#include "SimpleObject3D.h"
#include "Transformational.h"
#include "Types.h"

class ObjectEngine3D : public Transformational
{
public:
    ObjectEngine3D();
    void loadObjectFromFile(const QString &path);
    void addObject(SimpleObject3D* obj);
    SimpleObject3D* getObject(quint32 index);
    void calculateTBN(QVector<VertexData> &vertData);
    virtual void rotate(const QQuaternion &r) override;
    virtual void translate(const QVector3D &t) override;
    virtual void scale(const float &s) override;
    virtual void setGlobalTransform(const QMatrix4x4 &g) override;
    virtual void drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;
private:
    QVector<SimpleObject3D*> objects;
    MaterialLibrary materialLibrary;

};

#endif // OBJECTENGINE3D_H
