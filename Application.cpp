#include "Application.hpp"

Application::Application(int argc, char **argv) :
    QApplication(argc, argv)
{
    setOrganizationName("Remacu");
    setApplicationName("Flat2D");
    setApplicationVersion("Alpha");
}
