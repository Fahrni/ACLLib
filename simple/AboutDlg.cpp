#include "AboutDlg.h"
#include "ACLWinTypes.h"
#include "resource.h"
#include "CommCtrl.h"

static ACLMessageMap stcMsgMap[] =
	//	message			sub msg/ctl id		message handler method.
	//
	{ { WM_INITDIALOG,	WM_NULL,			(PMSGHNDLR)&AboutDlg::OnWMInit	}
	, { WM_DESTROY,		WM_NULL,			(PMSGHNDLR)&AboutDlg::OnDestroy	}
	, { WM_COMMAND,		IDOK,				(PMSGHNDLR)&AboutDlg::OnOk		}	
	};	//	stcMsgMap

//============================================================================
//  AboutDlg::AboutDlg(
//
//  Description of function's implementation issues
//============================================================================
AboutDlg::AboutDlg
(
	IN HINSTANCE		hInst,
    IN HWND             hWndParent
) :
	ACLWinDlg(hInst, (LPCTSTR)IDD_ABOUTBOX, hWndParent)
{
	AttachStaticMap(stcMsgMap, DLGMAPDIM(stcMsgMap));
}

//============================================================================
//  /*virtual*/ AboutDlg::~AboutDlg()
//
//  Description of function's implementation issues
//============================================================================
/*virtual*/ AboutDlg::~AboutDlg() throw()
{
}

//============================================================================
//  bool AboutDlg::OnWMInit()
//
//  Description of function's implementation issues
//============================================================================
bool AboutDlg::OnWMInit
(
	IN WPARAM wParam,
	IN LPARAM lParam
)
{
	::InitCommonControls();

	//	Center us over our parent.
	//
	Center();

	return false;
}	//	::OnWMInit

//============================================================================
//  bool AboutDlg::OnDestroy()
//
//  Description of function's implementation issues
//============================================================================
bool AboutDlg::OnDestroy
(
	IN WPARAM wParam,
	IN LPARAM lParam
)
{
	return true;
}	//	::OnDestroy


//============================================================================
//  bool AboutDlg::OnOk()
//
//  Description of function's implementation issues
//============================================================================
bool AboutDlg::OnOk
(
	IN WPARAM wParam,
	IN LPARAM lParam
)
{
	bool fOk(true);
	End(true);
	return fOk;
}	//	::OnOk
