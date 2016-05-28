//	Copyright (c) 1995-2011, Robert R. Fahrni
//	All rights reserved.
//
//	Redistribution and use in source and binary forms, with or without modification, 
//	are permitted provided that the following conditions are met:
//
//	1.	Redistributions of source code must retain the above copyright notice, 
//		this list of conditions and the following disclaimer.
//	2.	Redistributions in binary form must reproduce the above copyright notice, 
//		this list of conditions and the following disclaimer in the documentation and/or 
//		other materials provided with the distribution.
//	3.	Neither the name of the author nor the names of its contributors may be 
//		used to endorse or promote products derived from this software without specific 
//		prior written permission.
//
//	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS 
//	OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
//	AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
//	CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
//	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
//	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
//	IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
//	OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include "ACLWinBase.h"
#include "ACLOutMsg.h"

ACLWinBase::ACLWinBase
(
	IN HINSTANCE	hInst,
	IN HWND 		hWndParent
) :
    _hInstance(hInst),
    _hWndParent(hWndParent),
    _hWnd(NULL),
    //_msgMap,          // Object, no init necessary.
    _numMessages(0)
{
	_Init();
}	//	ACLWinBase

ACLWinBase::ACLWinBase()
{
	_Init();
}	//	::ACLWinBase

/*virtual*/ 
ACLWinBase::~ACLWinBase() throw()
{
    Destroy();
}

void ACLWinBase::_Init(void)
{
}	//	::_Init

bool ACLWinBase::_InternalCreate
(
	IN LPCTSTR	pszWndClassName,	//	Class name of window to create.
	IN HWND		hWndParent,			//	Parent window, NULL ok
	IN LPRECT	pRect,				//	default == NULL
	IN LPCTSTR	pszWindowName,		//	default == NULL
	IN DWORD	dwStyle,			//	default == 0
	IN DWORD	dwExStyle,			//	default == 0
	IN HMENU	hMenu				//	default == NULL
)
{
	//	If the window handle is good then we're good to go.
	//
	_hWnd = _CreateWindow(pszWndClassName, hWndParent, pRect, pszWindowName,
					dwStyle, dwExStyle, hMenu, (PVOID)this);
	return ((NULL != _hWnd) ? true : false);
}	// _InternalCreate

HWND ACLWinBase::_CreateWindow
(
	IN LPCTSTR	pszWndClassName,	//	Class name of window to create.
	IN HWND		hWndParent,			//	Parent window, NULL ok
	IN LPRECT	pRect,				//	default == NULL
	IN LPCTSTR	pszWindowName,		//	default == NULL
	IN DWORD	dwStyle,			//	default == 0
	IN DWORD	dwExStyle,			//	default == 0
	IN HMENU	hMenu,				//	default == NULL
	IN PVOID	pExtraBits			//	default == NULL
)
{
    HWND hWnd = NULL;

    //	We're going to fail if the message map is empty
    //
    if (false == _msgMap.empty())
	{
	    int left	= 0;
	    int top		= 0;
	    int width	= 0;
	    int height	= 0;
	    if (pRect != NULL)
	    {
    	    left	= pRect->left;
		    top		= pRect->top;
		    width	= (pRect->right - pRect->left); //PRECT_WIDTH(pRect);
		    height	= (pRect->bottom - pRect->top); //PRECT_HEIGHT(pRect);
	    }

	    hWnd = ::CreateWindowEx(dwExStyle, pszWndClassName, pszWindowName, dwStyle, 
							    left, top, width, height,
							    hWndParent, hMenu, (HINSTANCE)_hInstance, pExtraBits);
    }

#ifdef _DEBUG
    if (hWnd == NULL)
    {
	    DWORD dw(::GetLastError());
	    DBGPrintf(("ACLWinBase::CreateWin failed with error %d\n", dw));
    }
#endif

    return hWnd;
}

bool ACLWinBase::Create
(
	IN LPCTSTR	pszWndClassName,	//	Class name of window to create.
	IN LPRECT	pRect,				//	default == NULL
	IN LPCTSTR	pszWindowName,		//	default == NULL
	IN DWORD	dwStyle,			//	default == 0
	IN DWORD	dwExStyle,			//	default == 0
	IN HMENU	hMenu				//	default == NULL
)
{
	return _InternalCreate(pszWndClassName, _hWndParent, pRect, pszWindowName, dwStyle, dwExStyle, hMenu);
}	//	::Create

bool ACLWinBase::Create
(
	IN HWND		hWndParent,			//	Parent window, NULL ok
	IN LPCTSTR	pszWndClassName,	//	Class name of window to create.
	IN LPRECT	pRect,				//	default == NULL
	IN LPCTSTR	pszWindowName,		//	default == NULL
	IN DWORD	dwStyle,			//	default == 0
	IN DWORD	dwExStyle,			//	default == 0
	IN HMENU	hMenu				//	default == NULL
)
{
    return _InternalCreate(pszWndClassName, hWndParent, pRect, pszWindowName, dwStyle, dwExStyle, hMenu);
}	//	::Create

