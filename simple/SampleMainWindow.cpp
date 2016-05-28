#include "Sample.h"
#include <iostream>
#include <string>
#include "ACLOutMsg.h"
#include "SampleMainWindow.h"
#include "CommCtrl.h"           // for common control support, InitCommonControls()
#include "resource.h"
#include "AboutDlg.h"
#include "ACLStr.h"
#include "ACLLib.h"
#include "ACLString.h"

#define STATUS_READY	"Ready"
#define STATUS_PART		0

// Helper for setting menut state.
#define SETMENUSTATE(f) ((false == f) ? MF_GRAYED : MF_ENABLED)
#define	SETMENUCHECK(f)	((false == f) ? MF_UNCHECKED : MF_CHECKED)

//	Static message map, not part of the class because it doesn't need to be.
//
//	The class will point at this however.
//
static ACLWinMessageMap stcMsgMap[] =
	//	message			        sub msg/ctl id          message handler method.
	//
	{ { WM_CREATE,	            WM_NULL,			    (PWINMSGHNDLR)&SampleMainWindow::OnCreate	        }
	, { WM_DESTROY,		        WM_NULL,			    (PWINMSGHNDLR)&SampleMainWindow::OnDestroy			}
	, { WM_COMMAND,		        IDM_ABOUT,			    (PWINMSGHNDLR)&SampleMainWindow::OnAbout			}	
	, { WM_COMMAND,		        IDM_STRING_TEST,		(PWINMSGHNDLR)&SampleMainWindow::OnStringTest       }
	, { WM_COMMAND,		        IDM_LOADLIB_TEST,		(PWINMSGHNDLR)&SampleMainWindow::OnLoadLibTest      } 
	, { WM_COMMAND,		        IDM_EXIT,			    (PWINMSGHNDLR)&SampleMainWindow::OnExit             }
	, { WM_SIZE,		        WM_NULL,			    (PWINMSGHNDLR)&SampleMainWindow::OnResize           }
	};	//	stcMsgMap

SampleMainWindow::SampleMainWindow
(
    IN HINSTANCE        hInstance
) :
    ACLWinBase(hInstance, NULL),
    _statusBar(NULL)
{
    // Attach our message map.
	AttachStaticMap(stcMsgMap, MAPDIM(stcMsgMap));
}

/*virtual*/
SampleMainWindow::~SampleMainWindow() throw()
{
    _Cleanup();
}

