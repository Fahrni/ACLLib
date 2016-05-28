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
#ifndef _ACLWINTYPES_H
#define _ACLWINTYPES_H

#include <windows.h>
#include <windowsx.h>
#include <tchar.h> 

#ifndef IN
	#define IN
#endif

#ifndef OUT
	#define OUT
#endif

#ifndef EXTERNC
	#define EXTERNC extern "C"
#endif

#define ACLDLG_USERDATA	GWL_USERDATA
#define ACLEXPORT		__declspec(dllexport)
#define ACLDLGCB(type)	ACLEXPORT type WINAPI
#define ACLDLGCBV(type)	ACLEXPORT type WINAPIV
#define ACLWINCB(type)	ACLEXPORT type WINAPI
#define ACLWINCBV(type)	ACLEXPORT type WINAPIV

#ifndef LOCAL
	#define LOCAL static
#endif

//	String defines.
//
#define MAX_STRING		256
#define MID_SIZE		50
#define TITLE_SIZE		30
#define TINY_STRING		5

//	Helpful defines for characters.
//
#define CH_QUOTE		'"'
#define CH_SEMICOLON	';'
#define CH_EQ			'='
#define CH_NL			'\n'
#define CH_SPACE		32
#define CH_NOTHING		-1
#define CH_CR			10
#define CH_LF			13
#define CH_UNDERBAR		'_'
#define CH_MINUS		'-'
#define CH_PERIOD		'.'
#define CH_COMMA		','

#define STR_QUOTE		"\""
#define STR_SEMICOLON	";"
#define STR_EQ			"="
#define STR_NL			"\n"
#define STR_UNDERBAR	"_"
#define STR_MINUS		"-"
#define STR_PERIOD		"."
#define STR_COMMA		","

#define STRLEN			_tcslen
#define STRCPY			_tcscpy
#define STRCPY_S		_tcscpy_s
#define STRCAT			_tcscat
#define STRSTR			_tcsstr
#define STRCHR			_tcschr
#define STRRCHR			_tcsrchr
#define STRNCPY			_tcsnccpy
#define STRNCPY_S		_tcsnccpy_s
#define STRNCAT			_tcsncat
#define STRNCAT_S		_tcsncat_s
#define	STRCMP			_tcscmp
#define STRICMP			_tcsicmp
#define STRUPR			_tcsupr
#define STRLWR			_tcslwr
#define TOUPPER(ch)		_totupper(ch)
#define TOLOWER(ch)		_totlower(ch)
#define SPRINTF			_stprintf
#define SPRINTFNS       vsprintf_s
#define VSCPRINTF       _vscprintf
#define MEMSET			_fmemset
#define MEMCPY			memcpy
#define MEMMOVE			memmove

#define CLEANSTRING(s)	ZeroMemory(s, sizeof(s))
#define CLEAN(x)		ZeroMemory(x, sizeof(x))

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

#endif	// _ACLWINTYPES_H

