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
#ifndef _ACLLIB
#define _ACLLIB

#if defined(USE_STL_STR)
#include <string>
#define STRING		std::string
#define CHARPTR(s)	s.c_str()
#else
#include "ACLStr.h"
#define STRING		ACLStr
#define CHARPTR(s)	s
#endif // 

#if defined(WIN32)
#define IS_WINDOWS 1
#endif

#if defined(LINUX_OS)
#define IS_LINUX	1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#if defined(IS_WINDOWS)
#include "windows.h"
#endif

#if defined(IS_LINUX)
typedef void* HMODULE;
#endif

class ACLLib
{
public:
	ACLLib(IN const STRING& libName);
	ACLLib(IN const char* libName, IN const UINT32 cbLibName);
	virtual ~ACLLib() throw();
	
	bool Load();
	bool Unload();
	FARPROC GetProcAddress(IN const STRING& procName);	
	FARPROC GetProcAddress(IN const char* procName);
	
	//	Operations on a lib.
	//
	STRING LoadString(IN const UINT32 uid);
	HICON LoadIcon(IN const char* id);
	int MessageBox(
		IN HWND			hWnd,
		IN long			style,
		IN UINT32		resTitle,
		IN char*		formatString,
		IN ...);
	
	//	Useful for functions that take a raw HMODULE
	//
	operator HMODULE() const;

protected:
	ACLLib();
	
private:
	STRING		_modulePath;
	HMODULE		_moduleHandle;
};	//	ACLLib

#endif	//	_ACLLIB
