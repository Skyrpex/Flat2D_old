#ifndef BONE_HPP
#define BONE_HPP

#include <QGraphicsPolygonItem>

class Attachment;

class Bone : public QGraphicsItem
{
public:
    Bone(const QString &name, Bone *parent = 0);

    Bone *parentBone() const;
    QList<Bone *> childBones() const;

    QString name() const;

    QList<Attachment *> attachments() const;
    void addAttachment(Attachment *attachment);
    void mapAttachmentsToScene();
    void mapAttachmentsFromScene() const;

    void setScenePos(const QPointF &scenePos);
    void setSceneRotation(qreal sceneRotation);
    void setScaleFromLength(qreal length);
    void setScaleFromSceneLength(qreal sceneLength);

    void setBoneLength(qreal length);
    void setBoneSceneLength(qreal sceneLength);

    QRectF boundingRect() const;
//    QPainterPath shape() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void setJoint(bool isJoint);
    QPolygonF bonePolygon() const;

    bool m_isJoint;

    QString m_name;

    QList<Attachment *> m_attachments;

    qreal m_sceneRotation;
    qreal m_sceneScale;

    qreal m_boneLength;
};

#endif // BONE_HPP
