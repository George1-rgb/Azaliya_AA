#include "widget.h"
#include <QVector>
#include <QVector3D>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QQuaternion>
#include <QDebug>
#include <QOpenGLContext>
#include "Models/ObjectEngine3D.h"
#include "Transformational.h"
#include "Models/SimpleObject3D.h"
#include "Group.h"
#include <QtMath>
#include "Camera.h"
#include "SkyBox.h"
#include <QFile>
#include <QStringList>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include "Models/Material.h"
#include <QOpenGLFramebufferObject>

widget::widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    camera = new Camera;
    camera->translate(QVector3D(0.0f, 0.0f, -40.0f));
    camera->rotate(QQuaternion::fromAxisAndAngle(1.0f, 1.0f, 0.0f, 45.0f));
    setFocusPolicy(Qt::StrongFocus);
    fbHeight = 1024;
    fbWidth = 1024;

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
//    for (int i = 0; i < groups.size();  i++)
//        delete groups[i];

}

void widget::createSphere()
{
    objects.append(new ObjectEngine3D);
    objects[objects.size() - 1]->loadObjectFromFile(":/Sourse/Models/defaultMaterial.obj");
    objects[objects.size() - 1]->translate(QVector3D(8.0f, 2.0f, 0.0f));
    transformObjects.append(objects[objects.size() - 1]);
}

void widget::createThore()
{
    objects.append(new ObjectEngine3D);
    objects[objects.size() - 1]->loadObjectFromFile(":/Sourse/Models/thor.obj");
    objects[objects.size() - 1]->scale(2.0f);
    objects[objects.size() - 1]->translate(QVector3D(-8.0f, 2.0f, 0.0f));
    transformObjects.append(objects[objects.size() - 1]);
}
//virtual openGL functions
void widget::initializeGL()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();



    objects.append(new ObjectEngine3D);
    objects[objects.size() - 1]->loadObjectFromFile(":/Sourse/Models/case.obj");
    objects[objects.size() - 1]->translate(QVector3D(0.0f, 2.0f, 8.0f));
    transformObjects.append(objects[objects.size() - 1]);



    //floor
    QImage diffuseMap(":/Sourse/Models/Abstract_Organic_006_basecolor.jpg");
    QImage normalMap(":/Sourse/Models/Abstract_Organic_006_normal.jpg");
    initCube(40.0f, 2.0f, 40.0f, &diffuseMap, &normalMap);
    objects[objects.size() - 1]->translate(QVector3D(0.0f, -2.0f, 0.0f));
    transformObjects.append(objects[objects.size() - 1]);

    //camera
    QMatrix4x4 tmp;
    tmp.setToIdentity();
    camera->setGlobalTransform(tmp);

    //skybox
    skyBox = new SkyBox(100, QImage(":/Sourse/Models/pngegg.png"));

    depthBuffer = new QOpenGLFramebufferObject(fbWidth, fbHeight, QOpenGLFramebufferObject::Depth);

    timer.start(30, this);
}

void widget::resizeGL(int width, int height)
{
    float aspect = (float)width / (float)height;
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45, aspect, 0.01f, 10000.0f);
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

void widget::initCube(float width, float height, float depth, QImage* diffuseMap, QImage* normalMap)
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

    ObjectEngine3D* objEngine = new ObjectEngine3D;
    objEngine->calculateTBN(vertexes);
    objEngine->addObject(new SimpleObject3D(vertexes, indexes, newMtl));
    objects.append(objEngine);
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
//    else if (event->buttons() == Qt::MiddleButton)
//        updateCameraLocation(event);


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

    lightRotateX += 0.5;
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
    qDebug() << "Event has done!";
    switch (event->key())
    {
    case Qt::Key_Left:
        groups[0]->delObject(camera);
        groups[1]->addObject(camera);
        break;
    case Qt::Key_Right:
        groups[1]->delObject(camera);
        groups[0]->addObject(camera);
        break;
    case Qt::Key_Down:
        groups[0]->delObject(camera);
        groups[1]->delObject(camera);
        break;
   case Qt::Key_Up:
        groups[0]->delObject(camera);
        groups[1]->delObject(camera);

        QMatrix4x4 tmp;
        tmp.setToIdentity();
        camera->setGlobalTransform(tmp);
        break;
    }
    update();
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

//void widget::updateCameraLocation(QMouseEvent *event)
//{

//    QVector2D diff = QVector2D(event->position()) - mousePosition;
//    mousePosition = QVector2D(event->position());
//    camera->translate(QVector3D(diff, 0.0f));
//}
//*//

