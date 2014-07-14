#ifndef SCENEDATAMODEL_H
#define SCENEDATAMODEL_H

#include "custom/structures.h"

class SceneDataModel
{
public:
    SceneDataModel();

private:
    QList<QString> _scenes;
    QList<Sensor> _sensors;
};

#endif // SCENEDATAMODEL_H
