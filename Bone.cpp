#include "Bone.hpp"
#include "Attachment.hpp"
#include "ResizeNode.hpp"
#include <QGraphicsScene>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

static const qreal DefaultBoneLength = 80.0;
static QPolygonF BonePolygon;

static const qreal DefaultJointWidth = 10;
static const QRectF JointRect(-DefaultJointWidth/2, -DefaultJointWidth/2, DefaultJointWidth, DefaultJointWidth);

Bone::Bone(const QString &name, Bone *parent)
    : QGraphicsItem(parent)
    , m_isJoint(false)
    , m_name(name)
    , m_resizeNode(new ResizeNode(this))
    , m_sceneRotation(0)
    , m_sceneScale(1)
{
    if(BonePolygon.isEmpty()) {
        BonePolygon << QPointF(0, 0)
                    << QPointF(10, 5)
                    << QPointF(DefaultBoneLength, 0)
                    << QPointF(10, -5);
    }
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsPanel);
    setAcceptHoverEvents(true);

    addResizeNodesIntoScene();
}

Bone *Bone::parentBone() const
{
    return dynamic_cast<Bone *>(parentItem());
}

QList<Bone *> Bone::childBones() const
{
    QList<Bone *> childBones;
    foreach(QGraphicsItem *child, childItems()) {
        Bone *bone = dynamic_cast<Bone *>(child);
        if(bone) {
            childBones << bone;
        }
    }
    return childBones;
}

QString Bone::name() const
{
    return m_name;
}

QList<Attachment *> Bone::attachments() const
{
    return m_attachments;
}

void Bone::addAttachment(Attachment *attachment)
{
    Q_ASSERT( attachment && !m_attachments.contains(attachment) );

    m_attachments << attachment;
}

void Bone::mapAttachmentsToScene()
{
    m_sceneRotation = rotation();
    m_sceneScale = scale();
    if(parentBone()) {
        m_sceneRotation += parentBone()->m_sceneRotation;
        m_sceneScale *= parentBone()->m_sceneScale;
    }

    foreach(Attachment *attachment, m_attachments) {
        attachment->setPos(mapToScene(attachment->localPos()));
        attachment->setRotation(attachment->localRotation() + m_sceneRotation);
        attachment->setScale(attachment->localScale() * m_sceneScale);
    }

    foreach(Bone *child, childBones()) {
        child->mapAttachmentsToScene();
    }
}

void Bone::mapAttachmentsFromScene() const
{
    foreach(Attachment *attachment, m_attachments) {
        attachment->setLocalPos(mapFromScene(attachment->pos()));
        attachment->setLocalRotation(attachment->rotation() - m_sceneRotation);
        attachment->setLocalScale(attachment->scale() / m_sceneScale);
    }

    foreach(Bone *child, childBones()) {
        child->mapAttachmentsFromScene();
    }
}

void Bone::setScenePos(const QPointF &scenePos)
{
    setPos(mapFromScene(scenePos));
}

void Bone::setSceneRotation(qreal sceneRotation)
{
    Bone *parent = parentBone();
    if(parent) {
        sceneRotation -= parent->m_sceneRotation;
    }
    setRotation(sceneRotation);
}

void Bone::setScaleFromLength(qreal length)
{
    if(length < 10) {
        setJoint(true);
        setScale(1);
    }
    else {
        setJoint(false);
        setScale(length / DefaultBoneLength);
    }
}

void Bone::setScaleFromSceneLength(qreal sceneLength)
{
    Bone *parent = parentBone();
    if(parent) {
        sceneLength /= parent->m_sceneScale;
    }
    setScaleFromLength(sceneLength);
}

QVariant Bone::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch(change) {
    case ItemSceneHasChanged:
    case ItemParentHasChanged:
    case ItemChildAddedChange:
        addResizeNodesIntoScene();

    default:
        break;
    }

    return QGraphicsItem::itemChange(change, value);
}

void Bone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    qDebug() << option->state;
//    QStyleOptionGraphicsItem myOption(*option);
//    myOption.state &= ~QStyle::State_Selected;
//    QGraphicsPolygonItem::paint(painter, &myOption, widget);

    bool isSelected = option->state & QStyle::State_Selected;
    bool isMouseOver = option->state & QStyle::State_MouseOver;
    painter->setBrush(isSelected? Qt::white : Qt::lightGray);
    painter->setPen(QPen(Qt::black, 0));

    if(m_isJoint) {
        painter->drawRect(JointRect);
    }
    else {
        painter->drawPolygon(BonePolygon);
    }
}

void Bone::setJoint(bool isJoint)
{
    if(m_isJoint == isJoint) {
        return;
    }

    prepareGeometryChange();
    m_isJoint = isJoint;
}

void Bone::addResizeNodesIntoScene() const
{
    if(m_resizeNode->scene()) {
        m_resizeNode->scene()->removeItem(m_resizeNode);
    }

    if(scene()) {
        scene()->addItem(m_resizeNode);
    }

    foreach(Bone *child, childBones()) {
        child->addResizeNodesIntoScene();
    }
}

void Bone::mapResizeNodesToScene() const
{
    qreal x = m_isJoint? DefaultJointWidth : DefaultBoneLength;
    m_resizeNode->setPos(mapToScene(QPointF(x, 0)));
    foreach(Bone *child, childBones()) {
        child->mapResizeNodesToScene();
    }
}

QRectF Bone::boundingRect() const
{
    return m_isJoint? JointRect : BonePolygon.boundingRect();
}

ResizeNode *Bone::resizeNode() const
{
    return m_resizeNode;
}
