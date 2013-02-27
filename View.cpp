#include "View.hpp"
#include "Bone.hpp"
#include "Attachment.hpp"
#include "Application.hpp"
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QTimer>
#include <QStack>
#include <QDebug>

// Update interval: 60 fps
static const int UpdateInterval = 1000 / 60;

static const char * const SelectEditModeText = "Select mode";
static const char * const TranslateEditModeText = "Translate mode";
static const char * const RotateEditModeText = "Rotate mode";
static const char * const ScaleEditModeText = "Scale mode";

static const int TransformKey = Qt::Key_1;
static const int CreateKey = Qt::Key_2;

static const int TranslateKey = Qt::Key_T;
static const int RotateKey = Qt::Key_R;
static const int ScaleKey = Qt::Key_S;

View::View(QWidget *parent) :
    QGraphicsView(new QGraphicsScene, parent),
    m_root(new Bone("Root")),
    m_targetMode(BoneTargetMode),
    m_ellipseItem(new QGraphicsEllipseItem(-10, -10, 20, 20)),
    m_thickEllipseItem(new QGraphicsEllipseItem(-10, -10, 20, 20)),
    m_lineItem(new QGraphicsLineItem),
    m_solidLineItem(new QGraphicsLineItem)
{
    setSceneRect(-512, -400, 1024, 800);
    setRenderHints(QPainter::Antialiasing);
    setBackgroundBrush(Qt::darkGray);


    Bone *bone = new Bone("Bone 2", m_root);
    bone->setScaleFromLength(100);

    Attachment *attachment1 = new Attachment(QPixmap(":/images/head.png"));
    m_root->addAttachment(attachment1);

    Attachment *attachment2 = new Attachment(QPixmap(":/images/torso.png"));
    bone->addAttachment(attachment2);

    scene()->addItem(m_root);
    scene()->addItem(attachment1);
    scene()->addItem(attachment2);

    setBoneTargetMode();
    setTransformEditMode();
    setSelectTransformMode();

    m_circleItem = new QGraphicsEllipseItem(-10, -10, 20, 20);
    m_circleItem->setVisible(false);
    scene()->addItem(m_circleItem);

    m_ellipseItem->setVisible(false);
    scene()->addItem(m_ellipseItem);

    m_thickEllipseItem->setVisible(false);
    m_thickEllipseItem->setPen(QPen(Qt::black, 2));
    scene()->addItem(m_thickEllipseItem);

    m_lineItem->setPen(QPen(Qt::black, 0, Qt::DashLine));
    m_lineItem->setVisible(false);
    scene()->addItem(m_lineItem);

    m_solidLineItem->setPen(QPen(Qt::black, 0, Qt::SolidLine));
    m_solidLineItem->setVisible(false);
    scene()->addItem(m_solidLineItem);

    //
    setViewportUpdateMode(NoViewportUpdate);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), viewport(), SLOT(update()));
    timer->start(UpdateInterval);
}

void View::setBoneTargetMode()
{
    m_targetMode = BoneTargetMode;

    m_root->setOpacity(1);
    foreach(Bone *bone, bones()) {
        bone->setFlag(QGraphicsItem::ItemIsSelectable, true);
        bone->setFlag(QGraphicsItem::ItemIsMovable, true);
    }

    m_root->mapAttachmentsFromScene();
    foreach(Attachment *attachment, attachments()) {
        attachment->setOpacity(0.6);
        attachment->setEnabled(false);
    }
}

void View::setAttachmentTargetMode()
{
    m_targetMode = AttachmentTargetMode;

    m_root->setOpacity(0.6);
    foreach(Bone *bone, bones()) {
        bone->setFlag(QGraphicsItem::ItemIsSelectable, false);
        bone->setFlag(QGraphicsItem::ItemIsMovable, false);
    }

    foreach(Attachment *attachment, attachments()) {
        attachment->setOpacity(1);
        attachment->setEnabled(true);
    }
}

void View::setTransformEditMode()
{
    m_editMode = TransformEditMode;

    setDragMode(RubberBandDrag);

    m_ellipseItem->setVisible(false);
    m_solidLineItem->setVisible(false);
}

void View::setCreateEditMode()
{
    setSelectTransformMode();

    m_editMode = CreateEditMode;

    setBoneTargetMode();
    setDragMode(NoDrag);

    if(scene()->selectedItems().count() > 1) {
        scene()->clearSelection();
    }


    m_ellipseItem->setVisible(false);
    m_solidLineItem->setVisible(false);
}

