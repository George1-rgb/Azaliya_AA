#include "ChessDesk.h"
#include "Models/ObjectEngine3D.h"
#include "Models/Material.h"
#include <QString>
#include <QPoint>
ChessDesk::ChessDesk()
{
    QImage diffuseMap(":/Sourse/Models/chessDesk.jpg");
    QImage normalMap(":/Sourse/Models/Abstract_Organic_006_normal.jpg");
    initCube(70.0f, 2.0f, 70.0f, &diffuseMap, &normalMap);
    Desk->translate(QVector3D(0.0f, 0.0f, 0.0f));
    InitializeCoords();
}

ChessDesk::~ChessDesk()
{
    delete Desk;
}

QVector2D* ChessDesk::getPosition(const QString &key)
{
    return &coords[key];
}

void ChessDesk::initCube(float width, float height, float depth, QImage* diffuseMap, QImage* normalMap)
{
    float width_div_2 = width / 2.0f;
    float height_div_2 = height / 2.0f;
    float depth_div_2 = depth / 2.0f;
    QVector<VertexData> vertexes;
    vertexes.push_back(VertexData(QVector3D(-width_div_2, height_div_2, depth_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, -height_div_2, depth_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, height_div_2, depth_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, height_div_2, depth_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, -height_div_2, depth_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, -height_div_2, depth_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));

       vertexes.push_back(VertexData(QVector3D(width_div_2, height_div_2, depth_div_2), QVector2D(0.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, -height_div_2, depth_div_2), QVector2D(0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, height_div_2, -depth_div_2), QVector2D(1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, height_div_2, -depth_div_2), QVector2D(1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, -height_div_2, depth_div_2), QVector2D(0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, -height_div_2, -depth_div_2), QVector2D(1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));

       vertexes.push_back(VertexData(QVector3D(width_div_2, height_div_2, depth_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, height_div_2, -depth_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, height_div_2, depth_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, height_div_2, depth_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, height_div_2, -depth_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, height_div_2, -depth_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));

       vertexes.push_back(VertexData(QVector3D(width_div_2, height_div_2, -depth_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, -height_div_2, -depth_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, height_div_2, -depth_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, height_div_2, -depth_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, -height_div_2, -depth_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, -height_div_2, -depth_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));

       vertexes.push_back(VertexData(QVector3D(-width_div_2, height_div_2, depth_div_2), QVector2D(0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, height_div_2, -depth_div_2), QVector2D(0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, -height_div_2, depth_div_2), QVector2D(1.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, -height_div_2, depth_div_2), QVector2D(1.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, height_div_2, -depth_div_2), QVector2D(0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, -height_div_2, -depth_div_2), QVector2D(1.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));

       vertexes.push_back(VertexData(QVector3D(-width_div_2, -height_div_2, depth_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, -height_div_2, -depth_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, -height_div_2, depth_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, -height_div_2, depth_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(-width_div_2, -height_div_2, -depth_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));
       vertexes.push_back(VertexData(QVector3D(width_div_2, -height_div_2, -depth_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));

       QVector<GLuint> indexes;
       indexes.reserve(36);

       for (int i = 0; i < 36; ++i)
       {
           indexes.push_back(i);
       }

    Material* newMtl = new Material;
    if (diffuseMap != NULL)
    newMtl->setDiffuseMap(*diffuseMap);
    if (normalMap != NULL)
    newMtl->setNormalMap(*normalMap);
    newMtl->setShines(96);
    newMtl->setDiffuseColor(QVector3D(1.0f, 1.0, 1.0f));
    newMtl->setSpecularColor(QVector3D(1.0f, 1.0, 1.0f));
    newMtl->setAmbientColor(QVector3D(1.0f, 1.0, 1.0f));

    Desk = new ObjectEngine3D;
    Desk->calculateTBN(vertexes);
    Desk->addObject(new SimpleObject3D(vertexes, indexes, newMtl));
}

void ChessDesk::rotate(const QQuaternion &r)
{
    Desk->rotate(r);
}

void ChessDesk::translate(const QVector3D &t)
{
    Desk->translate(t);
}

void ChessDesk::scale(const float &s)
{
    Desk->scale(s);
}

void ChessDesk::setGlobalTransform(const QMatrix4x4 &g)
{
    Desk->setGlobalTransform(g);
}

void ChessDesk::drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    Desk->drawModel(program, functions);
}

void ChessDesk::InitializeCoords()
{
    coords.insert("a1", QVector2D(25.0f, -25.0f));
    coords.insert("a2", QVector2D(25.0f, -18.0f));
    coords.insert("a3", QVector2D(25.0f, -10.6f));
    coords.insert("a4", QVector2D(25.0f, -3.5f));
    coords.insert("a5", QVector2D(25.0f, 4.0f));
    coords.insert("a6", QVector2D(25.0f, 11.0f));
    coords.insert("a7", QVector2D(25.0f, 18.0f));
    coords.insert("a8", QVector2D(25.0f, 25.0f));

    coords.insert("b1", QVector2D(18.0f, -25.0f));
    coords.insert("b2", QVector2D(18.0f, -18.0f));
    coords.insert("b3", QVector2D(18.0f, -10.6f));
    coords.insert("b4", QVector2D(18.0f, -3.5f));
    coords.insert("b5", QVector2D(18.0f, 4.0f));
    coords.insert("b6", QVector2D(18.0f, 11.0f));
    coords.insert("b7", QVector2D(18.0f, 18.0f));
    coords.insert("b8", QVector2D(18.0f, 25.0f));

    coords.insert("c1", QVector2D(10.6f, -25.0f));
    coords.insert("c2", QVector2D(10.6f, -18.0f));
    coords.insert("c3", QVector2D(10.6f, -10.6f));
    coords.insert("c4", QVector2D(10.6f, -3.5f));
    coords.insert("c5", QVector2D(10.6f, 4.0f));
    coords.insert("c6", QVector2D(10.6f, 11.0f));
    coords.insert("c7", QVector2D(10.6f, 18.0f));
    coords.insert("c8", QVector2D(10.6f, 25.0f));

    coords.insert("d1", QVector2D(3.5f, -25.0f));
    coords.insert("d2", QVector2D(3.5f, -18.0f));
    coords.insert("d3", QVector2D(3.5f, -10.6f));
    coords.insert("d4", QVector2D(3.5f, -3.5f));
    coords.insert("d5", QVector2D(3.5f, 4.0f));
    coords.insert("d6", QVector2D(3.5f, 11.0f));
    coords.insert("d7", QVector2D(3.5f, 18.0f));
    coords.insert("d8", QVector2D(3.5f, 25.0f));

    coords.insert("e1", QVector2D(-3.5f, -25.0f));
    coords.insert("e2", QVector2D(-3.5f, -18.0f));
    coords.insert("e3", QVector2D(-3.5f, -10.6f));
    coords.insert("e4", QVector2D(-3.5f, -3.5f));
    coords.insert("e5", QVector2D(-3.5f, 4.0f));
    coords.insert("e6", QVector2D(-3.5f, 11.0f));
    coords.insert("e7", QVector2D(-3.5f, 18.0f));
    coords.insert("e8", QVector2D(-3.5f, 25.0f));

    coords.insert("f1", QVector2D(-10.6f, -25.0f));
    coords.insert("f2", QVector2D(-10.6f, -18.0f));
    coords.insert("f3", QVector2D(-10.6f, -10.6f));
    coords.insert("f4", QVector2D(-10.6f, -3.5f));
    coords.insert("f5", QVector2D(-10.6f, 4.0f));
    coords.insert("f6", QVector2D(-10.6f, 11.0f));
    coords.insert("f7", QVector2D(-10.6f, 18.0f));
    coords.insert("f8", QVector2D(-10.6f, 25.0f));

    coords.insert("g1", QVector2D(-18.0f, -25.0f));
    coords.insert("g2", QVector2D(-18.0f, -18.0f));
    coords.insert("g3", QVector2D(-18.0f, -10.6f));
    coords.insert("g4", QVector2D(-18.0f, -3.5f));
    coords.insert("g5", QVector2D(-18.0f, 4.0f));
    coords.insert("g6", QVector2D(-18.0f, 11.0f));
    coords.insert("g7", QVector2D(-18.0f, 18.0f));
    coords.insert("g8", QVector2D(-18.0f, 25.0f));

    coords.insert("h1", QVector2D(-25.0f, -25.0f));
    coords.insert("h2", QVector2D(-25.0f, -18.0f));
    coords.insert("h3", QVector2D(-25.0f, -10.6f));
    coords.insert("h4", QVector2D(-25.0f, -3.5f));
    coords.insert("h5", QVector2D(-25.0f, 4.0f));
    coords.insert("h6", QVector2D(-25.0f, 11.0f));
    coords.insert("h7", QVector2D(-25.0f, 18.0f));
    coords.insert("h8", QVector2D(-25.0f, 25.0f));
}
