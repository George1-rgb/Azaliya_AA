#ifndef TYPES_H
#define TYPES_H
#include <QVector3D>
#include <QVector2D>

class QLabel;
class Widget;

struct VertexData
{
    VertexData()
    {

    }
    VertexData(QVector3D p, QVector2D t, QVector3D n) :
        position(p), texCoord(t), normal(n)
    {

    }
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
    QVector3D tangent;
    QVector3D bitangent;
};
#endif // TYPES_H
