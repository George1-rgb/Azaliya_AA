#ifndef CHESSDESK_H
#define CHESSDESK_H
#include "Engine/Transformational.h"
#include <QMap>
#include <QImage>
#include <QVector2D>
class ObjectEngine3D;
class ChessDesk : public Transformational
{
public:
    ChessDesk();
    ~ChessDesk();
    QVector2D* getPosition(const QString& key);
    virtual void rotate(const QQuaternion &r) override;
    virtual void translate(const QVector3D &t) override;
    virtual void scale(const float &s) override;
    virtual void setGlobalTransform(const QMatrix4x4 &g) override;
    virtual void drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;
private:
    void initCube(float width, float height, float depth, QImage* diffuseMap, QImage* normalMap);
    void InitializeCoords();
    ObjectEngine3D * Desk;
    QMap<QString, QVector2D> coords;
};

#endif // CHESSDESK_H
