#include "ObjectEngine3D.h"
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QTextStream>
#include <QVector3D>
#include <QString>
#include "Engine/Types.h"

ObjectEngine3D::ObjectEngine3D()
{

}

void ObjectEngine3D::loadObjectFromFile(const QString &path)
{
    QFile objFile(path);
    if(!objFile.exists())
    {
        qDebug() << "Can't open file: " << path;
        return;
    }

    objFile.open(QIODevice::ReadOnly);
    qDebug() << "Object file was open" << path;
    QTextStream input(&objFile);

    QVector<QVector3D> coords;
    QVector<QVector2D> textCoords;
    QVector<QVector3D> normals;

    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;
    SimpleObject3D* object = NULL;
    QString mtlName;
    while (!input.atEnd())
    {
       QString str = input.readLine();
       QStringList list = str.split(" ");

       if (list[0] == "#")
       {
           continue;
       }
       else if (list[0] == "mtllib")
       {
           QFileInfo info(path);
           QString matPath = QString("%1/%2").arg(info.absolutePath()).arg(list[1]);
           materialLibrary.loadMaterialFromFile(matPath);
           continue;
       }
       else if (list[0] == "v")
       {
           coords.append(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
           continue;
       }
       else if (list[0] == "vt")
       {
           textCoords.append(QVector2D(list[1].toFloat(), list[2].toFloat()));
           continue;
       }
       else if (list[0] == "vn")
       {
           normals.append(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
           continue;
       }
       else if (list[0] == "f")
       {
           for (int i = 1; i <= 3; i++)
           {
              QStringList vert = list[i].split("/");
              vertexes.append(VertexData(coords[vert[0].toLong() - 1], textCoords[vert[1].toLong() - 1], normals[vert[2].toLong() - 1]));
              indexes.append(indexes.size());
           }
           continue;
       }
       else if (list[0] == "usemtl")
       {
           if (object)
           {
               calculateTBN(vertexes);
               object->initModel(vertexes, indexes, materialLibrary.getMaterial(mtlName));
           }
           mtlName = list[1];
           addObject(object);
           object = new SimpleObject3D;
           vertexes.clear();
           indexes.clear();
       }
     }
    if (object)
    {
        calculateTBN(vertexes);
        object->initModel(vertexes, indexes, materialLibrary.getMaterial(mtlName));
    }
    addObject(object);
    objFile.close();
}

void ObjectEngine3D::addObject(SimpleObject3D *obj)
{
    if(!obj) return;
    for (int i = 0; i < objects.size(); i++)
        if(objects[i] == obj)
            return;
    objects.append(obj);
}

SimpleObject3D* ObjectEngine3D::getObject(quint32 index)
{
    if (index < (quint32)objects.size())
        return objects[index];
    else
        return NULL;
}

void ObjectEngine3D::calculateTBN(QVector<VertexData> &vertData)
{
    for (int i = 0; i < vertData.size(); i += 3)
    {
        QVector3D &v1 = vertData[i].position;
        QVector3D &v2 = vertData[i + 1].position;
        QVector3D &v3 = vertData[i + 2].position;

        QVector2D &uv1 = vertData[i].texCoord;
        QVector2D &uv2 = vertData[i + 1].texCoord;
        QVector2D &uv3 = vertData[i + 2].texCoord;

        QVector3D deltaPos1 = v2 - v1;
        QVector3D deltaPos2 = v3 - v1;

        QVector2D deltaUV1 = uv2 - uv1;
        QVector2D deltaUV2 = uv3 - uv1;

        float r = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());

        QVector3D tangent = (deltaPos1 * deltaUV2.y() - deltaPos2 * deltaUV1.y()) * r;
        QVector3D bitangent = (deltaPos2 * deltaUV1.x() - deltaPos1 * deltaUV2.x()) * r;

        vertData[i].tangent = tangent;
        vertData[i + 1].tangent = tangent;
        vertData[i + 2].tangent = tangent;

        vertData[i].bitangent = bitangent;
        vertData[i + 1].bitangent = bitangent;
        vertData[i + 2].bitangent = bitangent;
    }
}

void ObjectEngine3D::rotate(const QQuaternion &r)
{
    for (int i = 0; i < objects.size(); i++)
        objects[i]->rotate(r);
}

void ObjectEngine3D::translate(const QVector3D &t)
{
    for (int i = 0; i < objects.size(); i++)
        objects[i]->translate(t);
}

void ObjectEngine3D::scale(const float &s)
{
    for (int i = 0; i < objects.size(); i++)
        objects[i]->scale(s);
}

void ObjectEngine3D::setGlobalTransform(const QMatrix4x4 &g)
{
    for (int i = 0; i < objects.size(); i++)
        objects[i]->setGlobalTransform(g);
}

void ObjectEngine3D::drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    for (int i = 0; i < objects.size(); i++)
        objects[i]->drawModel(program, functions);
}
