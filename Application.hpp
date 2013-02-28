#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QApplication>

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<Application *>(QCoreApplication::instance()))

class QUndoStack;

class Application : public QApplication
{
public:
    Application(int argc, char **argv);

    QUndoStack *undoStack() const;

private:
    QUndoStack *m_undoStack;
};

#endif // APPLICATION_HPP
