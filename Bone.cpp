#include "Bone.hpp"
#include "Attachment.hpp"
#include <QGraphicsScene>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

static const qreal DefaultBoneLength = 80.0;
static const qreal DefaultBoneHeight = 5.0;
static QPolygonF BonePolygon;

static const qreal DefaultJointWidth = 10;
static const QRectF JointRect(-DefaultJointWidth/2, -DefaultJointWidth/2, DefaultJointWidth, DefaultJointWidth);

Bone::Bone(const QString &name, Bone *parent)
    : QGraphicsItem(parent)
    , m_isJoint(false)
    , m_name(name)
    , m_sceneRotation(0)
    , m_sceneScale(1)
    , m_boneLength(DefaultBoneLength)
{
    if(BonePolygon.isEmpty()) {
        BonePolygon << QPointF(0, 0)
                    << QPointF(10, 5)
                    << QPointF(m_boneLength, 0)
                    << QPointF(10, -5);
    }
    setFlags(ItemIsSelectable | ItemIsMovable | ItemIsPanel);
    setAcceptHoverEvents(true);
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

void Bone::setSceneScale(qreal sceneScale)
{
    Bone *parent = parentBone();
    if(parent) {
        sceneScale /= parent->m_sceneScale;
    }
    setScale(sceneScale);
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

void Bone::setBoneLength(qreal length)
{
    setJoint(length < DefaultJointWidth);

    if(!m_isJoint) {
        prepareGeometryChange();
    }
    m_boneLength = length;
}

void Bone::setBoneSceneLength(qreal sceneLength)
{
    Bone *parent = parentBone();
    if(parent) {
        sceneLength /= parent->m_sceneScale;
    }
    setBoneLength(sceneLength / scale());
}

void Bone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    qDebug() << option->state;
//    QStyleOptionGraphicsItem myOption(*option);
//    myOption.state &= ~QStyle::State_Selected;
//    QGraphicsPolygonItem::paint(painter, &myOption, widget);




    bool isSelected = option->state & QStyle::State_Selected;
//    bool isMouseOver = option->state & QStyle::State_MouseOver;
    painter->setBrush(isSelected? Qt::white : Qt::gray);
    painter->setPen(QPen(Qt::black, 0));
//    painter->setPen(QPen(Qt::lightGray, 0));

    if(m_isJoint) {
        painter->drawEllipse(JointRect);
    }
    else {
        painter->drawPolygon(bonePolygon());
    }

//    painter->setPen(QPen(Qt::black, 0));
    painter->setBrush(QColor(0, 0, 0, 50));
    painter->drawEllipse(QPointF(), 4, 4);
}

void Bone::setJoint(bool isJoint)
{
    if(m_isJoint == isJoint) {
        return;
    }

    prepareGeometryChange();
    m_isJoint = isJoint;
}

#include <math.h>
QPolygonF Bone::bonePolygon() const
{
    qreal height = 0.9*DefaultBoneHeight*log(0.7 * (m_boneLength*DefaultBoneHeight) / DefaultBoneLength);
    height = qMax(height, DefaultBoneHeight);

    qreal offset = m_boneLength / 8.0;
    offset = qMin(offset, 10.0);
    return QPolygonF() << QPointF(0, 0)
                       << QPointF(offset, height)
                       << QPointF(m_boneLength, 0)
                       << QPointF(offset, -height);
}

QRectF Bone::boundingRect() const
{
    return m_isJoint? JointRect : bonePolygon().boundingRect();
}

//QPainterPath Bone::shape() const
//{
//}
