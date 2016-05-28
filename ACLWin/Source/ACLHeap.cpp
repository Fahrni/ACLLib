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
#include "ACLHeap.h"

#if defined(_DEBUG)
    #define DEBUG_CHK_HEAP(h)   ::HeapValidate(_hHeap, 0, pBuffer)
    #define CLOSING_STATS()     _Dump()
#else
    #define DEBUG_CHK_HEAP(h)   (NULL == pBuffer)
    #define CLOSING_STATS()     //
#endif

// Defaults, really good for a string buffer, not much else.
static int const MAX_BUFFER_SIZE    = 256;
static int const MIN_COUNT_BUFFERS  = 10;
static int const MAX_COUNT_BUFFERS  = 11;

ACLHeap::ACLHeap() :
    _objectSize(MAX_BUFFER_SIZE*sizeof(TCHAR))
{
    // This version of the constructor is basically around for
    // string heaps
    _Construct(0L,
        ((MAX_BUFFER_SIZE*sizeof(TCHAR))*MIN_COUNT_BUFFERS), 
        ((MAX_BUFFER_SIZE*sizeof(TCHAR))*MAX_COUNT_BUFFERS));
}

ACLHeap::ACLHeap
(
    IN DWORD objectSize,
    IN DWORD minSize,
    IN DWORD maxSize
) :
    _objectSize(objectSize)
{
    _Construct(0L, (minSize*_objectSize), (maxSize*_objectSize));
}

ACLHeap::ACLHeap
(
    IN DWORD heapOptions,
    IN DWORD objectSize,
    IN DWORD minSize,
    IN DWORD maxSize
) :
    _objectSize(objectSize)
{
    _Construct(heapOptions, (minSize*_objectSize), (maxSize*_objectSize));
}


ACLHeap::~ACLHeap()
{
    if (_hHeap)
    {
        CLOSING_STATS();
        ::HeapDestroy(_hHeap);
        _hHeap = NULL;
    }
}

LPBYTE ACLHeap::Lock()
{
    // Return the next available pointer. Yes, this can return
    // a NULL.
    return (LPBYTE)::HeapAlloc(_hHeap, 0, _objectSize);
}

void ACLHeap::Unlock(IN OUT LPBYTE pBuffer)
{
    // Make sure the buffer pointer isn't NULL and that
    // it's actually in our Heap. If not, it's a no-op.
    if (pBuffer &&
        ::HeapValidate(_hHeap, 0, pBuffer))
    {
        ::HeapFree(_hHeap, 0, (LPVOID)pBuffer);
    }
    // else, no-op, it's not a valid heap.
}

void ACLHeap::_Construct
(
    IN DWORD heapOptions,
    IN DWORD minSize,
    IN DWORD maxSize
)
{
    _hHeap = ::HeapCreate(heapOptions, minSize, maxSize);
}

#if defined(_DEBUG)
void ACLHeap::_Dump()
{
    PROCESS_HEAP_ENTRY processEntry;
    ::ZeroMemory(&processEntry, sizeof(processEntry));

    int numEntries(0);
    BOOL fContinue(TRUE);
    while(fContinue)
    {
        fContinue = ::HeapWalk(_hHeap, &processEntry);
        ++numEntries;
    }

    DWORD le(0L);
    if (FALSE == fContinue)
    {
        le = ::GetLastError();
    }
}
#endif // _DEBUG