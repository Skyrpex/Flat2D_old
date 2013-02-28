#include "Application.hpp"
#include <QUndoStack>

Application::Application(int argc, char **argv) :
    QApplication(argc, argv),
    m_undoStack(new QUndoStack(this))
{
    setOrganizationName("Remacu");
    setApplicationName("Flat2D");
    setApplicationVersion("Alpha");
}

QUndoStack *Application::undoStack() const
{
    return m_undoStack;
}
