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
#ifndef _ACLWINBASE_H_
#define _ACLWINBASE_H_

#include "ACLWinTypes.h"
#include <vector>

//	Forward Declaration
//
class ACLWinBase;
struct ACLWinMessageMap;

//	A collection for MessageMap entries.
//
typedef std::vector<ACLWinMessageMap> ACLWinMessageMapCollection;

EXTERNC ACLWINCB(LRESULT)   ACLWindowDispatchProc(IN HWND, IN UINT, IN WPARAM, IN LPARAM);
ACLWinBase*                 ACLPrivateGetBoundWindow(IN HWND);

typedef ACLWinMessageMap*	PACLWinMessageMap;
typedef ACLWinMessageMap**	PPACLWinMessageMap;

#define kWinOpacityMin      0
#define kWinOpacityDefault  220
#define kWinOpacityMax      255

//============================================================================
//  class ACLWinBase
//============================================================================
class ACLWinBase
{
public:
    ACLWinBase(IN HINSTANCE, IN HWND);
	ACLWinBase();
    virtual ~ACLWinBase() throw();

    // Message Map helpers.
	void AttachStaticMap(IN PACLWinMessageMap pMap, IN UINT32 cbSize);
	bool InsertMessage(IN ACLWinMessageMap& mapEntry);

    // Windows helpers.
    bool Create
    (
	    IN LPCTSTR	pszWndClassName,	//	Class name of window to create.
	    IN LPRECT	pRect,				//	default == NULL
	    IN LPCTSTR	pszWindowName,		//	default == NULL
	    IN DWORD	dwStyle,			//	default == 0
	    IN DWORD	dwExStyle,			//	default == 0
	    IN HMENU	hMenu				//	default == NULL
    );
    bool Create
    (
	    IN HWND		hWndParent,			//	Parent window, NULL ok
	    IN LPCTSTR	pszWndClassName,	//	Class name of window to create.
	    IN LPRECT	pRect,				//	default == NULL
	    IN LPCTSTR	pszWindowName,		//	default == NULL
	    IN DWORD	dwStyle,			//	default == 0
	    IN DWORD	dwExStyle,			//	default == 0
	    IN HMENU	hMenu				//	default == NULL
    );
	bool Show(IN int nCmdShow=SW_SHOWNORMAL);
    bool Hide(void);			
    bool Destroy(void);
	void Center(void);
    void SetTransparent
    ( 
        IN bool  bTransparent=true,
        IN int   opacity=kWinOpacityDefault
    );

    LRESULT DispatchMessage(IN UINT, IN WPARAM, IN LPARAM);
	LRESULT DefWindowProc(IN UINT msg, IN WPARAM wParam, IN LPARAM lParam);
    
	void SetBoundWindow(IN HWND);
	void SetHWND(IN HWND);
	const HWND GetHWND(void);
    const HWND GetParentHWND(void);

    bool GetWindowRect(LPRECT pRect);
    bool GetClientRect(LPRECT pRect);

    bool SetWindowSize(LPRECT pRect);
	
protected:
	void _Init();
    bool _InternalCreate
    (
	    IN LPCTSTR	pszWndClassName,	//	Class name of window to create.
	    IN HWND		hWndParent,			//	Parent window, NULL ok
	    IN LPRECT	pRect,				//	default == NULL
	    IN LPCTSTR	pszWindowName,		//	default == NULL
	    IN DWORD	dwStyle,			//	default == 0
	    IN DWORD	dwExStyle,			//	default == 0
	    IN HMENU	hMenu				//	default == NULL
    );
    HWND _CreateWindow
    (
	    IN LPCTSTR	pszWndClassName,	//	Class name of window to create.
	    IN HWND		hWndParent,			//	Parent window, NULL ok
	    IN LPRECT	pRect,				//	default == NULL
	    IN LPCTSTR	pszWindowName,		//	default == NULL
	    IN DWORD	dwStyle,			//	default == 0
	    IN DWORD	dwExStyle,			//	default == 0
	    IN HMENU	hMenu,				//	default == NULL
	    IN PVOID	pExtraBits			//	default == NULL
    );

protected:
    HINSTANCE 					_hInstance;
	HWND 						_hWndParent;
	HWND 						_hWnd;
	ACLWinMessageMapCollection	_msgMap;
	UINT32						_numMessages;
};	// ACLWinBase

typedef LRESULT (ACLWinBase::*PWINMSGHNDLR)(IN WPARAM, IN LPARAM);

// ACLWinMessageMap definition the second parameter is the pointer to our dispatch
// function, to use this class you need to create a message map like this:
// ACLWinMessageMap msgMap[] = { { WM_COMMAND, OnWMCommand }
//						    , { WM_DESTROY, OnWMDestroy }
//						    }
struct ACLWinMessageMap
{
	UINT 		    uMsg;			//	EG... W_COMMAND, etc...
	UINT		    uSubMsg;		//	EG... IDOK, control ID's and the like...
	PWINMSGHNDLR 	pMsgHandler; 	//	EG... OnWMCommand, notice the signature is the same for ALL MESSAGES!!!!
};	//	ACLWinMessageMap

//	Used to calculate the number of messages in a particular ACLWinMessageMap.
//
#define MAPDIM(mm)	(sizeof(mm) / sizeof(ACLWinMessageMap))

#endif //	_ACLWINBASE_H_

