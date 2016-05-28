#include "ACLOutMsg.h"
#include "StatusBar.h"
#include "CommCtrl.h"           // for common control support, InitCommonControls()
#include "resource.h"

//	Static message map, not part of the class because it doesn't need to be.
static ACLWinMessageMap stcMsgMap[] =
	//	message			sub msg/ctl id		message handler method.
	//
	{ { WM_CREATE,	    WM_NULL,			(PWINMSGHNDLR)&StatusBar::OnCreate	}
	, { WM_DESTROY,		WM_NULL,			(PWINMSGHNDLR)&StatusBar::OnDestroy	}
	};	//	stcMsgMap

StatusBar::StatusBar
(
    IN HINSTANCE    hInstance,
    IN HWND         hWndParent,
    IN ACLWinBase*  pParentWindow
) :
    ACLWinBase(hInstance, hWndParent),
    _pParentWindow(pParentWindow)
{
    // Attach our message map.
	AttachStaticMap(stcMsgMap, MAPDIM(stcMsgMap));
}

/*virtual*/
StatusBar::~StatusBar() throw()
{
}

bool StatusBar::Show(void)			
{
    bool fOk = true;

    // Create the video host window if necessary.
    if (NULL == ACLWinBase::GetHWND())
    {
        // Set the rectangle to all zeros, it'll take care of resizing and positioning itself.
        RECT rc = { 0, 0, 0, 0 };
        fOk = ACLWinBase::Create(_pParentWindow->GetHWND(), STATUSCLASSNAME, &rc, NULL, (SBARS_SIZEGRIP|WS_CHILD), 0, NULL);

        // Create the status bar parts.
        _CreateParts();
    }

    // If we have a valid window, show it.
    if (false != fOk &&
        NULL != GetHWND())
    {
        fOk = ACLWinBase::Show();
    }

    return fOk;
}

void StatusBar::SetPartText
(
    IN UINT16	index,
    IN char*	text
)
{
    ::SendMessage(GetHWND(), (UINT)SB_SETTEXT, (WPARAM)index, (LPARAM)text);
}

LRESULT StatusBar::OnCreate
(
	IN WPARAM wParam,
	IN LPARAM lParam
)
{
    DBGPrintf(("StatusBar::OnCreate()\n"));

	return 0L;
}	//	::OnCreate

LRESULT StatusBar::OnDestroy
(
	IN WPARAM wParam,
	IN LPARAM lParam
)
{
    return 0L;
}	//	::OnDestroy

void StatusBar::_CreateParts(void)
{
    #define _NUM_PARTS 4

    // Get the coordinates of the parent window's client area.
    RECT rcClient;
    if (NULL != _pParentWindow)
    {
        _pParentWindow->GetClientRect(&rcClient);
    }
    else
    {
        // Get our very own client rectangle, as a fallback, it
        // may not look so hot, but it'll do in a pinch.
        this->GetClientRect(&rcClient);
    }

    UINT32 parts[_NUM_PARTS];
    UINT32 numParts = _NUM_PARTS;

    // Calculate the right edge coordinate for each part, and
    // copy the coordinates to the array.
    UINT32 width = (rcClient.right / numParts);
    UINT32 nextWidth = width;
    for (UINT32 ii = 0; ii < numParts; ii++) 
    { 
       parts[ii] = nextWidth;
       nextWidth += width;
    }

    // Tell the status bar to create the window parts.
    ::SendMessage(GetHWND(), SB_SETPARTS, (WPARAM)numParts, (LPARAM)&parts);
}
