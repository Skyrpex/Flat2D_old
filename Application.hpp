#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QApplication>

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<Application *>(QCoreApplication::instance()))

class Application : public QApplication
{
public:
    Application(int argc, char **argv);
};

#endif // APPLICATION_HPP
