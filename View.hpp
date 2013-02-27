#ifndef VIEW_HPP
#define VIEW_HPP

#include <QGraphicsView>

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
        TranslateTransformMode,
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
    void setTranslateTransformMode();
    void setRotateTransformMode();
    void setScaleTransformMode();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    Bone *topLevelBone(const QList<QGraphicsItem *> &items) const;
    QGraphicsItem *nearestItem(const QPointF &scenePos, const QList<QGraphicsItem *> &items) const;
    QGraphicsItem *targetItem(const QList<QGraphicsItem *> &items) const;

    QList<Bone *> bones() const;
    QList<Attachment *> attachments() const;

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
};

#endif // VIEW_HPP
