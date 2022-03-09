#include "SimpleObject3D.h"
#include "Models/Material.h"
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

SimpleObject3D::SimpleObject3D()
    : indexBuffer(QOpenGLBuffer::IndexBuffer), diffuseMap(0), Scale(1.0f)
{

}

SimpleObject3D::SimpleObject3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, Material* _material)
    : indexBuffer(QOpenGLBuffer::IndexBuffer), diffuseMap(0), Scale(1.0f)
{
    initModel(vertData, indexes, _material);
}

SimpleObject3D::~SimpleObject3D()
{
    if(vertexBuffer.isCreated())
        vertexBuffer.destroy();
    if(indexBuffer.isCreated())
        indexBuffer.destroy();
    if(diffuseMap != 0 && diffuseMap->isCreated())
        diffuseMap->destroy();
}

void SimpleObject3D::initModel(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, Material* _material)
{
    if(vertexBuffer.isCreated())
        vertexBuffer.destroy();
    if(indexBuffer.isCreated())
        indexBuffer.destroy();
    if(diffuseMap != 0 && diffuseMap->isCreated())
    {
        delete diffuseMap;
        diffuseMap = 0;
    }

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertData.constData(), vertData.size() * sizeof(VertexData));
    vertexBuffer.release();

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    indexBuffer.release();

    material = _material;

    if (material->isUsingDiffuseMap())
    {
        diffuseMap = new QOpenGLTexture(material->getDiffuseMap().mirrored());
        diffuseMap->setMinificationFilter(QOpenGLTexture::Nearest);
        diffuseMap->setMagnificationFilter(QOpenGLTexture::Linear);
        diffuseMap->setWrapMode(QOpenGLTexture::Repeat);
    }
    if (material->isUsingNormalMap())
    {
        normalMap = new QOpenGLTexture(material->getNormalMap().mirrored());
        normalMap->setMinificationFilter(QOpenGLTexture::Nearest);
        normalMap->setMagnificationFilter(QOpenGLTexture::Linear);
        normalMap->setWrapMode(QOpenGLTexture::Repeat);
    }
}

void SimpleObject3D::drawModel(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if(!vertexBuffer.isCreated() || !indexBuffer.isCreated())
        return;

    if (material->isUsingDiffuseMap())
    {
        diffuseMap->bind(0);
        program->setUniformValue("u_diffuseMap", 0);
    }

    if (material->isUsingNormalMap())
    {
        normalMap->bind(1);
        program->setUniformValue("u_normalMap", 1);
    }

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(Translation);
    modelMatrix.rotate(Rotation);
    modelMatrix.scale(Scale);
    modelMatrix = GlobalTransform * modelMatrix;

    program->setUniformValue("u_modelMatrix", modelMatrix);

    program->setUniformValue("u_materialProperty.diffuseColor", material->getDiffuseColor());
    program->setUniformValue("u_materialProperty.ambientColor", material->getAmbientColor());
    program->setUniformValue("u_materialProperty.specularColor", material->getSpeculatColor());
    program->setUniformValue("u_materialProperty.shines", material->getShines());
    program->setUniformValue("u_isUsingDiffuseMap", material->isUsingDiffuseMap());
    program->setUniformValue("u_isUsingNormalMap", material->isUsingNormalMap());

    vertexBuffer.bind();
    int offset = 0;
    int vertLoc =  program->attributeLocation("a_position");
    program->enableAttributeArray(vertLoc);
    program->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int textLoc = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(textLoc);
    program->setAttributeBuffer(textLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    int normLoc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normLoc);
    program->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int tangentLoc = program->attributeLocation("a_tangent");
    program->enableAttributeArray(tangentLoc);
    program->setAttributeBuffer(tangentLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int bitangentLoc = program->attributeLocation("a_bitangent");
    program->enableAttributeArray(bitangentLoc);
    program->setAttributeBuffer(bitangentLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    indexBuffer.bind();
    functions->glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);
    vertexBuffer.release();
    indexBuffer.release();

    if (material->isUsingDiffuseMap())
        diffuseMap->release();
    if (material->isUsingNormalMap())
        normalMap->release();

}

void SimpleObject3D::rotate(const QQuaternion &r)
{
    Rotation = r * Rotation;
}

void SimpleObject3D::translate(const QVector3D &t)
{
    Translation += t;
}

void SimpleObject3D::scale(const float &s)
{
    Scale *= s;
}

void SimpleObject3D::setGlobalTransform(const QMatrix4x4 &g)
{
    GlobalTransform = g;
}
