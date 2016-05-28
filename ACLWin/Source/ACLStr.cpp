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
#include "ACLStr.h"
#include "ACLOutMsg.h"
#include <stdio.h>

#if !defined(va_copy)
#define va_copy(d, s) ((d) = (s))
#endif // va_copy

#define ARRAY_DELETE(a) if (a) { delete []a; a = NULL; }

// Local helper
static size_t __Alloc(IN size_t size, OUT LPTSTR* ppBuffer, OUT size_t* pRealSize);
static UINT __Format
(
	OUT LPTSTR* ppOutput,
	OUT UINT* pLen,
	IN LPCTSTR format, 
	IN va_list& args
);

ACLStr::ACLStr()
{
	_Init();
	_Alloc(0);
}	// ACLStr()

ACLStr::ACLStr(IN LPCTSTR str)
{
	_Init();

	if (str)
	{
		size_t len(STRLEN(str));
		_Alloc(len);
		STRNCPY_S(_string, _realLength, str, len); 
	}
}	// ACLStr(IN LPCTSTR)

ACLStr::ACLStr(IN const TCHAR ch)
{
	_Init();
	_Alloc(STR_PADDING);
	STRNCPY_S(_string, _realLength, &ch, 1); 
}	// ACLStr(IN LPCTSTR)


#if defined(_WIN32)  && !defined(NO_BSTRS)
ACLStr::ACLStr(IN const BSTR bstr)
{
	_Init();
	if (bstr)
	{
		UINT nStrLen = ::SysStringLen(bstr);
		_Alloc(nStrLen);

		MakeLPTSTR(bstr);
	}
}	// ACLStr(IN BSTR)
#endif	// _WIN32

ACLStr::ACLStr(IN const ACLStr& str)
{
	size_t len(str.Len());
	_Init();
	_Alloc(len);
	STRNCPY_S(_string, _realLength, str._string, len); 
}	// ACLStr(IN const &ACLStr)

ACLStr::~ACLStr(VOID)
{
	_Wack();
}	// ~ACLStr()

//	Provide MS-Basic like string functionality.
//	Left$   Right$   Mid$	etc...
//
//

ACLStr ACLStr::Right(IN size_t rightMost)
{
	ACLStr	rightStr;
	size_t	thisStringLength = Len();

	//	If we have something to copy and we were told to copy
	//	at least one byte then copy just what we were told.
	if ((_string && rightMost) && 
		(rightMost <= thisStringLength))
	{
		rightStr.Append(&_string[(thisStringLength - rightMost)], rightMost);
	}

	return rightStr;
}	//	Right()

ACLStr ACLStr::Left(IN size_t leftMost)
{
	ACLStr	leftStr;
	size_t	thisStringLength = Len();

	//	If we have something to copy and we were told to copy
	//	at least one byte then copy just what we were told.
	//
	if ((_string && leftMost) && (leftMost <= thisStringLength))
	{	
		leftStr.Append(_string, leftMost);
	}

	return leftStr;
}	//	Left()

ACLStr ACLStr::Mid(IN size_t startHere)
{
	ACLStr	midStr;
	size_t	thisStringLength = Len();
	
	if (startHere < (thisStringLength - 1))
	{
		midStr.Append(&_string[startHere], (thisStringLength - startHere));
	}

	return midStr;
}	//	Mid - 1

ACLStr ACLStr::Mid
(
	IN size_t startHere,
	IN size_t numCharsToTake
)
{
	ACLStr	midStr;
	size_t	thisStringLength = Len();
	
	if (numCharsToTake > thisStringLength)
	{
		numCharsToTake = thisStringLength;
	}
	else if (numCharsToTake < 1)
	{
		numCharsToTake = 1;
	}
	
	midStr.Append(&_string[startHere], numCharsToTake);

	return midStr;
}	//	Mid - 2

TCHAR ACLStr::GetAtOffset(IN size_t offset)
{
	if (offset > Len())
	{
		offset = 0;
	}

	return _string[offset];
}	//	::GetAtIndex

void ACLStr::FormatInternal(LPCTSTR format, ...)
{
	if (format)
	{
		va_list args;
		va_start(args, format);

		// Format our internal buffer
		UINT realLen((UINT)_realLength);
		_length = (int)__Format(&_string, &realLen, format, args);
		if (_length > _realLength)
		{
			_realLength = _length;
		}
		va_end(args);
	}
}

