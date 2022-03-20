#include "widget.h"
#include <QVector>
#include <QVector3D>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QQuaternion>
#include <QOpenGLContext>
#include "Models/ObjectEngine3D.h"
#include "Transformational.h"
#include "Models/SimpleObject3D.h"
#include "Group.h"
#include <QtMath>
#include "Camera.h"
#include "SkyBox.h"
#include <QStringList>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include "Models/Material.h"
#include <QOpenGLFramebufferObject>
#include <Game/ChessDesk.h>
#include <Game/ChessFigures.h>
#include <QPushButton>
#include <QLineEdit>
widget::widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    button = new QPushButton(this);
    lineEdit = new QLineEdit(this);
    button->move(130.0f, 0.0f);


    camera = new Camera;
    camera->translate(QVector3D(0.0f, 0.0f, -100.0f));
    camera->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 45.0f));
    setFocusPolicy(Qt::StrongFocus);
    fbHeight = 1024;
    fbWidth = 1024;
    connect(button, SIGNAL(clicked()), SLOT(OnButtonClicked()));
    projectionLightMatrix.setToIdentity();
    projectionLightMatrix.ortho(-40.0f, 40.0f, -40.0f, 40.0f, -40.0f, 40.0f);

    lightRotateX = 30.0f;
    lightRotateY = 40.0f;
    shadowLightMatrix.setToIdentity();
    shadowLightMatrix.rotate(lightRotateX, 1.0f, 0.0f, 0.0f);
    shadowLightMatrix.rotate(lightRotateY, 0.0f, 1.0f, 0.0f);

    lightMatrix.setToIdentity();
    lightMatrix.rotate(-lightRotateY, 0.0f, 1.0f, 0.0f);
    lightMatrix.rotate(-lightRotateX, 1.0f, 0.0f, 0.0f);
}

widget::~widget()
{
    delete camera;
    delete skyBox;
    for (int i = 0; i < objects.size(); i++)
        delete objects[i];
    delete Desk;
}

//virtual openGL functions
void widget::initializeGL()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();


    //floor
    Desk = new ChessDesk;
    transformObjects.append(Desk);
    FiguresDark = new ChessFigures;
    transformObjects.append(FiguresDark);
    //TEST///////
    //////////////
    FiguresDark->getFigure(Chess::Rook)->move2D(QVector2D(Desk->getPosition("h8")->x(), Desk->getPosition("h8")->y()));
    FiguresDark->getFigure(Chess::Bishop)->move2D(QVector2D(Desk->getPosition("g8")->x(), Desk->getPosition("g8")->y()));
    FiguresDark->getFigure(Chess::Knight)->move2D(QVector2D(Desk->getPosition("f8")->x(), Desk->getPosition("f8")->y()));
    FiguresDark->getFigure(Chess::King)->move2D(QVector2D(Desk->getPosition("e8")->x(), Desk->getPosition("e8")->y()));
    FiguresDark->getFigure(Chess::Queen)->move2D(QVector2D(Desk->getPosition("d8")->x(), Desk->getPosition("d8")->y()));
    FiguresDark->getFigure(Chess::Knight2)->move2D(QVector2D(Desk->getPosition("c8")->x(), Desk->getPosition("c8")->y()));
    FiguresDark->getFigure(Chess::Bishop2)->move2D(QVector2D(Desk->getPosition("b8")->x(), Desk->getPosition("h8")->y()));
    FiguresDark->getFigure(Chess::Rook2)->move2D(QVector2D(Desk->getPosition("a8")->x(), Desk->getPosition("a8")->y()));
    FiguresDark->getFigure(Chess::Pawn1)->move2D(QVector2D(Desk->getPosition("a7")->x(), Desk->getPosition("a7")->y()));
    qDebug() << *Desk->getPosition("a7");
    FiguresDark->getFigure(Chess::Pawn2)->move2D(QVector2D(Desk->getPosition("b7")->x(), Desk->getPosition("b7")->y()));
    qDebug() << *Desk->getPosition("b7");
    FiguresDark->getFigure(Chess::Pawn3)->move2D(QVector2D(Desk->getPosition("c7")->x(), Desk->getPosition("c7")->y()));
    qDebug() << *Desk->getPosition("c7");
    FiguresDark->getFigure(Chess::Pawn4)->move2D(QVector2D(Desk->getPosition("d7")->x(), Desk->getPosition("d7")->y()));
    qDebug() << *Desk->getPosition("d7");
    FiguresDark->getFigure(Chess::Pawn5)->move2D(QVector2D(Desk->getPosition("e7")->x(), Desk->getPosition("e7")->y()));
    qDebug() << *Desk->getPosition("e7");
    FiguresDark->getFigure(Chess::Pawn6)->move2D(QVector2D(Desk->getPosition("f7")->x(), Desk->getPosition("f7")->y()));
    qDebug() << *Desk->getPosition("f7");
    FiguresDark->getFigure(Chess::Pawn7)->move2D(QVector2D(Desk->getPosition("g7")->x(), Desk->getPosition("g7")->y()));
    qDebug() << *Desk->getPosition("g7");
    FiguresDark->getFigure(Chess::Pawn8)->move2D(QVector2D(Desk->getPosition("h7")->x(), Desk->getPosition("h7")->y()));
    qDebug() << *Desk->getPosition("h7");
    //////////////
    //camera
    QMatrix4x4 tmp;
    tmp.setToIdentity();
    camera->setGlobalTransform(tmp);

    //skybox
    skyBox = new SkyBox(1000, QImage(":/Sourse/Models/pngegg.png"));

    depthBuffer = new QOpenGLFramebufferObject(fbWidth, fbHeight, QOpenGLFramebufferObject::Depth);

    timer.start(30, this);
}