bool ACLWinBase::Show(IN int nCmdShow /*=SW_SHOWNORMAL*/)			
{
    bool fOk = false;

    if (NULL != _hWnd)
	{
		fOk = (::ShowWindow(_hWnd, nCmdShow) ? true : false);
	}

    return fOk;
}	// Show

bool ACLWinBase::Hide(void)
{
    bool fOk = false;

    if (NULL != _hWnd)
	{
		// ShowWindow will return a positive value, it can be something other than true, so
		// we'll just normalize it to true or false.
		//
		// 
		fOk = ((::ShowWindow(_hWnd, SW_HIDE)) ? true : false);
	}

    return fOk;
}			


bool ACLWinBase::Destroy(void)
{
	bool fOk = true;
	
	if (NULL != _hWnd)
	{
		fOk = (::DestroyWindow(_hWnd) ? true : false);
    }	

	_hWnd = NULL;
	
    return fOk;
}	// Destroy

//============================================================================
//  void ACLWinBase::AttachStaticMap(IN PACLWinMessageMap, IN UINT32)
//============================================================================
void ACLWinBase::AttachStaticMap
(
	IN PACLWinMessageMap	map, 
	IN UINT32				cbMap
)
{ 
	for (UINT32 idx(0);
		 idx < cbMap;
		 ++idx, ++map)
	{
		//	Push the messages into our message collection.
		//
		_msgMap.push_back(*map);
	}
	
	//	This is here for backward compatibility.
	//
	_numMessages += (int)cbMap;
};

//============================================================================
//  bool ACLWinBase::InsertMessage(IN ACLWinMessageMap& mapEntry)
//============================================================================
bool ACLWinBase::InsertMessage(IN ACLWinMessageMap& mapEntry)
{
	bool fOk(false);
	
	//	Do some sanity checking on the map entry.
	//
	if (NULL != mapEntry.pMsgHandler)
	{
		_msgMap.push_back(mapEntry);
		++_numMessages;
		fOk = true;
	}	
	
	return fOk;
}	//	::InsertMessage

/****************************************************************************
 *+ ACLWinBase::DispatchMessage
 ****************************************************************************
 */
LRESULT ACLWinBase::DispatchMessage
(
	IN UINT		uMsg,
	IN WPARAM	wParam,
	IN LPARAM	lParam
)
{
	LRESULT			lResult(1);
	UINT			uSubMsg(WM_NULL);		//	By default this really isn't looked at.
	
	//	If we've received a W_COMMAND message or a
	//	W_NOTIFY message the dive into the sub-messages
	//	contained in these messages.
	//
    switch (uMsg)
	{
		case WM_COMMAND:
		{
			uSubMsg = LOWORD(wParam);
			break;
		}

		case WM_NOTIFY:
		{
			LPNMHDR pHDR = (LPNMHDR)lParam;
			if (NULL != pHDR)
			{
				//	This is the REAL message.
				//
				uSubMsg = pHDR->code;		
			}
			break;
		}

		case WM_SYSCOMMAND:
		{
			uSubMsg = (UINT)wParam;
			break;
		}

	}	//	Pre-message processing.

	bool			bail(false);
	PWINMSGHNDLR 	pHandler 	= NULL;
	
	//	Check the Message map for the object, if the message passed
	//	in here matches one in the map then grab the name of the
	//	dispatch function and dispatch it!
	//
	for (ACLWinMessageMapCollection::iterator it = _msgMap.begin();
		 it != _msgMap.end() && false == bail;
		 ++it)
	{
		//	If the main command(wMsg) and the control ID(uSubMsg) 
		//	match then we've found a command handler for the 
		//	message.
		//
		if ((uMsg == (*it).uMsg) && 
			(uSubMsg == (*it).uSubMsg))
		{
			pHandler	= (*it).pMsgHandler;		
			bail		= true;
		}
	}

    //	If we found a matching message handler, then
	//	dispatch it otherwise return false.
	//
    if (NULL != pHandler)
	{
        lResult = (this->*pHandler)(wParam, lParam);
	}

	return lResult;
}	//	::DispatchMessage

LRESULT ACLWinBase::DefWindowProc
(
	IN UINT		msg,
	IN WPARAM	wParam,
	IN LPARAM	lParam
)
{
	return ::DefWindowProc(_hWnd, msg, wParam, lParam);
}	//	::DefWindowProc

/****************************************************************************
 *+ ACLWinBase::SetBoundDialogObject
 ****************************************************************************
 */
//#pragma warning(disable : 4311 4312)
void ACLWinBase::SetBoundWindow(IN HWND hWnd)
{
	//	First set the _hWnd member
	//
	this->SetHWND(hWnd);

	// 	This will save a pointer to the "this" for the class.
    LONG_PTR lValue = ::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
    if (0 == lValue)
    {
        DBGPrintf(("SetWindowLongPtr failed with error %x\n", ::GetLastError()));
    }
}	//	::SetBoundDialogObject
//#pragma warning(default : 4311 4312)

	
/****************************************************************************
 *+ ACLWinBase::SetHWND
 ****************************************************************************
 */