VOID ACLStr::Append(IN const ACLStr& str)
{
	_Concat(str.Len(), (LPCTSTR)str);
}

VOID ACLStr::Append(IN LPCTSTR str)
{
	_Concat(STRLEN(str), str);
}	//	Append

VOID ACLStr::Append(IN LPCTSTR str, IN size_t strLen)
{
    if (str && strLen)
    {
        _Concat(strLen, str);
    }
}	//	Append

VOID ACLStr::Append(IN TCHAR ch)
{
	_Concat(1, &ch);
}	//	Append

#if !defined(IS_WINDOWS)
static DWORD CharUpperBuff(IN LPTSTR lpsz, IN DWORD cchLength)
{
    DWORD cchResult(0L);
    if (NULL != lpsz &&
        0 != cchLength)
    {
        cchResult = cchLength;
        LPTSTR p = lpsz;
        while((*p++ = toupper(*p)));
    }
    return cchResult;
}

static DWORD CharLowerBuff(OUT LPTSTR lpsz, DWORD cchLength)
{
    DWORD cchResult(0L);
    if (NULL != lpsz &&
        0 != cchLength)
    {
        cchResult = cchLength;
        LPTSTR p = lpsz;
        while((*p++ = tolower(*p)));
    }
    return cchResult;
}
#endif // !IS_WINDOWS

VOID ACLStr::MakeUpper(VOID)
{
	CHAR_UPPER(_string, _length);
}	//	MakeUpper

VOID ACLStr::MakeLower(VOID)
{
	CHAR_LOWER(_string, _length);
}	//	MakeLower

int ACLStr::Find(IN const ACLStr& toFind)
{
	LPTSTR	string	= NULL;
	TCHAR	internalBuffer[MAX_STRING];

	//	If there is a string to look at...
	if (toFind.Len())
	{
		TCHAR upperFind[MAX_STRING];
		CLEANSTRING(upperFind);
		CLEANSTRING(internalBuffer);

		//	First create a couple of temporary upper case buffers.
		_UpperCase((LPCTSTR)toFind, upperFind, MAX_STRING, internalBuffer, MAX_STRING);

		//	Go look for the starting point of the string.
		//	if we do NOT find it return STR_NOTFOUND
		string = STRSTR(internalBuffer, upperFind);
	}
	
	return (NULL == string) ? STR_NOTFOUND : (int)(string - internalBuffer);
} // Find

int ACLStr::Find(IN LPCTSTR toFind)
{
	LPTSTR	string	= NULL;
	TCHAR	internalBuffer[MAX_STRING];

	// If there is a string to look at...
	if (toFind)
	{
		TCHAR upperFind[MAX_STRING];
		CLEANSTRING(upperFind);
		CLEANSTRING(internalBuffer);

		//	First create a couple of temporary upper case buffers.
		_UpperCase(toFind, upperFind, MAX_STRING, internalBuffer, MAX_STRING);

		//	Go look for the starting poin of the string.
		//	if we do NOT find it return STR_NOTFOUND
		string = STRSTR(internalBuffer, upperFind);
	}
	
	return (NULL == string) ? STR_NOTFOUND : (int)(string - internalBuffer);
	}	//	::Find.

int ACLStr::Find
(
	IN const TCHAR	charToFind,
	IN BOOL			caseSensitive/*=TRUE*/
)
	{
	//				ch			Case		   find reverse?
	return (_FindCh(charToFind, caseSensitive, FALSE));
	}	//	::Find.


int ACLStr::FindLast
(
	IN const TCHAR	charToFind,
	IN BOOL			caseSensitive/*=TRUE*/
)
{
	//				ch			Case		   find reverse?
	return (_FindCh(charToFind, caseSensitive, TRUE));
}	//	::FindLast.


BOOL ACLStr::Compare
(
	IN const ACLStr&	other,
	IN BOOL				caseSensitive/*=TRUE*/
) const
{
	if (other.Len() > 0)
	{
		if (caseSensitive)
		{
			return (STRCMP((LPCTSTR)other, _string) == 0);	
		}
		else
		{
			return (STRICMP((LPCTSTR)other, _string) == 0);	
		}
	}

	return FALSE;
} // Compare

BOOL ACLStr::Compare
(
	IN LPCTSTR	other,
	IN BOOL		caseSensitive/*=TRUE*/
) const
{
	if (other)
	{
		if (caseSensitive == TRUE)
		{
			return (STRCMP(other, _string) == 0);	
		}
		else
		{
			return (STRICMP(other, _string) == 0);	
		}
	}

	return FALSE;
} // Compare

