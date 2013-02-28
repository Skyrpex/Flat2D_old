#include "Application.hpp"
#include "MainWindow.hpp"
//#include <progressbar.h>
#include <QAction>

int main(int argc, char *argv[])
{
    Application a(argc, argv);

//    Manhattan::ProgressBar t;
//    t.setValue(40);
//    t.show();

    MainWindow w;
    w.show();
    return a.exec();
}
