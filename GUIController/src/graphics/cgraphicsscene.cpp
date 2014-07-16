#include "cgraphicsscene.h"

#include <QtCore/QDebug>
#include <QtWidgets/QGraphicsSceneMouseEvent>


CGraphicsScene::CGraphicsScene(MainWindow* mainWindow)
{
    _mainWindow = mainWindow;
}

void CGraphicsScene::mousePressEvent (QGraphicsSceneMouseEvent* mouseEvent)
{
    QPointF point = mouseEvent->scenePos();
    _mainWindow->selectTemperatureIndicator(point);
    QGraphicsScene::mousePressEvent(mouseEvent);
}
