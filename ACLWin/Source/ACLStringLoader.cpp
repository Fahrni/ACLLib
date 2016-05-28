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
#include "ACLStringLoader.h"
#include "ACLHeap.h"

#if defined(_DEBUG)
    #define DEBUG_CHK_HEAP(h)   ::HeapValidate(_hHeap, 0, pBuffer)
    #define CLOSING_STATS()     _Dump()
#else
    #define DEBUG_CHK_HEAP(h)   (NULL == pBuffer)
    #define CLOSING_STATS()     //
#endif

static int const INITIAL_STRING_BUFFERS = 10;
static int const MAX_STRING_BUFFERS     = 11;

// Global helpers.
static HINSTANCE gbl_hInstance = NULL;
#if !defined(AFX)
HINSTANCE AfxGetResourceHandle()
{
    return gbl_hInstance;
}
#endif

void SetResourceHandle(HINSTANCE hInstance)
{
    gbl_hInstance = hInstance;
}

//
// _StringHeap
//
class _StringHeap :
    public ACLHeap
{
public:
    static _StringHeap* Instance();
    
    _StringHeap();
    virtual ~_StringHeap();
}; // _StringHeap

//
// _StringHeap Implementation
//

// Singleton helper.
// static
_StringHeap* _StringHeap::Instance()
{
    static _StringHeap sh;
    return &sh;
}

_StringHeap::_StringHeap() :
    ACLHeap()
{
}

_StringHeap::~_StringHeap()
{
}

///////////////////////////////////////////////////////////////////////////////
//
// ACLStringLoader Implementation
//
///////////////////////////////////////////////////////////////////////////////

ACLStringLoader::ACLStringLoader() :
    _buffer(NULL)
{
}

ACLStringLoader::ACLStringLoader(IN UINT id) :
    _buffer(NULL)
{
    this->Load(id);
}

ACLStringLoader::~ACLStringLoader()
{
    if (_buffer)
    {
        _StringHeap::Instance()->Unlock(_buffer);
    }
    _buffer = NULL;
}

ACLStringLoader::operator LPCTSTR() const
{
	return (LPCTSTR)_buffer;
}	// Operator LPCTSTR

ACLStringLoader::operator LPTSTR() const
{
	return (LPTSTR)_buffer;
}	// Operator LPCSTR

void ACLStringLoader::Load(IN UINT id)
{
    _buffer = _StringHeap::Instance()->Lock();
    if (_buffer)
    {
        ::LoadString(AfxGetResourceHandle(), id, (LPTSTR)_buffer, MAX_STRING);
    }
}
