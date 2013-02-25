#include "ResizeNode.hpp"
#include "Bone.hpp"
#include <QBrush>
#include <QDebug>

static const qreal NodeWidth = 8;

ResizeNode::ResizeNode(Bone *bone)
    : QGraphicsEllipseItem(NodeWidth/2, -NodeWidth/2, NodeWidth, NodeWidth)
    , m_bone(bone)
{
    Q_ASSERT(bone);

    setFlags(ItemIsMovable);
    setBrush(Qt::white);
}

void ResizeNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseMoveEvent(event);

    Q_ASSERT(m_bone);
    QLineF line(m_bone->scenePos(), scenePos());
    m_bone->setScaleFromSceneLength(line.length());
}
