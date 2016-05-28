//	Copyright (c) 2015, Robert R. Fahrni
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
#include "ACLString.h"
#include "ACLOutMsg.h"
//#include <stdio.h>

ACLString::ACLString()
{
	_string = new ACLStr();
}

ACLString::ACLString(IN const LPCTSTR chars)
{
	_string = new ACLStr(chars);
}

ACLString::ACLString(IN const ACLString& other)
{
	// Does an AddRef on other._string
	_string = other._string;
}

ACLString::~ACLString()
{
	_string.Clear();
}

const ACLString& ACLString::operator+=(IN LPCTSTR string)
{
	_BreakAndCloneInternalIfNecessary();
	_string->Append(string);
	return *this;
}

ACLString::operator LPCTSTR() const
{
	return *_string;
}

ACLString::operator LPTSTR() const
{
	return *_string;
}

// P R I V A T E   M E T H O D S

inline void ACLString::_BreakAndCloneInternalIfNecessary()
{
	if (_string.Count() > 1)
	{
		ACLStr* s = new ACLStr(*_string);
		_string = s;
	}
}