// P R I V A T E   M E T H O D S
int ACLStr::_FindCh
(
	IN const TCHAR	charToFind,
	IN BOOL			caseSensitive	/*=TRUE*/,
	IN BOOL			reverseFind		/*=FALSE*/
)
{
	LPTSTR	string	= NULL;
	TCHAR	ib[MAX_STRING];
	
	//	Initialize this to the
	//	internal buffer.
	LPTSTR	ibPtr		= _string;
	TCHAR	chToFind	= charToFind;

	//	If the internal string is NULL or the lenght is
	//	zero then we will just bail and say we couldn't
	//	find the string we were searching for.
	if (ibPtr != NULL || 0 != _length)
	{
		if (caseSensitive == TRUE)
		{
			chToFind = TOUPPER(charToFind);

			//	Now copy the internal ACLStr buffer to the new
			//	tempory Upper case version.
			if (_string)
			{
				// NOTE: STRCPY_S(target, length, source) - The length parameter is for
				// the length of the target string, not the source string. 
				STRCPY_S(ib, MAX_STRING, _string); 
				AnsiUpper(ib);
				ibPtr = ib;
			}
		}	//	case sensitive search?

		//	Go look for the starting point of the string.
		//	if we do NOT find it return STR_NOTFOUND
		if (reverseFind == TRUE)
		{
			string = STRRCHR(ibPtr, chToFind);
		}
		else
		{
			string = STRCHR(ibPtr, chToFind);
		}
	}	//	 search string NULL?

	return (string == NULL) ? STR_NOTFOUND : (int)(string - ibPtr);
	}	//	::_FindCh

ACLStr ACLStr::Replace
(
	IN const TCHAR replace,
	IN const TCHAR replaceWith
)
{
	ACLStr replaceStr;
	LPTSTR thisString = CharFirst();

	//	While we have something to work with
	//	continue.
	while (thisString != NULL)
	{
		if (*thisString != replace)
		{
			replaceStr.Append(*thisString);
		}
		else
		{
			replaceStr.Append(replaceWith);
		}

		//	Grab the next character in the string.
		//	use CharNext so we'll be wide string
		//	aware
		thisString = CharNext();
	}

	return replaceStr;
} // Replace

ACLStr ACLStr::Strip(IN const TCHAR chToStrip)
{
	ACLStr tmpStr;
	LPTSTR thisString = CharFirst();

	//	While we have something to work with
	//	continue.
	while (thisString != NULL)
	{
		if (*thisString != chToStrip)
		{
			tmpStr.Append(*thisString);
		}
		thisString = CharNext();
	}

	return tmpStr;
} // Strip

ACLStr ACLStr::StripWord(IN const ACLStr& strToStrip)
{
    ACLStr newString;
    if (strToStrip.Len())
    {
        size_t lenStrToStrip(strToStrip.Len());
        
        LPTSTR strBuffer(_string);
        
        LPTSTR findString(NULL);
        while ((findString = STRSTR(strBuffer, (LPCTSTR)strToStrip)))
        {
            newString.Append(strBuffer, (findString - strBuffer));
            strBuffer = (findString + lenStrToStrip);
        }
        if (strBuffer) newString.Append(strBuffer);
    }
    return newString;
} // StripWord

VOID ACLStr::RTrim(VOID)
{
	LPTSTR string			= CharFirst();
	LPTSTR lastWhiteSpace	= NULL;

	//	Loop over the string looking
	//	for white spaces. We start at the
	//	front since we want to be DBCS compatible.
	//	At least that's the HOPE! :-)
	while(*string != '\0')
	{
		if (*string == CH_SPACE)
		{
			if (lastWhiteSpace == NULL)
			{
				lastWhiteSpace = string;
			}
		}
		else
			lastWhiteSpace = NULL;

		//	Bump our current pointer to the next
		//	character in the stream.
		//
		string = CharNext();
	}	//	while.

	//	Move the NULL terminator to the
	//	new location.
	if (lastWhiteSpace != NULL)
	{
		*lastWhiteSpace	= '\0';
		_length			= (lastWhiteSpace - _string);
	}
} // RTrim

