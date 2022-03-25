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

void* operator new(size_t size) {



    
    //std::cout << "alloc " << size << std::endl;

    return malloc(size);
}

void operator delete(void* memory, size_t size) {


  
   // std::cout << "dealloc " << size << std::endl;

    free(memory);
}