VOID ACLWinBase::SetHWND(IN HWND hWnd)
{
	_hWnd = hWnd;
}	// SetHWND

/****************************************************************************
 *+ ACLWinBase::GetDlgHDLG
 ****************************************************************************
 */
const HWND ACLWinBase::GetHWND(void)
{
	return _hWnd;
}	// GetHWND

const HWND ACLWinBase::GetParentHWND(void)
{
    return _hWndParent;
}

bool ACLWinBase::GetWindowRect(LPRECT pRect)
{
    bool fOk = false;
    if (NULL != pRect)
    {
		fOk = (::GetWindowRect(_hWnd, pRect) ? true : false);
    }

    return fOk;
}

bool ACLWinBase::GetClientRect(LPRECT pRect)
{
    bool fOk = false;
    if (NULL != pRect)
    {
		fOk = (::GetClientRect(_hWnd, pRect) ? true : false);
    }
    
    return fOk;
}

bool ACLWinBase::SetWindowSize(LPRECT pRect)
{
    bool fOk = false;
    if (NULL != pRect)
    {
		fOk = (::SetWindowPos(_hWnd, NULL, pRect->left, pRect->top, (pRect->right - pRect->left), (pRect->bottom - pRect->top), SWP_NOOWNERZORDER) ? true : false);
    }

    return fOk;
}


/****************************************************************************
 *+ ACLWinBase::Center
 ****************************************************************************
 */
void ACLWinBase::Center(void)
{
	RECT rect;

	::GetWindowRect(_hWnd, &rect);
	::OffsetRect(&rect, -rect.left, -rect.top);

	rect.left = ((::GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2 + 4) & ~7;
	rect.top =  (::GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2;

	::SetWindowPos(_hWnd, NULL, rect.left, rect.top, NULL, NULL, (SWP_NOSIZE|SWP_NOZORDER));
}	// ::Center

void ACLWinBase::SetTransparent
( 
    IN bool  bTransparent/*=true*/,
    IN int   opacity/*=kWinOpacityMin*/
)
{
	if (false != bTransparent &&
        opacity >= kWinOpacityMin && opacity <= kWinOpacityMax)
	{
        // The Window must be a layered window. Get the
        // style bits and make sure it's layered.
		long style = ::GetWindowLong(_hWnd, GWL_EXSTYLE);
		style |= WS_EX_LAYERED;
		::SetWindowLong(_hWnd, GWL_EXSTYLE, style);

        // Set the opacity.
		::SetLayeredWindowAttributes(_hWnd, 0, opacity, LWA_ALPHA);		
	}
	else
	{
        // Twiddle the layered bits back off.
		long style = ::GetWindowLong(_hWnd, GWL_EXSTYLE);
		style ^= WS_EX_LAYERED;
		::SetWindowLong(_hWnd, GWL_EXSTYLE, style);

        // Invalidate the window rectangle so it'll repaint
        // a solid color.
        RECT rc;
		::GetWindowRect(_hWnd, &rc);
		::InvalidateRect(_hWnd, &rc, true);
	}
}

/****************************************************************************
 *	--------------+  "C"  H E L P E R  F U N C T I O N S  +------------------
 ***************************************************************************/
 
/****************************************************************************
 *+ Function: ACLWindowDispatchProc
 *
 *	This function is the one and only dialog proc for dialogs derived from
 *	the ACLWinBase class. All messages will be forwarded thru this 
 *	dialog proc. This will require that the derived class creates a 
 *	message map that will allow this function to work.
 *
 ****************************************************************************
 */
EXTERNC ACLWINCB(LRESULT) ACLWindowDispatchProc
(
	IN HWND 	hWnd,
	IN UINT 	uMsg,
	IN WPARAM	wParam,
	IN LPARAM	lParam
)
{
    UNALIGNED ACLWinBase*   pWindow 	= NULL;
	PWINMSGHNDLR 	        pHandler 	= NULL;
	LRESULT		            lResult		= 1;

    switch (uMsg)
	{
	    case WM_CREATE:
		{
			//	lparam is passed with a pointer to the "this" of our
			//	object. Grab it here and "attach" it to the dialog
			//	proc.
			//
			pWindow = (ACLWinBase*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
			pWindow->SetBoundWindow(hWnd);
	        break;
		}    
	    default:
	    {
	        //	By default we're going to grab a pointer to the "this" we
			//	saved at Init time.
			//
			pWindow = ACLPrivateGetBoundWindow(hWnd);
	        break;
	    }
	}	//	switch

	if (NULL != pWindow)
	{
		lResult = pWindow->DispatchMessage(uMsg, wParam, lParam);
	}

	//	If we couldn't get a window to dispatch to or the message returned 1
	//	then go ahead and call DefWindowProc.
	//
	if (pWindow == NULL || lResult == 1)
	{
		lResult = ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return lResult;
} 	// ACLWindowDispatchProc

#pragma warning(disable : 4311 4312)
ACLWinBase* ACLPrivateGetBoundWindow(IN HWND hWnd)
{
    return (ACLWinBase*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
}	//	ACLPrivateGetBoundWindow
#pragma warning(default : 4311 4312)
