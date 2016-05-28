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
#ifndef _ACLSTR_H
#define _ACLSTR_H
#include "ACLPlat.h"

class ACLStr 
{
public:
	//	Constructors
	ACLStr();
	ACLStr(IN const TCHAR);
	ACLStr(IN LPCTSTR);
	ACLStr(IN const ACLStr&);

	//	Destructor
	virtual ~ACLStr();

	//	Members that act like MS-BASIC string functions.
	ACLStr Right(IN size_t);
	ACLStr Left(IN size_t);
	ACLStr Mid(IN size_t);
	ACLStr Mid(IN size_t, IN size_t);
	int Find(IN const ACLStr&);
	int Find(IN LPCTSTR);
	int Find(IN const TCHAR, IN BOOL fCaseSensitive = TRUE);
	int FindLast(IN const TCHAR, IN BOOL fCaseSensitive = TRUE);
	BOOL Compare(IN const ACLStr&, IN BOOL fCaseSensitive = TRUE) const;
	BOOL Compare(IN LPCTSTR, IN BOOL fCaseSensitive = TRUE) const;
	ACLStr Replace(IN const TCHAR, IN const TCHAR);
	ACLStr Strip(IN const TCHAR);
	ACLStr StripWord(IN const ACLStr&);
	VOID RTrim(VOID);
	VOID LTrim(VOID);
	VOID Trim(VOID);
	BOOL LoadString(IN UINT);
	TCHAR GetAtOffset(IN size_t);
	void FormatInternal(LPCTSTR format, ...);	// Format in place

	// Static Helper Methods
	static ACLStr Format(LPCTSTR format, ...);

	//	Misc. Helper members
	size_t Len(VOID) const;
	VOID Clear(VOID);
	VOID Append(IN const ACLStr&);
	VOID Append(IN LPCTSTR);
    VOID Append(IN LPCTSTR str, IN size_t strLen);
	VOID Append(IN TCHAR);
	VOID MakeUpper(VOID);
	VOID MakeLower(VOID);
	LPTSTR CharFirst(IN VOID);
	LPTSTR CharNext(IN VOID);
	
#if !defined(NO_BSTRS)
	//	OLE Helpers.
	//
	BSTR MakeBstr(VOID);		//	Use SysFreeString on the resulting BSTR.
	VOID MakeLPTSTR(IN BSTR);	//	Used to convert a BSTR to an LPTSTR
#endif

#if defined(IS_WINDOWS) //_WIN32)
	ACLStr(IN const BSTR);
	const ACLStr& operator=(IN const BSTR);
	const ACLStr& operator+=(IN const BSTR);
#endif	//	_WIN32
    
	// Operators
	//
	const ACLStr& operator=(IN const TCHAR);
	const ACLStr& operator=(IN const ACLStr&);
	const ACLStr& operator=(IN LPCTSTR);
	const ACLStr& operator+=(IN const TCHAR);
	const ACLStr& operator+=(IN const ACLStr&);
	const ACLStr& operator+=(IN LPCTSTR);
	const TCHAR operator[](IN int);
	BOOL operator==(IN const ACLStr&) const;
	BOOL operator==(IN LPCTSTR) const;
	//BOOL operator==(IN BSTR) const;		//	Later!!!

	friend ACLStr operator+(IN const ACLStr&, IN const ACLStr&);

	//	This operator is VERY handy for function that take
	//	a string as a parameter.
	operator LPCTSTR() const;
	operator LPTSTR() const;

//	Properties
private:
	LPTSTR	_string;				//	The ACTUAL string buffer.
	LPTSTR	_currStr;				//	used for iteration through characters.
	size_t	_length;				//	Length we tell the user of the class.
	size_t	_realLength;			//	REAL buffer lenght, we allocate with padding.

//	Internal methods
private:
	VOID _Init(VOID);
	VOID _SafeInit(VOID);
	VOID _Alloc(IN size_t);
	VOID _Grow(IN const size_t);
	VOID _Wack(VOID);
	size_t _RealLen(VOID) const;
	VOID _UpperCase(IN LPCTSTR, IN OUT LPTSTR, IN const int, IN OUT LPTSTR, IN const int);
	int _FindCh(IN const TCHAR CharToFind, IN BOOL fCaseSensitive = TRUE, IN BOOL fReverseFind = FALSE);
	VOID _Concat(IN size_t, IN LPCTSTR);
	VOID _Recycle(VOID);
};	// ACLStr

#if defined(REMOVE_ME)
//	Used by Find, if no string found return this.
#define STR_NOTFOUND			-1

//	Used for comparisons.
#define FSTR_CASESENSITIVE		TRUE
#define FSTR_NOTCASESENSITIVE	FALSE

//	Other defines.
#define STR_PADDING		8				//	 Number of bytes to pad the string.

#define CHAR_UPPER	::CharUpperBuff
#define CHAR_LOWER	::CharLowerBuff
#define CHARNEXT	::CharNext
#define CHARPREV	::CharPrev
#endif // REMOVE_ME

//	Global helper
ACLStr LoadString(IN HINSTANCE, IN UINT);

#endif	// _VDSSTR_H
