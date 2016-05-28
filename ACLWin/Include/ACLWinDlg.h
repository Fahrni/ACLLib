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
#ifndef _ACLDLG_H_
#define _ACLDLG_H_

#include "ACLWinDlgTypes.h"
#include <vector>

//	Forward Declaration
//
class ACLWinDlg;
struct ACLMessageMap;

//	A collection for MessageMap entries.
//
typedef std::vector<ACLMessageMap> ACLMessageMapCollection;

EXTERNC ACLDLGCB(BOOL) ACLDlgDispatchProc(IN HWND, IN UINT, IN WPARAM, IN LPARAM);
ACLWinDlg* ACLPrivateGetBoundDialog(IN HWND);

typedef ACLMessageMap*	PACLMessageMap;
typedef ACLMessageMap**	PPACLMessageMap;

//============================================================================
//  class ACLWinDlg
//============================================================================
class ACLWinDlg
{

public:
	friend ACLDLGCB(BOOL) ACLDlgDispatchProc(IN HWND, IN UINT, IN WPARAM, IN LPARAM);
	friend ACLWinDlg* ACLPrivateGetBoundDialog(IN HWND);

    ACLWinDlg(IN HINSTANCE, IN LPCTSTR, IN HWND);
	ACLWinDlg();
    virtual ~ACLWinDlg() throw();

	void AttachStaticMap(IN PACLMessageMap pMap, IN UINT32 cbSize);
	bool InsertMessage(IN ACLMessageMap& mapEntry);
	bool DispatchMessage(IN UINT, IN WPARAM, IN LPARAM);
	bool DefWindowProc(IN UINT msg, IN WPARAM wParam, IN LPARAM lParam);
	void SetBoundDialogObject(IN HWND);
	void SetDlgHDLG(IN HWND);
	const HWND GetDlgHDLG(VOID);
	LPCTSTR GetDlgTemplateName(VOID);
	void Center(VOID);
	const HWND GetItem(IN int);
	void SetDialogProperties(IN HINSTANCE, IN LPTSTR, IN HWND);

    virtual INT_PTR Show();
	virtual bool End(IN bool);

private:
	VOID init();

protected:
    HINSTANCE 				_hInstance;
	HWND 					_hWndParent;
	HWND 					_hDialog;
	LPCTSTR					_DlgTemplate;
	ACLMessageMapCollection	_msgMap;
	int 					_numMessages;
};	// ACLWinDlg

typedef BOOL (ACLWinDlg::*PMSGHNDLR)(IN WPARAM, IN LPARAM);

// ACLMessageMap definition the second parameter is the pointer to our dispatch
// function, to use this class you need to create a message map like this:
// ACLMessageMap msgMap[] = { { W_COMMAND, OnWMCommand }
//						 , { W_DESTROY, OnWMDestroy }
//						}
struct ACLMessageMap
{
	UINT 		uMsg;			//	EG... W_COMMAND, etc...
	UINT		uSubMsg;		//	EG... IDOK, control ID's and the like...
	PMSGHNDLR 	pMsgHandler; 	//	EG... OnWMCommand, notice the signature is the same for ALL MESSAGES!!!!
};	//	ACLMessageMap

//	Used to calculate the number of messages in a particular ACLMessageMap.
//
#define DLGMAPDIM(mm)	(sizeof(mm) / sizeof(ACLMessageMap))

#endif //	_ACLDLG_H_

