#include "nwnchargen.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NwnCharGen w;
    w.show();
    return a.exec();
}
