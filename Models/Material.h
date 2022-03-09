#ifndef MATERIAL_H
#define MATERIAL_H

#include <QString>
#include <QVector3D>
#include <QImage>
class Material
{
public:
    Material();
    void setName(const QString &_mtlName);
    const QString &getMtlName() const;

    void setDiffuseColor(const QVector3D &_diffuseColor);
    const QVector3D &getDiffuseColor() const;

    void setAmbientColor(const QVector3D &_ambientColor);
    const QVector3D &getAmbientColor() const;

    void setSpecularColor(const QVector3D &_specularColor);
    const QVector3D &getSpeculatColor() const;

    void setShines(const float &_shines);
    const float &getShines() const;

    void setDiffuseMap(const QString &_fileName);
    void setDiffuseMap(const QImage &_texture);
    const QImage &getDiffuseMap() const;
    bool isUsingDiffuseMap() const;

    void setNormalMap(const QString &_fileName);
    void setNormalMap(const QImage &_texture);
    const QImage &getNormalMap() const;
    bool isUsingNormalMap() const;

private:
    QString mtlName;
    QVector3D diffuseColor;
    QVector3D ambientColor;
    QVector3D specularColor;
    float shines;
    QImage diffuseMap;
    QImage normalMap;
    bool isDiffuseMap;
    bool isNormalMap;
};

#endif // MATERIAL_H
