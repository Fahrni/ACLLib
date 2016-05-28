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
#include "ACLPlat.h"
#include "ACLOutMsg.h"
#include <cstdio>

#ifdef _DEBUG

#define ACL_STATIC_BUFFER_LENGTH	4096

#define MSG_OUTPUT_TEMPLATE					"%s(%d): %s"
#define FORMAT_OUTPUT_STRING(b, s, f, l, m)	sprintf_s(b, s, MSG_OUTPUT_TEMPLATE, f, l, m)

//	Not exposed outside.
//
static void _ACLDebugOutputString(char* message)
{
	CRITICAL_SECTION critSection;
	::InitializeCriticalSection(&critSection);
	::EnterCriticalSection(&critSection);
	
	::OutputDebugStringA(message);

	::LeaveCriticalSection(&critSection);
	::DeleteCriticalSection(&critSection);
}


char* _ACLBuildMsg
(
	char* msg, 
	...
)
{
	static char	buf[ACL_STATIC_BUFFER_LENGTH*2];

	if (msg != NULL)
	{
		memset(buf, 0, ACL_STATIC_BUFFER_LENGTH*2);

		// handle variable extra stuff
		//
		va_list		ap;
	    va_start(ap, msg);
		vsprintf_s(buf, ACL_STATIC_BUFFER_LENGTH*2, msg, ap);
		va_end(ap);
	}

	return buf;
}	//	_ACLBuildMsg

void ACLDebugMsg
(
	char*	fileName, 
	int		lineNum, 
	char*	msg
)
{
	static char buf[ACL_STATIC_BUFFER_LENGTH*2];

	// 	Build the standard string, include the file and line # at
	//	the beginning of the template.
	//
	memset(buf, 0, sizeof(buf));
	FORMAT_OUTPUT_STRING(buf, ACL_STATIC_BUFFER_LENGTH*2, fileName, lineNum, msg);

	//	Write the message to the display.
	//
	_ACLDebugOutputString(buf);
}	//	ACLDebugMsg

#endif	//	_DEBUG
