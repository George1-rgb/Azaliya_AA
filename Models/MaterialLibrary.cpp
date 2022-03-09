#include "MaterialLibrary.h"
#include "Models/Material.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>

MaterialLibrary::MaterialLibrary()
{

}

void MaterialLibrary::addMaterial(Material *material)
{
    if (!material) return;
    for (int i = 0; i < materials.size(); i++)
        if (materials[i] == material)
            return;

    materials.append(material);
}

Material *MaterialLibrary::getMaterial(quint32 index)
{
    if (index < (quint32)materials.size())
        return materials[index];
    else
        return 0;
}

Material *MaterialLibrary::getMaterial(const QString &_mtlName)
{
    for (int i = 0; i < materials.size(); i++)
        if (materials[i]->getMtlName() == _mtlName)
            return materials[i];

    return 0;
}

quint32 MaterialLibrary::getCountOfMaterials()
{
    return materials.size();
}

void MaterialLibrary::loadMaterialFromFile(const QString &path)
{
    QFile mtlFile(path);
    if (!mtlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "MaterialLibrary: " << "Error reading fron file";
        return;
    }
    qDebug() << "Material file was open: " << path;
    for (int i = 0; i < materials.size(); i++)
        delete materials[i];
    materials.clear();

    QFileInfo fileInfo(path);
    QTextStream input(&mtlFile);
    Material* newMtl = 0;

    while(!input.atEnd())
    {
        QString str = input.readLine();
        QStringList list = str.split(" ");

        if (list[0] == "#")
        {
            qDebug() << "This is comment: " <<  str;
            continue;
        }
        else if (list[0] == "newmtl")
        {
            qDebug() << "Added new material";
            addMaterial(newMtl);
            newMtl = new Material;
            newMtl->setName(list[1]);

        }
        else if (list[0] == "Ns")
        {
            newMtl->setShines(list[1].toFloat());
        }
        else if (list[0] == "Ka")
        {
            newMtl->setAmbientColor(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
        }
        else if (list[0] == "Kd")
        {
            newMtl->setDiffuseColor(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
        }
        else if (list[0] == "Ks")
        {
            newMtl->setSpecularColor(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
        }
        else if (list[0] == "map_Kd")
        {
            newMtl->setDiffuseMap(QString("%1/%2").arg(fileInfo.absolutePath()).arg(list[1]));
        }
        else if (list[0] == "map_Bump")
        {
            newMtl->setNormalMap(QString("%1/%2").arg(fileInfo.absolutePath()).arg(list[1]));
        }
    }
    addMaterial(newMtl);

    mtlFile.close();
}