void View::setSelectTransformMode()
{
    setTransformEditMode();

    m_transformMode = SelectTransformMode;

    m_ellipseItem->setVisible(false);
    m_solidLineItem->setVisible(false);
}

void View::setTranslateTransformMode()
{
    setTransformEditMode();

    m_transformMode = TranslateTransformMode;

    m_ellipseItem->setVisible(false);
    m_solidLineItem->setVisible(false);
}

void View::setRotateTransformMode()
{
    setTransformEditMode();

    m_transformMode = RotateTransformMode;

    m_targetItem = targetItem(scene()->selectedItems());

    m_ellipseItem->setVisible(m_targetItem);
    if(m_targetItem) {
        m_ellipseItem->setPos(m_targetItem->scenePos());
    }

    m_solidLineItem->setVisible(false);
}

void View::setScaleTransformMode()
{
    setTransformEditMode();

    m_transformMode = ScaleTransformMode;

    m_targetItem = targetItem(scene()->selectedItems());
    m_ellipseItem->setVisible(m_targetItem);
    if(m_targetItem) {
        m_thickEllipseItem->setPos(m_targetItem->scenePos());
        m_ellipseItem->setPos(m_targetItem->scenePos());
    }

    m_ellipseItem->setVisible(false);
}

void View::keyPressEvent(QKeyEvent *event)
{
    if(m_editMode == TransformEditMode) {
        switch(event->key()) {
//        case TranslateKey:
//            setTranslateTransformMode();
//            break;

        case RotateKey:
            setRotateTransformMode();
            break;

        case ScaleKey:
            setScaleTransformMode();
            break;

        default:
            break;
        }
    }
}

void View::keyReleaseEvent(QKeyEvent *event)
{
    if(m_editMode == TransformEditMode) {
        switch(event->key()) {
//        case TranslateKey:
        case RotateKey:
        case ScaleKey:
            setSelectTransformMode();
            break;

        default:
            break;
        }
    }
}

void View::mousePressEvent(QMouseEvent *event)
{
    if(m_editMode == TransformEditMode) {
        switch(m_transformMode) {
        case SelectTransformMode:  {
            QGraphicsItem *item = itemAt(event->pos());
            if(m_targetMode == BoneTargetMode && dynamic_cast<Attachment *>(item)) {
                setAttachmentTargetMode();
            }
            else if(m_targetMode == AttachmentTargetMode && dynamic_cast<Bone *>(item)) {
                setBoneTargetMode();
            }
            QGraphicsView::mousePressEvent(event);
            break;
        }

        case TranslateTransformMode:
            m_hotSpot = mapToScene(event->pos());
            break;

        case RotateTransformMode: {
            if(m_targetItem) {
                QLineF line(m_targetItem->scenePos(), mapToScene(event->pos()));
                m_lineItem->setLine(line);
                m_solidLineItem->setLine(line);

                m_oldRotation = line.angle();
            }
            m_lineItem->setVisible(m_targetItem);
            m_solidLineItem->setVisible(m_targetItem);
            break;
        }

        case ScaleTransformMode: {
            if(m_targetItem) {
                QLineF line(m_targetItem->scenePos(), mapToScene(event->pos()));
                m_lineItem->setLine(line);
                m_oldLength = line.length();

                m_thickEllipseItem->setVisible(true);
                qreal size = m_oldLength*2;
                m_thickEllipseItem->setRect(-m_oldLength, -m_oldLength, size, size);
            }
            m_lineItem->setVisible(m_targetItem);
            break;
        }
        }
    }
    else if(m_editMode == CreateEditMode) {
        QGraphicsItem *item = itemAt(event->pos());
        Bone *bone = dynamic_cast<Bone *>(item);
        if(bone) {
            scene()->clearSelection();
            bone->setSelected(true);
            m_targetBone = 0;
        }
        else {

            Bone *parent = 0;
            QList<QGraphicsItem *> selectedItems = scene()->selectedItems();
            if(selectedItems.count() == 1) {
                parent = dynamic_cast<Bone *>(selectedItems.first());
            }

            if(parent) {
                m_targetBone = new Bone("New", parent);

                QPointF scenePos = mapToScene(event->pos());
                m_targetBone->setScenePos(scenePos);
                m_targetBone->setScaleFromLength(0);

                scene()->clearSelection();
                m_targetBone->setSelected(true);
            }
            else {
                m_targetBone = 0;
            }
        }
    }
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    if(m_editMode == TransformEditMode) {
        if(event->buttons() & Qt::LeftButton) {
            switch(m_transformMode) {
            case SelectTransformMode:
                QGraphicsView::mouseMoveEvent(event);
                break;

            case TranslateTransformMode: {
                QPointF scenePos = mapToScene(event->pos());
                QPointF offset = scenePos - m_hotSpot;

                foreach(QGraphicsItem *item, scene()->selectedItems()) {
                    item->moveBy(offset.x(), offset.y());
                }

                m_hotSpot = scenePos;
                break;
            }

            case RotateTransformMode: {
                if(m_targetItem) {
                    QPointF scenePos = mapToScene(event->pos());
                    QLineF line(m_targetItem->scenePos(), scenePos);
                    m_lineItem->setLine(line);

                    qreal offset = m_oldRotation - line.angle();

                    QList<QGraphicsItem*> selectedItems = scene()->selectedItems();
                    foreach(QGraphicsItem *item, selectedItems) {
                        item->setRotation(item->rotation() + offset);
                    }

                    m_oldRotation = line.angle();
                }
                break;
            }

            case ScaleTransformMode: {
//                QGraphicsView::mouseMoveEvent(event);

                /*QPointF scenePos = mapToScene(event->pos());
                QPointF offset = scenePos - m_hotSpot;

                foreach(QGraphicsItem *item, scene()->selectedItems()) {
                    item->moveBy(offset.x(), offset.y());
                }

                m_hotSpot = scenePos;*/


                if(m_targetItem) {
                    QPointF scenePos = mapToScene(event->pos());
                    QLineF line(m_targetItem->scenePos(), scenePos);
                    m_lineItem->setLine(line);

                    qreal factor = line.length() / m_oldLength;
//                    qDebug() << factor;

                    foreach(QGraphicsItem *item, scene()->selectedItems()) {
                        item->setScale(item->scale() * factor);
                    }

                    m_oldLength = line.length();
                }
                break;
            }
            }
        }
    }
    else {
        if(event->buttons() & Qt::LeftButton) {
            if(m_targetBone) {
                QPointF scenePos = mapToScene(event->pos());
                QLineF line(m_targetBone->scenePos(), scenePos);

                m_targetBone->setSceneRotation(-line.angle());

                m_targetBone->setScaleFromSceneLength(line.length());
            }
        }
    }
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    m_lineItem->setVisible(false);
    m_thickEllipseItem->setVisible(false);
    QGraphicsView::mouseReleaseEvent(event);
}

