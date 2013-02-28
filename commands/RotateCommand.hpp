#ifndef ROTATECOMMAND_HPP
#define ROTATECOMMAND_HPP

#include <QUndoCommand>

class QGraphicsItem;

class RotateCommand : public QUndoCommand
{
    Q_OBJECT
public:
    explicit RotateCommand(QGraphicsItem *item, qreal oldRotation, qreal newRotation, QUndoCommand  *parent = 0);
    
    void redo();

private:
    QGraphicsItem *m_item;
    qreal m_oldRotation;
    qreal m_newRotation;
};

#endif // ROTATECOMMAND_HPP
