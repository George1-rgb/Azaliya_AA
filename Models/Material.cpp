#include "Material.h"
#include <QString>
#include <QVector3D>
#include <QImage>

Material::Material()
    : isDiffuseMap(false), isNormalMap(false)
{

}

void Material::setName(const QString &_mtlName)
{
    mtlName = _mtlName;
}

const QString &Material::getMtlName() const
{
return mtlName;
}

void Material::setDiffuseColor(const QVector3D &_diffuseColor)
{
    diffuseColor = _diffuseColor;
}

const QVector3D &Material::getDiffuseColor() const
{
    return diffuseColor;
}

void Material::setAmbientColor(const QVector3D &_ambientColor)
{
    ambientColor = _ambientColor;
}

const QVector3D &Material::getAmbientColor() const
{
    return ambientColor;
}

void Material::setSpecularColor(const QVector3D &_specularColor)
{
    specularColor = _specularColor;
}

const QVector3D &Material::getSpeculatColor() const
{
    return specularColor;
}

void Material::setShines(const float &_shines)
{
    shines = _shines;
}

const float &Material::getShines() const
{
    return shines;
}

void Material::setDiffuseMap(const QString &_fileName)
{
    diffuseMap = QImage(_fileName);
    isDiffuseMap = true;
}

void Material::setDiffuseMap(const QImage &_texture)
{
    diffuseMap = _texture;
    isDiffuseMap = true;
}

const QImage &Material::getDiffuseMap() const
{

    return diffuseMap;
}

bool Material::isUsingDiffuseMap() const
{
    return isDiffuseMap;
}

void Material::setNormalMap(const QString &_fileName)
{
    normalMap = QImage(_fileName);
    isNormalMap = true;
}

void Material::setNormalMap(const QImage &_texture)
{
    normalMap = _texture;
    isNormalMap = true;
}

const QImage &Material::getNormalMap() const
{
    return normalMap;
}

bool Material::isUsingNormalMap() const
{
    return isNormalMap;
}
