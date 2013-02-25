#ifndef RESIZENODE_HPP
#define RESIZENODE_HPP

#include <QGraphicsEllipseItem>

class Bone;

class ResizeNode : public QGraphicsEllipseItem
{
public:
    ResizeNode(Bone *bone);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    Bone *m_bone;
};

#endif // RESIZENODE_HPP
