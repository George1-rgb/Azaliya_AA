#ifndef SIMPLEOBJECT3D_H
#define SIMPLEOBJECT3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include "Engine/Types.h"
#include "Engine/Transformational.h"

class QOpenGLShaderProgram;
class QOpenGLFunctions;
class QOpenGLTexture;
class Material;

class SimpleObject3D : public Transformational
{
public:
    SimpleObject3D();
    SimpleObject3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, Material* _material);
    ~SimpleObject3D();
    void initModel(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, Material* _material);
    void drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);
private:
    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer indexBuffer;
    QOpenGLTexture* diffuseMap;
    QOpenGLTexture* normalMap;

    QQuaternion Rotation;
    QVector3D Translation;
    float Scale;
    QMatrix4x4 GlobalTransform;

    Material* material;
};

#endif // SIMPLEOBJECT3D_H
