#ifndef ATTACHMENT_HPP
#define ATTACHMENT_HPP

#include <QGraphicsPixmapItem>

class Bone;

class Attachment : public QGraphicsPixmapItem
{
public:
    Attachment(const QPixmap &pixmap);

    Bone *bone() const;
    void setBone(Bone *bone);

    QPointF localPos() const;
    qreal localRotation() const;
    qreal localScale() const;

    void setLocalPos(const QPointF &pos);
    void setLocalRotation(qreal rotation);
    void setLocalScale(qreal scale);

private:
    Bone *m_bone;

    QPointF m_localPos;
    qreal m_localRotation;
    qreal m_localScale;
};

#endif // ATTACHMENT_HPP
