#include "application.h"
#include "utility.h"
#include <Windows.h>

#ifdef _DEBUG

#include "debugUtility.h"

#endif

using namespace SeatlesFramework;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
    Application* app = Application::instance();
    app->initialize();
    app->update();
    app->destroy();

#ifdef _DEBUG
    
    dumpMemoryLeakReport();

#endif

    return 0;
}



