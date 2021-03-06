#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include "Types.h"
#include <QBasicTimer>

class QPushButton;
class QLineEdit;
class ObjectEngine3D;
class Transformational;
class ChessDesk;
class ChessFigures;
class Group;
class Camera;
class SkyBox;
class QOpenGLFramebufferObject;

class widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    widget(QWidget *parent = nullptr);
    ~widget();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int width, int height) override;
    virtual void paintGL() override;

    void initShaders();

    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void timerEvent(QTimerEvent* event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
public slots:
    void OnButtonClicked();
private:
    QPushButton *button;
    QLineEdit *lineEdit;

    QMatrix4x4 projectionMatrix;
    QMatrix4x4 projectionLightMatrix;
    QMatrix4x4 lightMatrix;
    QMatrix4x4 shadowLightMatrix;

    float lightRotateX;
    float lightRotateY;

    QOpenGLShaderProgram shaderProgram;
    QOpenGLShaderProgram shaderSkyBoxProgram;
    QOpenGLShaderProgram shaderDepthProgram;
    QVector2D mousePosition;
    QBasicTimer timer;


    QVector<ObjectEngine3D*> objects;
    QVector<Transformational*> transformObjects;
    QVector<Group*> groups;

    void updateAngle(QMouseEvent* event);
    void updateScale(QWheelEvent* event);
//    void updateCameraLocation(QMouseEvent* event);

    float angleObject;
    float angleGroup1;
    float angleGroup2;
    float angleMain;
    ChessDesk *Desk;
    ChessFigures* FiguresDark;
    Camera*  camera;
    SkyBox* skyBox;
    QOpenGLFramebufferObject* depthBuffer;
    quint32 fbHeight;
    quint32 fbWidth;
};
#endif // WIDGET_H
