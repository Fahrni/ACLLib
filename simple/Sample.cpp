// Sample.cpp : Sampling of ACLWin Library Classes.
//
#include "Sample.h"
#include "SampleApp.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    // Create an Instance of Sample, Initialize(), and Run()
    SampleApp app;
    app.InitInstance(hInstance, nCmdShow);
    return app.Run();
}
