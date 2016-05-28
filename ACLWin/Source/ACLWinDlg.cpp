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
#include "ACLWinDlg.h"
#include "ACLOutMsg.h"

ACLWinDlg::ACLWinDlg
(
	IN HINSTANCE	hInst,
	IN LPCTSTR		lpszTemplate,
	IN HWND 		hWnd
)
{
	init();
	
	_hInstance 		= hInst;
	_hWndParent		= hWnd;
	_DlgTemplate	= lpszTemplate;
}	//	ACLWinDlg

ACLWinDlg::ACLWinDlg(VOID)
{
	init();
}	//	::ACLWinDlg

/*virtual*/ ACLWinDlg::~ACLWinDlg() throw()
{

}


VOID ACLWinDlg::init(VOID)
{
	//	Set initial values of these variables.
	//
	_hInstance 		= NULL;
	_hWndParent		= NULL;
	_DlgTemplate	= NULL;
	_hDialog		= NULL;
	_numMessages	= 0;
	//_pMsgMap		= NULL;
}	//	::init


/****************************************************************************
 *+ ACLWinDlg::Show
 ****************************************************************************
 */
INT_PTR ACLWinDlg::Show(VOID)
{
	INT_PTR result(-1);
	
	//	If the object isn't initialized properly then
	//	_DlgTemplate will be NULL, return the message
	//	expected to be returned by the FAILURE of DialogBoxParam.
	//
	if (NULL != _DlgTemplate &&
		0 != _msgMap.size())
	{
		result = ::DialogBoxParam(_hInstance, _DlgTemplate, _hWndParent, (DLGPROC)ACLDlgDispatchProc, (LPARAM)this);
	}
	ELSEIF_DBGPrintf(("**** ::Show Failed, Missing Resource(_DlgTemplate) OR No Message Map(_msgMap), error %d\n", result));

	return result;
}	//	::Show


/****************************************************************************
 *+ ACLWinDlg::End
 ****************************************************************************
 */
bool ACLWinDlg::End(IN bool fTrueOrFalse)
{
	::EndDialog(_hDialog, fTrueOrFalse);
	return fTrueOrFalse;
}	//::End