bool SampleMainWindow::Show(void)			
{
    bool fOk = true;

    if (NULL == ACLWinBase::GetHWND())
    {
        // Load the titlebar text.
        TCHAR szTitle[MAX_LOADSTRING];
	    ::LoadString(_hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
        
        // Create the window.
        RECT rc = { 20, 20, 350, 350 };
        fOk = ACLWinBase::Create(SAMPLE_MAIN_WINDOW_CLASS, &rc, szTitle, WS_OVERLAPPEDWINDOW,  0, NULL);
    }

    // If we have a valid window, show it.
    if (false != fOk &&
        NULL != GetHWND())
    {
        fOk = ACLWinBase::Show();
        ::UpdateWindow(_hWnd);
    }

    return fOk;
}

LRESULT SampleMainWindow::OnCreate
(
	IN WPARAM wParam,
	IN LPARAM lParam
)
{
    DBGPrintf(("SampleMainWindow::OnCreate()\n"));

	::InitCommonControls();

	//	Center us over our parent.
	//
	//Center();

    _CreateStatusBar();

    _SetStatusText(STATUS_READY);

	return 0L;
}	//	::OnCreate

LRESULT SampleMainWindow::OnDestroy
(
	IN WPARAM wParam,
	IN LPARAM lParam
)
{
    DBGPrintf(("SampleMainWindow::OnDestroy()\n"));

    // This will cleanup our internal data members, release
    // interfaces and destroy windows.
    _Cleanup();

    // Tell the application it's time to go away.
    ::PostQuitMessage(0);

    return 0L;
}	//	::OnDestroy

LRESULT SampleMainWindow::OnAbout
(
	IN WPARAM wParam,
	IN LPARAM lParam
)
{
    DBGPrintf(("SampleMainWindow::OnAbout()\n"));
    AboutDlg about(_hInstance, _hWnd);
    about.Show();

	return 0L;
}	//	::OnAbout

static void validate(ACLStr& source, LPCTSTR expected)
{
    if (source == expected)
    {
        DBGPrintf(("GOOD: \"%s\"\n", (LPTSTR)source));
    }
    else
    {
        DBGPrintf(("==> BAD:\n\tsource(\"%s\")\n\texpected(\"%s\")\n", (LPTSTR)source, expected));
    }
}

LRESULT SampleMainWindow::OnStringTest
(
	IN WPARAM wParam,
	IN LPARAM lParam
)
{
	{
		ACLStr str1("Hello, I'm a String, what are you?");
		DBGPrintf(("str1 = \"%s\"\n", (LPCTSTR)str1));
		str1.Append(" I was appended.");
		DBGPrintf(("str1 post append = \"%s\"\n", (LPCTSTR)str1));
		ACLStr str2("Hi, my name is str2");
		DBGPrintf(("str2 = \"%s\"\n", (LPCTSTR)str2));
		str1.Append(32); // space
		str1.Append(str2);
		str1.Append("what who what how what which");
		DBGPrintf(("str1 post append = \"%s\"\n", (LPCTSTR)str1));

	// Sub-string creation.
		ACLStr ss1(str2.Left(5));		// str2 first five = "Hi, my"
		DBGPrintf(("ss1 = \"%s\"\n", (LPCTSTR)ss1));
		ACLStr ss2(str2.Right(7));		// str2 last seven = "is str2"
		DBGPrintf(("ss2 = \"%s\"\n", (LPCTSTR)ss2));
		ACLStr ss3(str2.Mid(5));
		DBGPrintf(("ss3 = \"%s\"\n", (LPCTSTR)ss3));
		ACLStr ss4(str2.Mid(5, 10));	// Span from char five to char 10 (I believe?)
		DBGPrintf(("ss4 = \"%s\"\n", (LPCTSTR)ss4));

	// BAD sub-strings
		DBGPrintf((">> START: Bad sub-string\n"));
		ACLStr bss1(str2.Left(str2.Len()+10));
		DBGPrintf(("bss1 = \"%s\"\n", (LPCTSTR)bss1));
		ACLStr bss2(str2.Right(400));
		DBGPrintf(("bss2 = \"%s\"\n", (LPCTSTR)bss2));
		ACLStr bss3(str2.Left(0));
		DBGPrintf(("bss3 = \"%s\"\n", (LPCTSTR)bss3));
		DBGPrintf(("<< END: Bad sub-string\n"));

	// Find
		int f1 = str2.Find('m');
		DBGPrintf(("f1 location = %d in \"%s\"\n", f1, (LPCTSTR)str2));
		ACLStr f("was");
		int f2 = str2.Find(f);
		DBGPrintf(("f2 location = %d in \"%s\"\n", f2, (LPCTSTR)str2));
		int f3 = str2.Find('H', 0);
		DBGPrintf(("f3 location = %d in \"%s\"\n", f3, (LPCTSTR)str2));

	// Strip
		ACLStr st0("what");
		ACLStr st1(str1.StripWord(st0));
		DBGPrintf(("st1 is %s after strip of \"%s\"\n", (LPCTSTR)st1, (LPCTSTR)st0));

	// Trim
		ACLStr ts0("   string woo!    ");
		ts0.RTrim();
		DBGPrintf(("ts0 after RTrim \"%s\"\n", (LPCTSTR)ts0));
		ACLStr ts1("   string woo!    ");
		ts1.LTrim();
		DBGPrintf(("ts1 after LTrim \"%s\"\n", (LPCTSTR)ts1));
		ACLStr ts2("   string woo!    ");
		ts2.Trim();	
		DBGPrintf(("ts2 after Trim \"%s\"\n", (LPCTSTR)ts2));

	// Make Upper/Lower
		str1.MakeUpper();
		DBGPrintf(("str1 MakeUpper \"%s\"\n", (LPCTSTR)str1));
		str1.MakeLower();
		DBGPrintf(("str1 MakeLower \"%s\"\n", (LPCTSTR)str1));
	}

	// Newer tests, copied from Mac test app...
	
	// Grow/Shrink Buffer
	{
		DBGPrintf(("==> Test Grow/Shrink Buffer <==\n"));
		ACLStr gs("Hello"); validate(gs, "Hello");
		gs.Append(" Rob"); validate(gs, "Hello Rob");
		gs.Append(" 45"); validate(gs, "Hello Rob 45");
		gs.Append(" you are an old guy"); validate(gs, "Hello Rob 45 you are an old guy");
		gs = "Blah"; validate(gs, "Blah");
		gs.Append(" Rob"); validate(gs, "Blah Rob");
		gs += " you're a big stinker"; validate(gs, "Blah Rob you're a big stinker");

		DBGPrintf(("\n==> Append Different Types <==\n"));
		ACLStr str1("Hello, I'm a String, what are you?"); validate(str1, "Hello, I'm a String, what are you?");
		str1.Append(" I was appended."); validate(str1, "Hello, I'm a String, what are you? I was appended.");
		ACLStr str2("Hi, my name is str2"); validate(str2, "Hi, my name is str2");
		str1.Append(32); validate(str1, "Hello, I'm a String, what are you? I was appended. ");
		str1.Append(str2); validate(str1, "Hello, I'm a String, what are you? I was appended. Hi, my name is str2");
		str1.Append(" what who what how what which"); validate(str1, "Hello, I'm a String, what are you? I was appended. Hi, my name is str2 what who what how what which");
    
		// Sub-string creation.
		DBGPrintf(("\n==> Sub-String Creation <==\n"));
		ACLStr src("0123456789a123456789b123456789c");
		ACLStr ss1(src.Left(5)); validate(ss1, "01234");
		ACLStr ss2(src.Right(7)); validate(ss2, "456789c");
		ACLStr ss3(src.Mid(5)); validate(ss3, "56789a123456789b123456789c");
		ACLStr ss4(src.Mid(5, 10)); validate(ss4, "56789a1234");
    
		// Strip
		src = "Who what when when why what";
		ACLStr st0("what"); ACLStr stripped(src.StripWord(st0)); validate(stripped, "Who  when when why ");
		src = "Whatwhatwhat1";
		stripped = src.StripWord(st0); validate(stripped, "What1");
		src = "Whatwhatwhat1what2what3";
		stripped = src.StripWord(st0); validate(stripped, "What123");
		stripped = src.StripWord("hat"); validate(stripped, "Www1w2w3");
		stripped = src.StripWord("1"); validate(stripped, "Whatwhatwhatwhat2what3");
		stripped = src.StripWord("2"); validate(stripped, "Whatwhatwhat1whatwhat3");
		stripped = src.StripWord("3"); validate(stripped, "Whatwhatwhat1what2what");
		src = "HELLOhelloHello";
		stripped = src.StripWord("hello"); validate(stripped, "HELLOHello");
    
		// Trim
		DBGPrintf(("\n=> Trim Test <=\n"));
		ACLStr ts0("   string woo!    ");
		ts0.RTrim(); validate(ts0, "   string woo!");
		ACLStr ts1("   string woo!    ");
		ts1.LTrim(); validate(ts1, "string woo!    ");    
		ACLStr ts2("   string woo!    ");
		ts2.Trim();	validate(ts2, "string woo!");
    
		// Make Upper/Lower
		DBGPrintf(("\n=> Make Upper/Lower Test <=\n"));
		ACLStr basic("Hello, I'm a basic string");
		basic.MakeUpper(); validate(basic, "HELLO, I'M A BASIC STRING");
		basic = "Hello, I'm a basic string";
		basic.MakeLower(); validate(basic, "hello, i'm a basic string");

		// BAD sub-strings
		DBGPrintf(("\n=> START: Bad sub-string\n"));
		ACLStr bss1(str2.Left(str2.Len()+10));
		DBGPrintf(("bss1 = \"%s\"\n", (LPCTSTR)bss1));
		ACLStr bss2(str2.Right(400));
		DBGPrintf(("bss2 = \"%s\"\n", (LPCTSTR)bss2));
		ACLStr bss3(str2.Left(0));
		DBGPrintf(("bss3 = \"%s\"\n", (LPCTSTR)bss3));
		DBGPrintf(("<< END: Bad sub-string\n"));
    
		// Find
		int f1 = str2.Find('m');
		DBGPrintf(("f1 location = %d in \"%s\"\n", f1, (LPCTSTR)str2));
		ACLStr f("was");
		int f2 = str2.Find(f);
		DBGPrintf(("f2 location = %d in \"%s\"\n", f2, (LPCTSTR)str2));
		int f3 = str2.Find('H', 0);
		DBGPrintf(("f3 location = %d in \"%s\"\n", f3, (LPCTSTR)str2));
	}
//    printf("\n=> Make Upper/Lower Test <=\n");
//    f.FormatInternal("Internal format test: %d", 1); validate(f, "Internal format test: 1");
//    f.FormatInternal("Internal format test: %s", "One"); validate(f, "Internal format test: One");
//    ACLStr ff(ACLStr::Format("External format test: %d", 99)); validate(ff, "Internal format test: 99");
//    ACLStr ff2(ACLStr::Format("External format test: %s", "NineNine")); validate(ff2, "Internal format test: NineNine");
	{
		ACLString string1("String #1");
		DBGPrintf(("string1 = \"%s\"\n", (LPCTSTR)string1));
		ACLString string2(string1);
		DBGPrintf(("string2 = \"%s\"\n", (LPCTSTR)string2));
		ACLString string3(string2);
		DBGPrintf(("string3 = \"%s\"\n", (LPCTSTR)string3));

		string2 += " wacka, wacka, wacka";
		DBGPrintf(("string2 POST change = \"%s\"\n", (LPCTSTR)string2));

		ACLString string4("String 4, completey new");
		DBGPrintf(("string4 = \"%s\"\n", (LPCTSTR)string4));
		string4 += " wacka, wacka, WACKA";
		DBGPrintf(("string4 POST change = \"%s\"\n", (LPCTSTR)string4));
	}
	return 0L;
}

LRESULT SampleMainWindow::OnLoadLibTest
(
	IN WPARAM wParam,
	IN LPARAM lParam
)
{
	#define TEST_LIB			"comctl32.dll"
	#define TEST_LIB_STRING_ID	5
	
	// Load the library
	ACLLib library(TEST_LIB);
	library.Load();

	// Load a string from the libraries string table.
	STRING string = library.LoadStringA(TEST_LIB_STRING_ID);
	DBGPrintf(("Loaded %s from library %s\n", string, TEST_LIB));

	// Unload the library
	library.Unload();

	return 0L;
}

LRESULT SampleMainWindow::OnExit
(
	IN WPARAM wParam,
	IN LPARAM lParam
)
{
    DBGPrintf(("SampleMainWindow::OnExit()\n"));

    // Calling Destroy() on our base window will cause
    // it to be killed off, which in turn will send
    // a WM_DESTROY, which we answer in OnDestroy.
    ACLWinBase::Destroy();

	return 0L;
}	//	::OnExit


LRESULT SampleMainWindow::OnResize
(
    IN WPARAM wParam,
    IN LPARAM lParam
)
{
    DBGPrintf(("SampleMainWindow::OnResize()\n"));

    // Tell the status bar to resize itself.
    ::SendMessage(_statusBar->GetHWND(), WM_SIZE, wParam, lParam);

    // Calculate our client rect, we need to account for the
    // status bar now.
    RECT rc;
    _CalculateClientRect(rc);

    return 0;
}

LRESULT SampleMainWindow::OnPaint
(
    IN WPARAM wParam,
    IN LPARAM lParam
)
{
    PAINTSTRUCT ps;
    HDC         hdc = ::BeginPaint(_hWnd, &ps);
    // TODO: Add any drawing code here...
    ::EndPaint(_hWnd, &ps);

    DBGPrintf(("SampleMainWindow::OnPaint()\n"));

    return 0;
}

bool SampleMainWindow::_CreateStatusBar(void)
{
    bool fOk = false;
    if (NULL == _statusBar)
    {
        _statusBar = new StatusBar(_hInstance, this->GetHWND(), this);
        if (NULL != _statusBar)
        {
            fOk = _statusBar->Show();
        }
    }

    return fOk;
}

void SampleMainWindow::_Cleanup(void)
{
    if (NULL != _statusBar)
    {
        _statusBar->Destroy();
    }

    delete _statusBar;
    _statusBar = NULL;
}

void SampleMainWindow::_SetMenuItemCheck
(
	IN UINT		menuId, 
	IN bool	fCheck
)
{
	HMENU hMenu = ::GetMenu(_hWnd);
	if (NULL != hMenu)
	{
		CheckMenuItem(hMenu, menuId, SETMENUCHECK(fCheck));
	}
}

void SampleMainWindow::_CalculateClientRect(RECT& clientRect)
{
    // Get the size of the status bar, we're only interested 
    // in the height.
    RECT rc;
    _statusBar->GetWindowRect(&rc);

    // Get our client rectangle.
    GetClientRect(&clientRect);

    // Adjust the rectangle, clientRect, by subtracting the height of rc.
    clientRect.bottom -= (rc.bottom - rc.top);
}

void SampleMainWindow::_SetStatusText(IN char* text)
{
    if (NULL != _statusBar)
    {
        // Set the status bar, status part, to "Ready"
        _statusBar->SetPartText(STATUS_PART, text);
    }
    else
    {
        DBGPrintf(("Status Bar is NULL, cannot set message\n"));
    }
}
