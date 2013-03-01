#ifndef BONE_HPP
#define BONE_HPP

#include <QGraphicsPolygonItem>

class Attachment;
class Arrow;

class Bone : public QGraphicsItem
{
public:
    Bone(const QString &name, Bone *parent = 0);
    ~Bone();

    Bone *parentBone() const;
    QList<Bone *> childBones() const;

    QString name() const;

    QList<Attachment *> attachments() const;
    void addAttachment(Attachment *attachment);
    void removeAttachment(Attachment *attachment);
    void mapAttachmentsToScene();
    void mapAttachmentsFromScene() const;

    void setScenePos(const QPointF &scenePos);
    void setSceneRotation(qreal sceneRotation);
    void setSceneScale(qreal sceneScale);
    void setScaleFromLength(qreal length);
    void setScaleFromSceneLength(qreal sceneLength);

    qreal sceneRotation() const;
    QPointF scenePeakPos() const;

    void setBoneLength(qreal length);
    void setBoneSceneLength(qreal sceneLength);

    QRectF boundingRect() const;
//    QPainterPath shape() const;

    void setJoint(bool isJoint);

    Arrow *arrow() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QPolygonF bonePolygon() const;
    QPainterPath bonePath() const;

    Arrow *m_arrow;

    bool m_isJoint;

    QString m_name;

    QList<Attachment *> m_attachments;

    qreal m_sceneRotation;
    qreal m_sceneScale;

    qreal m_boneLength;
};

#endif // BONE_HPP
