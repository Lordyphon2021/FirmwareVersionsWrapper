#include "VersionWrapper.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VersionWrapper w;
    w.show();
    return a.exec();
}
