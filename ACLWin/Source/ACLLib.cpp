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
#include "ACLLib.h"
#include "ACLOutMsg.h"

#if defined(USE_STL_STR)

#else

#endif // USE_STL_STR

ACLLib::ACLLib(IN const STRING& libName) :
	_modulePath(libName),
	_moduleHandle(NULL)
{
	//	@note - assert here on MAX_PATH <= _modulePath.size()
}


ACLLib::ACLLib(
	IN const char*	libName, 
	IN const UINT32	cbLibName) :
	_modulePath(libName),
	_moduleHandle(NULL)
{
	//	@note - assert here on MAX_PATH <= cbLibName
}

/*virtual*/ ACLLib::~ACLLib() throw()
{
	//	If we still have an hmodule we need to unload the
	//	library now.
	//
	if (NULL != _moduleHandle)
	{
		Unload();
	}
}

bool ACLLib::Load()
{
#if defined(IS_WINDOWS)
	_moduleHandle = ::LoadLibraryA(CHARPTR(_modulePath));
#else if defined(IS_LINUX)
	_moduleHandle = dlopen(_modulePath.c_str(), RTLD_LAZY)) == NULL)   
//#else
	//	@note - need error here, compile time error, no platform supported.
	//
#endif
	
	return (NULL != _moduleHandle);
}

bool ACLLib::Unload()
{
	bool fOk(false);

#if defined(IS_WINDOWS)
	fOk = (::FreeLibrary(_moduleHandle) ? true : false);
#else if defined(IS_LINUX)
	fOk = (bool)!dlclose(_moduleHandle);
//#else
	//	@note - need error here, compile time error, no platform supported.
	//
#endif

	_moduleHandle = NULL;
	
	return fOk;
}

FARPROC ACLLib::GetProcAddress(IN const STRING& procName)
{
	return ::GetProcAddress(_moduleHandle, CHARPTR(procName));
}

FARPROC ACLLib::GetProcAddress(IN const char* procName)
{
	return ::GetProcAddress(_moduleHandle, procName);
}

STRING ACLLib::LoadString(IN const UINT32 uid)
{
	STRING		result;
	char		buffer[MAX_PATH];
	UINT32		cbBuffer(MAX_PATH);
	
#if defined(IS_WINDOWS)
	if (0 != ::LoadString(_moduleHandle, uid, (LPTSTR)buffer, (int)cbBuffer))
	{
		result = buffer;
	}
	ELSEIF_DBGPrintf(("ACLLib::LoadString() failed, returning empty string!\n"));
//#else
	//	@note - need error here, compile time error, no platform supported.
	//
#endif
	
	return result;
}

HICON ACLLib::LoadIcon(IN const char* id)
{
#if defined(IS_WINDOWS)
	return ::LoadIcon(_moduleHandle, (LPCTSTR)id);
//#else
	//	@note - need error here, compile time error, no platform supported.
	//
#endif
}

//============================================================================
//  int ACLLib::MessageBox()
//
//  Displays a mesage box for user input.
//============================================================================
int ACLLib::MessageBox(
	IN HWND			hWnd,
	IN long			style,
	IN UINT32		resTitle,
	IN char*		formatString,
	IN ...)
{
	STRING	msgBuffer; 				//	The message buffer passed to MessageBox.
	UINT32	cbFmtString((UINT32)strlen(formatString));
	
	if (0 != cbFmtString)
	{
		va_list 		marker; 			//	Marker to next argument, see stdarg.h
		va_start(marker, formatString);

		#define __TMP_BUFF 32
		char temp[__TMP_BUFF];			//	Pointer to tmp conversion buffer.
		for (UINT32 loopCntr(0); 
			 loopCntr < cbFmtString; 
			 loopCntr++)
		{
			if (formatString[loopCntr] == '%')
			{
				loopCntr++;

				switch(formatString[loopCntr])
				{
					case 'd':		// decimal integer
						_itoa_s(va_arg(marker, int), temp, __TMP_BUFF, 10);
						msgBuffer += temp;
						break;

					case 'D':		// decimal long
						_ltoa_s(va_arg(marker, long), temp, __TMP_BUFF, 10);
						msgBuffer += temp;
						break;

					case 'x':		// hex integer
						_itoa_s(va_arg(marker, int), temp, __TMP_BUFF, 16);
						msgBuffer += temp;
						break;

					case 'X':		// hex long
						_ltoa_s(va_arg(marker, long), temp, __TMP_BUFF, 16);
						msgBuffer += temp;
						break;

					case 'b':		// binary integer
						_itoa_s(va_arg(marker, int), temp, __TMP_BUFF, 2);
						msgBuffer += temp;
						break;

					case 'B':		// binary long
						_ltoa_s(va_arg(marker, long), temp, __TMP_BUFF, 2);
						msgBuffer += temp;
						break;

					case 's':		// string
					case 'S':
						msgBuffer += va_arg(marker, char*);
						break;
                
					case 'r':		//	resource
					case 'R':
						msgBuffer += LoadString((UINT32)va_arg(marker, int));
						break;

					case '%':
					default:
						msgBuffer += formatString[loopCntr+1];
						break;
					}		// switch
				}
			else
			{
				//	If the character isn't a '%' just add it to the buffer.
				//
				msgBuffer += formatString[loopCntr];
			}	// if
		}	// for

		va_end(marker);
	}

	//	Finally get around to displaying the Message Box.
	//
	STRING title(LoadString(resTitle));
	return((int)::MessageBoxA(hWnd, CHARPTR(msgBuffer), CHARPTR(title), style));
}	//	MessageBox



ACLLib::operator HMODULE() const
{
	return _moduleHandle;
}	// HMODULE()

