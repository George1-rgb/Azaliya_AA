#include "Game/ChessFigures.h"
#include "Models/ObjectEngine3D.h"
#include "Engine/Types.h"
ChessFigures::ChessFigures()
{
    initFigures();
}

ChessFigures::~ChessFigures()
{

}

void ChessFigures::rotate(const QQuaternion &r)
{
    QMap<Chess, ObjectEngine3D*>::Iterator It = figures.begin();
    for (; It != figures.end(); It++)
    {
        It.value()->rotate(r);
    }
}

void ChessFigures::translate(const QVector3D &t)
{
    QMap<Chess, ObjectEngine3D*>::Iterator It = figures.begin();
    for (; It != figures.end(); It++)
    {
        It.value()->translate(t);
    }
}

void ChessFigures::scale(const float &s)
{
    QMap<Chess, ObjectEngine3D*>::Iterator It = figures.begin();
    for (; It != figures.end(); It++)
    {
        It.value()->scale(s);
    }
}

void ChessFigures::setGlobalTransform(const QMatrix4x4 &g)
{
    QMap<Chess, ObjectEngine3D*>::Iterator It = figures.begin();
    for (; It != figures.end(); It++)
    {
        It.value()->setGlobalTransform(g);
    }
}

void ChessFigures::drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    QMap<Chess, ObjectEngine3D*>::Iterator It = figures.begin();
    for (; It != figures.end(); It++)
    {
        It.value()->drawModel(program, functions);
    }
}

ObjectEngine3D* ChessFigures::getFigure(Chess name)
{
    return figures[name];
}

void ChessFigures::initFigures()
{
    figures[Chess::Bishop] = new ObjectEngine3D;
    figures[Chess::Bishop2] = new ObjectEngine3D;
    figures[Chess::King] = new ObjectEngine3D;
    figures[Chess::Knight] = new ObjectEngine3D;
    figures[Chess::Knight2] = new ObjectEngine3D;
    figures[Chess::Pawn1] = new ObjectEngine3D;
    figures[Chess::Pawn2] = new ObjectEngine3D;
    figures[Chess::Pawn3] = new ObjectEngine3D;
    figures[Chess::Pawn4] = new ObjectEngine3D;
    figures[Chess::Pawn5] = new ObjectEngine3D;
    figures[Chess::Pawn6] = new ObjectEngine3D;
    figures[Chess::Pawn7] = new ObjectEngine3D;
    figures[Chess::Pawn8] = new ObjectEngine3D;
    figures[Chess::Queen] = new ObjectEngine3D;
    figures[Chess::Rook] = new ObjectEngine3D;
    figures[Chess::Rook2] = new ObjectEngine3D;

    QMap<Chess, ObjectEngine3D*>::Iterator It = figures.begin();
    for (int i = 1; It != figures.end(); It++)
    {
        It.value()->loadObjectFromFile(":/Sourse/Models/Chess/" + QString::number(i) + ".obj");
        It.value()->translate(QVector3D(0.0f, 1.0f, 0.0f));
        It.value()->scale(1.5f);
        i++;
    }
}
