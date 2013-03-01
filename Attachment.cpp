#include "Attachment.hpp"
#include <QDebug>

Attachment::Attachment(const QPixmap &pixmap)
    : QGraphicsPixmapItem(pixmap)
    , m_localRotation(0)
    , m_localScale(1)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setZValue(-1);

    QPointF pixmapSize(static_cast<qreal>(pixmap.width()), static_cast<qreal>(pixmap.height()));
    setOffset(-pixmapSize/2);
}

QPointF Attachment::localPos() const
{
    return m_localPos;
}

qreal Attachment::localRotation() const
{
    return m_localRotation;
}

qreal Attachment::localScale() const
{
    return m_localScale;
}

void Attachment::setLocalPos(const QPointF &pos)
{
    m_localPos = pos;
}

void Attachment::setLocalRotation(qreal rotation)
{
    m_localRotation = rotation;
}

void Attachment::setLocalScale(qreal scale)
{
    m_localScale = scale;
}
