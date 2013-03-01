#include "Attachment.hpp"
#include "Arrow.hpp"
#include "Bone.hpp"
#include <QDebug>
#include <QGraphicsScene>

Attachment::Attachment(const QPixmap &pixmap)
    : QGraphicsPixmapItem(pixmap)
    , m_bone(NULL)
    , m_arrow(new Arrow(NULL, this))
    , m_localRotation(0)
    , m_localScale(1)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setZValue(-1);

    QPointF pixmapSize(static_cast<qreal>(pixmap.width()), static_cast<qreal>(pixmap.height()));
    setOffset(-pixmapSize/2);

    setTransformationMode(Qt::SmoothTransformation);
}

Bone *Attachment::bone() const
{
    return m_bone;
}

void Attachment::setBone(Bone *bone)
{
    m_bone = bone;
    m_arrow->setStartItem(bone);
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

QVariant Attachment::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSceneChange) {
        if(scene()) {
            scene()->removeItem(m_arrow);
        }

        QGraphicsScene *scene = value.value<QGraphicsScene *>();
        if(scene) {
            scene->addItem(m_arrow);
        }
    }

    return QGraphicsPixmapItem::itemChange(change, value);
}
