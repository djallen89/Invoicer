#include "invoicer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Invoicer w;
    w.show();

    return a.exec();
}
