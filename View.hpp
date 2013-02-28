#ifndef VIEW_HPP
#define VIEW_HPP

#include <QGraphicsView>
#include <QMap>

class Bone;
class Attachment;
class ResizeNode;

class View : public QGraphicsView
{
    Q_OBJECT
public:
    enum TargetMode {
        BoneTargetMode,
        AttachmentTargetMode
    };

    enum EditMode {
        TransformEditMode,
        CreateEditMode
    };

    enum TransformMode {
        SelectTransformMode,
        RotateTransformMode,
        ScaleTransformMode
    };

    explicit View(QWidget *parent = 0);

public slots:
    void setBoneTargetMode();
    void setAttachmentTargetMode();

    void setTransformEditMode();
    void setCreateEditMode();

    void setSelectTransformMode();
    void setRotateTransformMode();
    void setScaleTransformMode();

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    Bone *topLevelBone(const QList<QGraphicsItem *> &items) const;
    QGraphicsItem *nearestItem(const QPointF &scenePos, const QList<QGraphicsItem *> &items) const;
    QGraphicsItem *targetItem(const QList<QGraphicsItem *> &items) const;
    QPointF cursorScenePos() const;

    QList<Bone *> bones() const;
    QList<Attachment *> attachments() const;

    void commitRotation();
    void commitScale();

    Bone *m_root;

    TargetMode m_targetMode;
    EditMode m_editMode;
    TransformMode m_transformMode;

    QGraphicsEllipseItem *m_circleItem;

    QPointF m_hotSpot;

    qreal m_oldRotation;
    qreal m_oldLength;
    QGraphicsItem *m_targetItem;

    QGraphicsEllipseItem *m_ellipseItem;
    QGraphicsEllipseItem *m_thickEllipseItem;
    QGraphicsLineItem *m_lineItem;
    QGraphicsLineItem *m_solidLineItem;

    Bone *m_targetBone;

    QMap<QGraphicsItem *, qreal> m_rotationBackup;
    QMap<QGraphicsItem *, qreal> m_scaleBackup;
};

#endif // VIEW_HPP