VOID ACLStr::LTrim(VOID)
{
	LPTSTR string = CharFirst();

	//	Loop over the string looking
	//	for white spaces. We start at the
	//	front since we want to be DBCS compatible.
	//	At least that's the HOPE! :-)
	while(string != NULL && *string == CH_SPACE)
	{
		string = CharNext();
	}	//	while.

	//	Move the NULL terminator to the
	//	new location.
	if (string != NULL)
	{
		size_t size = _length - (string - _string);
		MEMMOVE(_string, string, size);
		_string[size] = '\0';				//	BUG: This is not WIDE SOUND!
		_length = (size-1);
	}
} // LTrim

VOID ACLStr::Trim(VOID)
{
	RTrim();
	LTrim();
} // Trim

#if defined(IS_MAC)
int LoadString(IN HINSTANCE hInst, IN UINT resId, OUT LPTSTR pszBuffer, int cbBuffer)
{
    // THIS DOES NOTHING!!!
    return 1;
}
#endif //NOT IS_WINDOWS

BOOL ACLStr::LoadString(IN UINT resId)
{
	TCHAR	tmpBuffer[MAX_STRING];
	int		len = ::LoadString(NULL, resId, tmpBuffer, sizeof(tmpBuffer));

	//	ROB: If we fail to load the string should the buffer
	//	be cleared(recycle'd) anyway or should it just be
	//	left alone?
	//
	if (len > 0)
	{
		_Recycle();
		_Concat(len, tmpBuffer);
	}
	
	return (len > 0);
}

LPTSTR ACLStr::CharFirst(VOID)
{
	_currStr = _string;
	return _currStr;
} // CharFirst

LPTSTR ACLStr::CharNext(VOID)
{
#if defined(IS_WINDOWS)
	LPTSTR currStr = NULL;

	currStr = CHARNEXT(_currStr);

	//	According to the Win32 SDK CharNext will
	//	return the same pointer when it reaches the
	//	end of the string.
	if (currStr == _currStr)
	{
		currStr = NULL;
	}
	else
	{
		_currStr = currStr;
	}
	
	return currStr;
#else
    return ++_currStr;
#endif // IS_WINDOWS
} // CharNext


#if  !defined(NO_BSTRS)
BSTR ACLStr::MakeBstr(VOID)
{
	BSTR	bstr = 0;

	UINT	cch;
	LPWSTR	wideStr;

	UINT	codePage = GetACP();
	
	//	Pass zero for destination buffer size to get the actual size required.
	//	The returned size is expressed in number of wide characters, including
	//	the final null.
	cch = MultiByteToWideChar(codePage, MB_PRECOMPOSED, _string, -1, 0, 0);

	//	Now do the real work of converting the string.
	if (wideStr = new WCHAR[cch])
	{
		MultiByteToWideChar(codePage, MB_PRECOMPOSED, _string, -1, wideStr, cch);
		bstr = SysAllocString(wideStr);
        ARRAY_DELETE(wideStr);
	}

	return bstr;
}	//	::MakeBstr

VOID ACLStr::MakeLPTSTR(IN BSTR bStr)
{
	UINT	codePage	= GetACP();
	int     bufSize		= WideCharToMultiByte(codePage, 0, bStr, -1, NULL, 0, NULL, NULL);
	
	WideCharToMultiByte(codePage, 0, bStr, -1, _string, Len(), NULL, NULL);
}	//	::MakeLPTSTR

#endif	//	!defined(NO_BSTRS)

size_t ACLStr::Len(VOID) const
{
	return _length;
}

VOID ACLStr::Clear(VOID)
{
	//	_Recycle will just basically keep the current
	//	buffer we're using, which could be quite large,
	//	and reset the pointer to the beginning of the
	//	buffer so we can just start setting values into
	//	that buffer without reallocation. Maybe we should
	//	restrict the size of the initial buffer?
	_Recycle();
} // Clear

///////////////////////////////////////////////////////////////////////////////
//
// O P E R A T O R S
//
///////////////////////////////////////////////////////////////////////////////
const ACLStr& ACLStr::operator=(IN const TCHAR ch)
{
	_Recycle();
	_Concat(1, &ch);
	return *this;
}

const ACLStr& ACLStr::operator=(IN LPCTSTR str)
{
	if (this->_string != str)
	{
		_Recycle();
		_Concat(STRLEN(str)+1, str);
	}

	return *this;
}

