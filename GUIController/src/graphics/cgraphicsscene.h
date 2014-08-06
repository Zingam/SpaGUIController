#ifndef CGRAPHICSSCENE_H
#define CGRAPHICSSCENE_H

#include <QtWidgets/QGraphicsScene>

#include "../mainwindow.h"

class MainWindow;


class CGraphicsScene : public QGraphicsScene
{
private:
    MainWindow* _mainWindow;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);

public:
    CGraphicsScene(MainWindow* mainWindow);
};

#endif // CGRAPHICSSCENE_H
