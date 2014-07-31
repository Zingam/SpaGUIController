#include "cgraphicsrectitem.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOptionGraphicsItem>

CGraphicsRectItem::CGraphicsRectItem(QGraphicsItem *parent) :
    QGraphicsRectItem(parent)
{
}

void CGraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Disable the selection rectangle
    QStyleOptionGraphicsItem l_option(*option);
    // Remove the selected state from the flags
    l_option.state &= ~QStyle::State_Selected;
    QGraphicsRectItem::paint(painter, &l_option, widget);
}

void CGraphicsRectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit doubleClicked(event);
}

void CGraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsRectItem::mousePressEvent(event);
#ifdef DEBUG_MODE_FINETUNING
    if (Qt::RightButton == event->button()) {
        QString infoMessage = QString("Clicked CGraphicsRectItem at:\n"
                                      "x: %1\n"
                                      "y: %2")
                .arg(this->x())
                .arg(this->y());
        QMessageBox::information(nullptr,
                                 "Clicked CGraphicsRectItem",
                                 infoMessage);
    }
#endif // DEBUG_MODE_FINETUNING
}