#if defined(_WIN32) && !defined(NO_BSTRS)
const ACLStr& ACLStr::operator=(IN const BSTR bstr)
{
	_SafeInit();
	
	if (bstr)
	{
		UINT nStrLen = SysStringLen(bstr);
		
		_Alloc(nStrLen);
		MakeLPTSTR(bstr);
	}
	
	return *this;
}
#endif	//	_WIN32

const ACLStr& ACLStr::operator=(IN const ACLStr& str)
{
	//	Make sure we're not copying ourselves
	//
	if (this != &str)
	{
		_Recycle();
		_Concat(str.Len(), (LPCTSTR)str);
	}

	return *this;
}

//	+= implementations
//
const ACLStr& ACLStr::operator+=(IN const TCHAR ch)
{
	Append(ch);
	return *this;
}

const ACLStr& ACLStr::operator+=(IN LPCTSTR lpStr)
{
	Append(lpStr);
	return *this;
}

const ACLStr& ACLStr::operator+=(IN const ACLStr& str)
{
	//	Make sure we're not copying ourselves
	//
	if (this != &str)
	{
		Append(str);
	}

	return *this;
}


ACLStr operator+
(
	IN const ACLStr& str1,
	IN const ACLStr& str2
)
{
	ACLStr tmpStr;

	//	Do the copy here
	//
	tmpStr = str1;
	tmpStr.Append(str2);

	return tmpStr;
}

ACLStr::operator LPCTSTR() const
{
	return _string;
}	// Operator LPCTSTR

ACLStr::operator LPTSTR() const
{
	return _string;
}

//const ACLStr& ACLStr::operator[](IN int nOffset)
const TCHAR ACLStr::operator[](IN int nOffset)
{
	return GetAtOffset(nOffset);
}

BOOL ACLStr::operator==(const ACLStr& strOther) const
{
	return Compare(strOther/*, FSTR_CASESENSITIVE*/);
}

BOOL ACLStr::operator==(LPCTSTR pSzOther) const
{
	return Compare(pSzOther/*, FSTR_CASESENSITIVE*/);
}


// P R I V A T E   M E M B E R S

VOID ACLStr::_Init(VOID)
{
	_string		= NULL;
	_currStr	= NULL;			//	Used for iteration.
	_length		= 0;
	_realLength	= 0;
} // _Init

VOID ACLStr::_Concat
(
	IN size_t	nStrLen,
	IN LPCTSTR	pString
)
{
	if (nStrLen > 0 && 
		pString)
	{
		_Grow(nStrLen);
		STRNCAT_S(_string, _realLength, pString, nStrLen);
	}
}	//	::concat

VOID ACLStr::_Recycle(VOID)
	{
	//	We're leaving the allocated buffer
	//	alone in this case, we're going to
	//	lie and say that the data length is
	//	zero.
	_length	= 0;

	//	If we actually have a buffer then
	//	put a null on the end so our concat
	//	operations will work properly.
	if (_realLength > 0 && 
		_string)
	{
		_string[_length] = 0;
	}
} // _Recycle

VOID ACLStr::_SafeInit(VOID)
{
	//	If we're doing a copy on an existing
	//	buffer then we need to destroy it 
	//	before jamming anything into it.
	if (_string)
	{
		_Wack();
	}

	//	Null out values...
	//
	_Init();
} // _SafeInit

#define ALIGNSTRING(size)	((((size < 1) ? 1 : size) + (STR_PADDING-1)) / STR_PADDING) * STR_PADDING
VOID ACLStr::_Alloc(size_t size)
{
	_length = __Alloc(size, &_string, &_realLength);
} // _Alloc

VOID ACLStr::_Grow(IN const size_t appendSize)
{
	//	If the string doesn't need growing then
	//	just bail.
	if (appendSize) 
	{
		size_t  strSize(Len());

		//	Check to see if we've run out of room, remember the
		//	string is padded by xxx bytes. Where xxx is the next
		//	eight byte boundry
		if (_realLength <= (_length+appendSize))
		{
            size_t newRealLength(0);
            LPTSTR newString(NULL);
            size_t newLength(__Alloc(strSize+appendSize, &newString, &newRealLength));
            if (newString && newLength && newRealLength)
            {
                MEMSET(newString, 0, newRealLength);
                STRCPY_S(newString, newRealLength, _string);
                ARRAY_DELETE(_string);
                
                _string = newString;
                _length = newLength;
                _realLength = newRealLength;
            }
		}
		else
		{
			//	If the append size is less than the size of
			//	the REAL buffer size then don't worry about
			//	growing the buffer. Just tell this string
			//	that its' size is now larger.
			_length = (strSize+appendSize);
		}
	}
}	//	::grow

