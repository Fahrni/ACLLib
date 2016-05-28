#include "Sample.h"
#include "SampleApp.h"
#include "resource.h"
#include "ACLWinBase.h"
#include "ACLOutMsg.h"

SampleApp::SampleApp() :
    ACLWinApp(),
    _atomMain(NULL)
{
}

SampleApp::~SampleApp() throw()
{
}

bool SampleApp::InitInstance
(
    IN HINSTANCE    hInstance, 
    IN int       nCmdShow
)
{
    // Store the instance handle for later use.
    _hInstance      = hInstance;

    // Load our accelerator table, used in message loop of Run() method.
    _hAccelTable    = ::LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ACLWIN));

    // Register the window classes we'll use to create all
    // Remy based windows. Constants for each class of window
    // can be found in RemyConst.h.
    bool fOk = _RegisterWindowClasses();
    if (false != fOk)
    {
        fOk = _CreateMainWindow();
    }
    ELSEIF_DBGPrintf(("Failed to Register Window Classes\n"));

    return fOk;
}

int SampleApp::CleanupInstance(void)
{
    int result = 0;
    
    delete _sampleMainWindow;
    _sampleMainWindow = NULL;

    return result;
}

bool SampleApp::_RegisterWindowClasses(void)
{
    bool     fOk = false;

	WNDCLASSEX  wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	// Register the Main Window
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= ACLWindowDispatchProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= sizeof(UINT);
	wcex.hInstance		= _hInstance;
	wcex.hIcon			= LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_ACLWIN));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ACLWIN);
	wcex.lpszClassName	= SAMPLE_MAIN_WINDOW_CLASS;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    _atomMain = RegisterClassEx(&wcex);

    return (NULL != _atomMain);
}

bool SampleApp::_CreateMainWindow(void)
{
    bool fOk = false;

    // Construct our main window.
    _sampleMainWindow = new SampleMainWindow(_hInstance);
    if (NULL != _sampleMainWindow)
    {
        //  Show the window
        fOk = _sampleMainWindow->Show();
    }
    ELSEIF_DBGPrintf(("Failed to construct SampleMainWindow()\n"));

    return fOk;
}

