#include "Application.hpp"
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
