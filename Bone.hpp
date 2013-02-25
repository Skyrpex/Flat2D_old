#ifndef BONE_HPP
#define BONE_HPP

#include <QGraphicsPolygonItem>

class Attachment;
class ResizeNode;

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

    ResizeNode *resizeNode() const;
    void addResizeNodesIntoScene() const;
    void mapResizeNodesToScene() const;

    QRectF boundingRect() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void setJoint(bool isJoint);

    bool m_isJoint;

    QString m_name;

    ResizeNode *m_resizeNode;
    QList<Attachment *> m_attachments;

    qreal m_sceneRotation;
    qreal m_sceneScale;
};

#endif // BONE_HPP
