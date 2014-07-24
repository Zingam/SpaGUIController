#include "cgraphicsrectitem.h"

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
