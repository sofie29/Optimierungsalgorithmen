#include "Optimierungsalgorithmen.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    std::cout << "hdoh\n";

    QApplication a(argc, argv);
    Optimierungsalgorithmen w;
    w.show();
    return a.exec();
}