void widget::resizeGL(int width, int height)
{
    float aspect = (float)width / (float)height;
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45, aspect, 0.01f, 1000.0f);
}

void widget::paintGL()
{
    //Отрисовка в буффер
    depthBuffer->bind();
    glViewport(0, 0, fbWidth, fbHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderDepthProgram.bind();
    shaderDepthProgram.setUniformValue("u_projectionLightMatrix", projectionLightMatrix);
    shaderDepthProgram.setUniformValue("u_shadowLightMatrix", shadowLightMatrix);
    for (int i = 0; i < transformObjects.size(); i++)
    {
        transformObjects[i]->drawModel(&shaderDepthProgram, context()->functions());
    }
    shaderDepthProgram.release();
    depthBuffer->release();
    GLuint texture = depthBuffer->texture();
    context()->functions()->glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, texture);

    //Отрисовка в экран
    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderSkyBoxProgram.bind();
    shaderSkyBoxProgram.setUniformValue("u_projectionMatrix", projectionMatrix);
    camera->drawModel(&shaderSkyBoxProgram);
    skyBox->drawModel(&shaderSkyBoxProgram,  context()->functions());
    shaderSkyBoxProgram.release();
    shaderProgram.bind();
    shaderProgram.setUniformValue("u_shadowMap", GL_TEXTURE4 - GL_TEXTURE0);
    shaderProgram.setUniformValue("u_projectionMatrix", projectionMatrix);
    shaderProgram.setUniformValue("u_lightDirection", QVector4D(0.0f, 0.0f, -1.0f, 0.0f));
    shaderProgram.setUniformValue("u_projectionLightMatrix", projectionLightMatrix);
    shaderProgram.setUniformValue("u_shadowLightMatrix", shadowLightMatrix);
    shaderProgram.setUniformValue("u_lightMatrix", lightMatrix);
    shaderProgram.setUniformValue("u_lightPower", 1.0f);
    camera->drawModel(&shaderProgram);
    for (int i = 0; i < transformObjects.size(); i++)
    {
        transformObjects[i]->drawModel(&shaderProgram, context()->functions());
    }
    shaderProgram.release();
}
//*//
void widget::initShaders()
{
    if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Engine/Shaders/vshader.vsh"))
        close();
    if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Engine/Shaders/fshader.fsh"))
        close();
    if(!shaderProgram.link())
        close();

    if(!shaderSkyBoxProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Engine/Shaders/skybox.vsh"))
        close();
    if(!shaderSkyBoxProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Engine/Shaders/skybox.fsh"))
        close();
    if(!shaderSkyBoxProgram.link())
        close();

    if(!shaderDepthProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Engine/Shaders/depthShader.vsh"))
        close();
    if(!shaderDepthProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Engine/Shaders/depthShader.fsh"))
        close();
    if(!shaderDepthProgram.link())
        close();
}

//Events
void widget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton) return;
    mousePosition = QVector2D(event->position());

    event->accept();
}

void widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
        updateAngle(event);

    update();
}

void widget::wheelEvent(QWheelEvent *event)
{
    updateScale(event);
    update();
}

void widget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    lightRotateY += 0.01;
    shadowLightMatrix.setToIdentity();
    shadowLightMatrix.rotate(lightRotateX, 1.0f, 0.0f, 0.0f);
    shadowLightMatrix.rotate(lightRotateY, 0.0f, 1.0f, 0.0f);

    lightMatrix.setToIdentity();
    lightMatrix.rotate(-lightRotateY, 0.0f, 1.0f, 0.0f);
    lightMatrix.rotate(-lightRotateX, 1.0f, 0.0f, 0.0f);

    update();
}

void widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:

        break;
    case Qt::Key_Right:

        break;
    case Qt::Key_Down:

        break;
   case Qt::Key_Up:

        break;
    }
    update();
}

void widget::OnButtonClicked()
{
   QString key = lineEdit->text();
   qDebug() << key;
   QVector2D pos = *Desk->getPosition(key);
   qDebug() << pos;

}

//*//
//updates
void widget::updateAngle(QMouseEvent* event)
{
    QVector2D diff = QVector2D(event->position()) - mousePosition;
    mousePosition = QVector2D(event->position());

    float angle = diff.length() / 2.0f;
    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0f);

    camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));
}

void widget::updateScale(QWheelEvent* event)
{
    if(event->angleDelta().y() > 0)
        camera->translate(QVector3D(0.0f, 0.0f, 0.25f));
    else if (event->angleDelta().y() < 0)
        camera->translate(QVector3D(0.0f, 0.0f, -0.25f));
}


