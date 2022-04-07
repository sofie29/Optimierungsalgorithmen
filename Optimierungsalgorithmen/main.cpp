#include "Optimierungsalgorithmen.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "TestEnvironment.h"
int main(int argc, char *argv[])
{
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
   
    QApplication a(argc, argv);
    if (UIConstants::useUI_) {
        
        Optimierungsalgorithmen w;
        w.show();
        return a.exec();
    }
    else {
        std::unique_ptr<TestEnvironment> test = std::make_unique<TestEnvironment>(250, 75);
        test->Run("FastEx.csv");
        while (true);
    }
}

void* operator new(size_t size) {



    
    //std::cout << "alloc " << size << std::endl;

    return malloc(size);
}

void operator delete(void* memory, size_t size) {


  
   // std::cout << "dealloc " << size << std::endl;

    free(memory);
}