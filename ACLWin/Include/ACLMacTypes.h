//	Copyright (c) 2013, Robert R. Fahrni
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
#ifndef _ACLMACTYPES_H
#define _ACLMACTYPES_H
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef EXTERNC
#define EXTERNC extern "C"
#endif

#define ACLDLG_USERDATA	// GWL_USERDATA
#define ACLEXPORT		// __declspec(dllexport)
#define ACLDLGCB(type)	ACLEXPORT type
#define ACLDLGCBV(type)	ACLEXPORT type
#define ACLWINCB(type)	ACLEXPORT type
#define ACLWINCBV(type)	ACLEXPORT type

#ifndef LOCAL
#define LOCAL static
#endif

#define TRUE            true
#define FALSE           false

#define TCHAR           char
#define LPTSTR          char*
#define LPCTSTR         const char*
#define BOOL            bool
#define VOID            void
#define UINT            unsigned int
#define HINSTANCE       void*
#define DWORD           unsigned long

#define STRLEN                  strlen
#define STRCPY                  strcpy
#define STRCPY_S(t, sl, s)      strncpy(t, s, sl)
#define STRCAT                  strcat
#define STRSTR                  strstr
#define STRCHR                  strchr
#define STRRCHR                 strrchr
#define STRNCPY                 strncpy
#define STRNCPY_S(t, tl, s, sl)	strncpy(t, s, sl)
#define STRNCAT                 strncat
#define STRNCAT_S(t, tl, s, sl) strncat(t, s, sl)
#define	STRCMP                  strcmp
#define STRICMP                 strcasecmp
#define STRUPR                  strupr
#define STRLWR                  strlwr
#define TOUPPER(ch)             toupper(ch)
#define TOLOWER(ch)             tolower(ch)
#define SPRINTF                 sprintf
#define SPRINTFNS               vsnprintf
#define VSCPRINTF               _vscprintf
#define MEMSET                  memset
#define MEMCPY                  memcpy
#define MEMMOVE                 memmove

#define CLEANSTRING(s)	memset(s, 0, sizeof(s))
#define CLEAN(x)		memset(x, 0, sizeof(x))

//	Used by Find, if no string found return this.
#define STR_NOTFOUND			-1

//	Used for comparisons.
#define FSTR_CASESENSITIVE		TRUE
#define FSTR_NOTCASESENSITIVE	FALSE

//	Other defines.
#define STR_PADDING		8				//	 Number of bytes to pad the string.

#define CHAR_UPPER	CharUpperBuff
#define CHAR_LOWER  CharLowerBuff
#define CHARNEXT            //::CharNext
#define CHARPREV            //::CharPrev

#define AnsiUpper(ib)   //
#define ZeroMemory(b, l)    memset(b, 0, l)

#endif