//============================================================================
//  void ACLWinDlg::AttachStaticMap(IN PACLMessageMap, IN UINT32)
//
//  Description of function's implementation issues
//============================================================================
void ACLWinDlg::AttachStaticMap
(
	IN PACLMessageMap	map, 
	IN UINT32			cbMap
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
//  bool ACLWinDlg::InsertMessage(IN ACLMessageMap& mapEntry)
//
//  Description of function's implementation issues
//============================================================================
bool ACLWinDlg::InsertMessage(IN ACLMessageMap& mapEntry)
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
 *+ ACLWinDlg::DispatchMessage
 ****************************************************************************
 */
bool ACLWinDlg::DispatchMessage
(
	IN UINT		uMsg,
	IN WPARAM	wParam,
	IN LPARAM	lParam
)
{
	bool			fResult(false);
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
			if (pHDR)
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
	PMSGHNDLR 		pHandler 	= NULL;
	
	//	Check the Message map for the object, if the message passed
	//	in here matches one in the map then grab the name of the
	//	dispatch function and dispatch it!
	//
	for (ACLMessageMapCollection::iterator it = _msgMap.begin();
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
		fResult = ((this->*pHandler)(wParam, lParam) ? true : false);
	}

	return fResult;
}	//	::DispatchMessage

bool ACLWinDlg::DefWindowProc
(
	IN UINT		msg,
	IN WPARAM	wParam,
	IN LPARAM	lParam
)
{
	return (::DefWindowProc(_hDialog, msg, wParam, lParam) ? true : false);
}	//	::DefWindowProc

/****************************************************************************
 *+ ACLWinDlg::SetBoundDialogObject
 ****************************************************************************
 */
#pragma warning(disable : 4311 4312)
void ACLWinDlg::SetBoundDialogObject(IN HWND hDlg)
{
	//	First set the _hDialog member
	//
	this->SetDlgHDLG(hDlg);

	// 	This will save a pointer to the "this" for the class
	// 	object.
	//
	::SetWindowLong(hDlg, ACLDLG_USERDATA, (LONG)this);
}	//	::SetBoundDialogObject
#pragma warning(default : 4311 4312)

	
/****************************************************************************
 *+ ACLWinDlg::SetDlgHDLG
 ****************************************************************************
 */
VOID ACLWinDlg::SetDlgHDLG(IN HWND hDlg)
{
	_hDialog = hDlg;
}	//	::SetDlgHDLG

/****************************************************************************
 *+ ACLWinDlg::GetDlgHDLG
 ****************************************************************************
 */
const HWND ACLWinDlg::GetDlgHDLG(void)
{
	return _hDialog;
}	//	SetDlgHDLG

/****************************************************************************
 *+ ACLWinDlg::GetDlgTemplateName
 ****************************************************************************
 */
LPCTSTR ACLWinDlg::GetDlgTemplateName(void)
{
	return _DlgTemplate;
}	//	::GetDlgTemplateName

/****************************************************************************
 *+ ACLWinDlg::GetItem
 ****************************************************************************
 */
const HWND ACLWinDlg::GetItem(IN int nResID)
{
	return ::GetDlgItem(_hDialog, nResID);
}	//	::GetItem

/****************************************************************************
 *+ ACLWinDlg::SetDialogProperties
 ****************************************************************************
 */
void ACLWinDlg::SetDialogProperties
(
	IN HINSTANCE	hInst,
	IN LPTSTR		lpszTemplate,
	IN HWND 		hWnd
)
{
	_hInstance 		= hInst;
	_hWndParent		= hWnd;
	_DlgTemplate	= lpszTemplate;
}	//	::SetDialogProperties


/****************************************************************************
 *+ ACLWinDlg::Center
 ****************************************************************************
 */
void ACLWinDlg::Center(void)
{
	RECT rect;

	::GetWindowRect(_hDialog, &rect);
	::OffsetRect(&rect, -rect.left, -rect.top);

	rect.left = ((::GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2 + 4) & ~7;
	rect.top =  (::GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2;

	::SetWindowPos(_hDialog, NULL, rect.left, rect.top, NULL, NULL, (SWP_NOSIZE|SWP_NOZORDER));
}	// ::Center


/****************************************************************************
 *	--------------+  "C"  H E L P E R  F U N C T I O N S  +------------------
 ***************************************************************************/
 
/****************************************************************************
 *+ Function: ACLDlgDispatchProc
 *
 *	This function is the one and only dialog proc for dialogs derived from
 *	the ACLWinDlg class. All messages will be forwarded thru this 
 *	dialog proc. This will require that the derived class creates a 
 *	message map that will allow this function to work.
 *
 ****************************************************************************
 */
EXTERNC ACLDLGCB(BOOL) ACLDlgDispatchProc
(
	IN HWND 	hDlg,
	IN UINT 	uMsg,
	IN WPARAM	wParam,
	IN LPARAM	lParam
)
{
    ACLWinDlg* 	pDialog 	= NULL;
	PMSGHNDLR 	pHandler 	= NULL;
	BOOL		fResult		= false;

    switch (uMsg)
	{

	    case WM_INITDIALOG:
		{
			//	lparam is passed with a pointer to the "this" of our
			//	object. Grab it here and "attach" it to the dialog
			//	proc.
			//
			pDialog = (ACLWinDlg*)lParam;
			pDialog->SetBoundDialogObject(hDlg);
	        break;
		}    
	    default:
	    {
	        //	By default we're going to grab a pointer to the "this" we
			//	saved at Init time.
			//
			pDialog = ACLPrivateGetBoundDialog(hDlg);
	        break;
	    }
	}	//	switch

	if (NULL != pDialog)
	{
		fResult = pDialog->DispatchMessage(uMsg, wParam, lParam);
	}

	return fResult;
} 	// ACLDlgDispatchProc

#pragma warning(disable : 4311 4312)
ACLWinDlg* ACLPrivateGetBoundDialog(IN HWND hDlg)
{
	return (ACLWinDlg*)::GetWindowLong(hDlg, ACLDLG_USERDATA);
}	//	ACLPrivateGetBoundDialog
#pragma warning(default : 4311 4312)