VOID ACLStr::_Wack(VOID)
{
	//	Use the real allocation length to determine
	//	if we should wack this string.
	//
	if (_realLength)
	{	
		ARRAY_DELETE(_string);
	}
} // _Wack

size_t ACLStr::_RealLen(VOID) const
{
	return _realLength;
}

VOID ACLStr::_UpperCase
(
	IN LPCTSTR		strToFind,
	IN OUT LPTSTR	upperFind,
	IN const int	sizeUpperFind,
	IN OUT LPTSTR	internalBuff,
	IN const int	sizeInternalBuff
)
{
	//	First copy the buffer sent to us to the new
	//	uppercase buffer.
	STRCPY_S(upperFind, sizeUpperFind, strToFind);
	AnsiUpper(upperFind);

	//	Now copy the internal ACLStr buffer to the new
	//	tempory Upper case version.
	if (_string)
	{
		STRCPY_S(internalBuff, sizeInternalBuff, _string);
		AnsiUpper(internalBuff);
	}
} // _UpperCase

//	Global helper function!
//
ACLStr LoadString
(
	IN HINSTANCE	hResourceModule,
	IN UINT			resID
)
{
	TCHAR	tmpBuffer[MAX_STRING];
	ACLStr	str;

	if (::LoadString(hResourceModule, resID, tmpBuffer, sizeof(tmpBuffer)))
	{
		str = (LPCTSTR)tmpBuffer;
	}
	
	return str;
}	//	LoadString

//static
size_t __Alloc(IN size_t size, OUT LPTSTR* ppBuffer, OUT size_t* pRealSize)
{
	//	Determine the size to allocate.
	*pRealSize = ALIGNSTRING(size);
	//ASSERT(size <= realSize);

	//	If the size requested falls on the eight byte boundry then pad it
	//	to the next eight byte boundry. This will prevent stepping on the
	//	NULL terminator and squashing somebody else's memory.
	*pRealSize = (*pRealSize == size) ? *pRealSize + STR_PADDING : *pRealSize;

	//	Allocate the memory we've requested.
	*ppBuffer = new TCHAR[*pRealSize];
		
	//	If the string is looking good then save off some info needed by
	//	operators and methods of the class and null terminate the string
	//	at the size + 1. We should be padded well enough to handle that.
	//
	if (*ppBuffer)
	{
		::ZeroMemory(*ppBuffer, *pRealSize);
	}

	return size;
}

#if defined(IS_MAC)
// Not defined on the Mac
int _vscprintf (const char * format, va_list pargs)
{
    int retval;
    va_list argcopy;
    va_copy(argcopy, pargs);
    retval = vsnprintf(NULL, 0, format, pargs);
    va_end(argcopy);
    return retval;
}
#endif // IS_MAC

static UINT __Format
(
	OUT LPTSTR* ppOutput,
	OUT UINT* pLen,
	IN LPCTSTR format, 
	IN va_list& args
)
{
	UINT len(0);

    if (format)
    {
		va_list newargs;
        va_copy(newargs, args);
        
		// Figure out the length. If we were given 
		// a valid pointer that's long enough, use it
		// otherwise we need to reallocate it.
		len = VSCPRINTF(format, newargs)+1;
		if (len > *pLen)
		{
			// Delete the buffer
            ARRAY_DELETE(*ppOutput);
			
			// Resize the buffer.
			size_t unused(0);
			__Alloc(len, ppOutput, &unused);
			*pLen = len;
		}

		// Fill the buffer.
		if (*ppOutput)
		{
			SPRINTFNS(*ppOutput, *pLen, format, newargs);
		}
    }
	//else
	//{
	//	YOU'RE GETTING A NULL POINTER BACK!
	//}

	return len;
}

//static
ACLStr ACLStr::Format(LPCTSTR format, ...)
{
	ACLStr output;
	if (format)
	{
		va_list args;
		va_start(args, format);

		// Caller owes a delete on outputPtr
		LPTSTR	outputPtr(NULL);
		UINT	outputLen(0);
		__Format(&outputPtr, &outputLen, format, args);
		va_end(args);

		// Ok, copy the string.
		output = outputPtr;

		// All done.
        ARRAY_DELETE(outputPtr);
	}

	return output;
}