void View::paintEvent(QPaintEvent *event)
{
    if(m_targetMode == BoneTargetMode) {
        m_root->mapAttachmentsToScene();
    }
    QGraphicsView::paintEvent(event);
}

Bone *View::topLevelBone(const QList<QGraphicsItem *> &items) const
{
    Bone *topBone = 0;
    int topLevel = -1;

    foreach(QGraphicsItem *item, items) {
        Bone *bone = dynamic_cast<Bone *>(item);
        if(!bone) {
            continue;
        }

        int level = 0;
        Bone *parent = bone->parentBone();
        while(parent) {
            ++level;
            parent = parent->parentBone();
        }

        if((topBone && level < topLevel) || !topBone) {
            topBone = bone;
            topLevel = level;
        }
    }

    return topBone;
}

QGraphicsItem *View::nearestItem(const QPointF &scenePos, const QList<QGraphicsItem *> &items) const
{
    QGraphicsItem *nearestItem = 0;
    qreal nearestDistance = 0;
    foreach(QGraphicsItem *item, items) {
        qreal distance = QLineF(scenePos, item->scenePos()).length();
        if((nearestItem && distance < nearestDistance) || !nearestItem) {
            nearestItem = item;
            nearestDistance = distance;
        }
    }
    return nearestItem;
}

QGraphicsItem *View::targetItem(const QList<QGraphicsItem *> &items) const
{
    QGraphicsItem *item = topLevelBone(items);
    if(!item) {
        QPointF scenePos = mapToScene(mapFromGlobal(QCursor::pos()));
        item = nearestItem(scenePos, items);
    }

    return item;
}

QList<Bone *> View::bones() const
{
    QList<Bone *> bones;

    QStack<Bone *> stack;
    stack << m_root;
    while(!stack.isEmpty()) {
        Bone *bone = stack.pop();

        bones << bone;

        foreach(Bone *child, bone->childBones()) {
            stack.push(child);
        }
    }

    return bones;
}


QList<Attachment *> View::attachments() const
{
    QList<Attachment *> attachments;

    QStack<Bone *> bones;
    bones << m_root;
    while(!bones.isEmpty()) {
        Bone *bone = bones.pop();

        attachments << bone->attachments();

        foreach(Bone *child, bone->childBones()) {
            bones.push(child);
        }
    }

    return attachments;
}
