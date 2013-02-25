#include "View.hpp"
#include "Bone.hpp"
#include "Attachment.hpp"
#include "Application.hpp"
#include "ResizeNode.hpp"
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QTimer>
#include <QStack>
#include <QDebug>

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
    m_lineItem(new QGraphicsLineItem)
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

    m_root->addResizeNodesIntoScene();


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

    //
    setViewportUpdateMode(NoViewportUpdate);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), viewport(), SLOT(update()));
    timer->start(1.0/60.0);
}

void View::setBoneTargetMode()
{
    m_targetMode = BoneTargetMode;

    m_root->setOpacity(1);
    foreach(Bone *bone, bones()) {
        bone->setFlag(QGraphicsItem::ItemIsSelectable, true);
        bone->setFlag(QGraphicsItem::ItemIsMovable, true);
    }

    setResizeNodesVisible(true);

    m_root->mapAttachmentsFromScene();
    foreach(Attachment *attachment, attachments()) {
        attachment->setOpacity(0.5);
        attachment->setEnabled(false);
    }
}

void View::setAttachmentTargetMode()
{
    m_targetMode = AttachmentTargetMode;

    m_root->setOpacity(0.5);
    foreach(Bone *bone, bones()) {
        bone->setFlag(QGraphicsItem::ItemIsSelectable, false);
        bone->setFlag(QGraphicsItem::ItemIsMovable, false);
    }

    setResizeNodesVisible(false);

    foreach(Attachment *attachment, attachments()) {
        attachment->setOpacity(1);
        attachment->setEnabled(true);
    }
}

void View::setTransformEditMode()
{
    m_editMode = TransformEditMode;

    setDragMode(RubberBandDrag);
}

void View::setCreateEditMode()
{
    m_editMode = CreateEditMode;

    setSelectTransformMode();
    setBoneTargetMode();
    setDragMode(NoDrag);

    if(scene()->selectedItems().count() > 1) {
        scene()->clearSelection();
    }
}

void View::setSelectTransformMode()
{
    m_transformMode = SelectTransformMode;

    foreach(Bone *bone, bones()) {
        bone->setFlag(QGraphicsItem::ItemIsMovable, true);
    }
    foreach(ResizeNode *resizeNode, resizeNodes()) {
        resizeNode->setVisible(false);
    }
}

void View::setTranslateTransformMode()
{
    m_transformMode = TranslateTransformMode;
}

void View::setRotateTransformMode()
{
    m_transformMode = RotateTransformMode;

    m_targetItem = targetItem(scene()->selectedItems());

    m_ellipseItem->setVisible(m_targetItem);
    if(m_targetItem) {
        m_ellipseItem->setPos(m_targetItem->scenePos());
    }
}

void View::setScaleTransformMode()
{
    m_transformMode = ScaleTransformMode;

    m_targetItem = targetItem(scene()->selectedItems());
//    m_ellipseItem->setVisible(m_targetItem);
//    if(m_targetItem) {
//        m_thickEllipseItem->setPos(m_targetItem->scenePos());
//        m_ellipseItem->setPos(m_targetItem->scenePos());
//    }

    foreach(Bone *bone, bones()) {
        bone->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
    foreach(ResizeNode *resizeNode, resizeNodes()) {
        resizeNode->setVisible(true);
    }
}

void View::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat()) {
        return;
    }

    if(event->modifiers() & Qt::ControlModifier) {
        switch(event->key()) {
        case TransformKey:
            setTransformEditMode();
            break;

        case CreateKey:
            setCreateEditMode();
            break;
        }
    }
    else {
        if(m_editMode != CreateEditMode) {
            switch(event->key()) {
            case TranslateKey:
                setTranslateTransformMode();
                break;

            case RotateKey:
                setRotateTransformMode();
                break;

            case ScaleKey:
                setScaleTransformMode();
                break;
            }
        }
    }
}

void View::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat()) {
        return;
    }

    if(m_editMode != CreateEditMode) {
        switch(event->key()) {
        case TranslateKey:
        case RotateKey:
        case ScaleKey:
            setSelectTransformMode();
            break;
        }
    }

    m_ellipseItem->setVisible(false);
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
            m_lineItem->setVisible(m_targetItem);
            if(m_targetItem) {
                QLineF line(m_targetItem->scenePos(), mapToScene(event->pos()));
                m_lineItem->setLine(line);
                m_oldRotation = line.angle();
            }
            break;
        }

        case ScaleTransformMode: {
            /*m_lineItem->setVisible(m_targetItem);
            if(m_targetItem) {
                QLineF line(m_targetItem->scenePos(), mapToScene(event->pos()));
                m_lineItem->setLine(line);
                m_oldLength = line.length();

                m_thickEllipseItem->setVisible(true);
                qreal size = m_oldLength*2;
                m_thickEllipseItem->setRect(-m_oldLength, -m_oldLength, size, size);
            }*/


//            QGraphicsView::mousePressEvent(event);

            if(m_targetItem) {
                QLineF line(m_targetItem->scenePos(), mapToScene(event->pos()));
                m_oldLength = line.length();
            }
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
    m_root->mapResizeNodesToScene();
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

QList<ResizeNode *> View::resizeNodes() const
{
    QList<ResizeNode *> resizeNodes;

    QStack<Bone *> bones;
    bones << m_root;
    while(!bones.isEmpty()) {
        Bone *bone = bones.pop();

        resizeNodes << bone->resizeNode();

        foreach(Bone *child, bone->childBones()) {
            bones.push(child);
        }
    }

    return resizeNodes;
}

void View::setResizeNodesVisible(bool visible) const
{
    foreach(ResizeNode *resizeNode, resizeNodes()) {
        resizeNode->setVisible(visible);
    }
}
