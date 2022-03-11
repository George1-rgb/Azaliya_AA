#include "SkyBox.h"
#include "Models/SimpleObject3D.h"
#include "Models/Material.h"
#include <QImage>
SkyBox::SkyBox(float width, const QImage &texture)
{
    float w = width / 2.0f;
    QVector<VertexData> vertexes;
    //z Позади нас
    vertexes.append(VertexData(QVector3D(-w, w, w), QVector2D(1.0f, 2.0f/3.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-w, -w, w), QVector2D(1.0f, 1.0/3.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(w, w, w), QVector2D(3.0f/4.0f, 2.0f/3.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(w, -w, w), QVector2D(3.0f/4.0f, 1.0f/3.0f), QVector3D(0.0f, 0.0f, -1.0f)));

    //x Справа от нас
    vertexes.append(VertexData(QVector3D(w, w, w), QVector2D(3.0f/4.0f, 2.0f/3.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w, -w, w), QVector2D(3.0f/4.0f, 1.0f/3.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w, w, -w), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w, -w, -w), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(-1.0f, 0.0f, 0.0f)));

    //y Над нами
    vertexes.append(VertexData(QVector3D(w, w, w), QVector2D(2.0f/4.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w, w, -w), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w, w, w), QVector2D(1.0f/4.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w, w, -w), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(0.0f, -1.0f, 0.0f)));

    //-z Перед нами
    vertexes.append(VertexData(QVector3D(w, w, -w), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(w, -w, -w), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-w, w, -w), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-w, -w, -w), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(0.0f, 0.0f, 1.0f)));

    //-x Слева от нас
    vertexes.append(VertexData(QVector3D(-w, w, w), QVector2D(0.0f, 2.0f/3.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w, w, -w), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w, -w, w), QVector2D(0.0f, 1.0f/3.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w, -w, -w), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(1.0f, 0.0f, 0.0f)));

    //-y Под нами
    vertexes.append(VertexData(QVector3D(-w, -w, w), QVector2D(1.0f/4.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w, -w, -w), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w, -w, w), QVector2D(2.0f/4.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w, -w, -w), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(0.0f, 1.0f, 0.0f)));

    QVector<GLuint> indexes;
    for (int i = 0; i <= 24; i += 4)
    {
        indexes.append(i + 0);
        indexes.append(i + 2);
        indexes.append(i + 1);

        indexes.append(i + 2);
        indexes.append(i + 3);
        indexes.append(i + 1);
}

    Material* newMtl = new Material;
    newMtl->setDiffuseMap(texture);
    newMtl->setShines(96);
    newMtl->setDiffuseColor(QVector3D(1.0f, 1.0, 1.0f));
    newMtl->setSpecularColor(QVector3D(1.0f, 1.0, 1.0f));
    newMtl->setAmbientColor(QVector3D(1.0f, 1.0, 1.0f));

    cube = new SimpleObject3D(vertexes, indexes, newMtl);
}

SkyBox::~SkyBox()
{
    delete cube;
}

void SkyBox::drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    cube->drawModel(program, functions);
}

void SkyBox::rotate(const QQuaternion &r)
{
    (void)r;
}

void SkyBox::translate(const QVector3D &t)
{
    (void)t;
}

void SkyBox::scale(const float &s)
{
    (void)s;
}

void SkyBox::setGlobalTransform(const QMatrix4x4 &g)
{
    (void)g;
}
