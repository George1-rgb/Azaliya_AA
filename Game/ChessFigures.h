#ifndef CHESSFIGURES_H
#define CHESSFIGURES_H

#include "Engine/Transformational.h"
#include <QMap>
#include "Engine/Types.h"

class ObjectEngine3D;
class ChessFigures : public Transformational
{
public:
    ChessFigures();
    ~ChessFigures();
    virtual void rotate(const QQuaternion &r) override;
    virtual void translate(const QVector3D &t) override;
    virtual void scale(const float &s) override;
    virtual void setGlobalTransform(const QMatrix4x4 &g) override;
    virtual void drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;
    ObjectEngine3D* getFigure(Chess name);
private:
    void initFigures();
    QMap<Chess, ObjectEngine3D*> figures;
};

#endif // CHESSFIGURES_H
