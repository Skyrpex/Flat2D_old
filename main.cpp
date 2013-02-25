#include "Application.hpp"
#include "View.hpp"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    View view;
    view.resize(800, 600);
    view.show();
    return a.exec();
}
