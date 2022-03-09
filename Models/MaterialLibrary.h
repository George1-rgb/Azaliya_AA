#ifndef MATERIALLIBRARY_H
#define MATERIALLIBRARY_H

#include <QVector>

class Material;

class MaterialLibrary
{
public:
    MaterialLibrary();
    void addMaterial(Material* material);

    Material* getMaterial(quint32 index);
    Material* getMaterial(const QString &_mtlName);

    quint32 getCountOfMaterials();

    void loadMaterialFromFile(const QString &path);

private:
    QVector<Material*> materials;
};

#endif // MATERIALLIBRARY_H
