//	Copyright (c) 2013-15, Robert R. Fahrni
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

#define SAFE_DELETE(o) if (o) { delete o; o = 0; }

class ACLRefCounter
{
public:
	ACLRefCounter() : _cnt(0) {}

	long AddRef()
	{
		return ::InterlockedIncrement(&_cnt);
	}

	long Release()
	{
		long count(::InterlockedDecrement(&_cnt));
		if (0L == count)
		{
			delete this;
		}
		//char d[25];
		//::OutputDebugStringA("=> Object Count: "); ::OutputDebugStringA(_itoa(count, d, 10)); OutputDebugStringA("\n");
		return count;
	}

	long Count() const
	{
		return _cnt;
	}

private:
	long _cnt;
};	// ACLRefCounter

template<class T>
class ACLRef
{
public:
	ACLRef(T* pointer = NULL) :
		_pointer(NULL),
		_counter(NULL)
	{
		_InternalAssign(pointer, new ACLRefCounter());
	}

	ACLRef(const ACLRef<T>& other) :
		_pointer(NULL),
		_counter(NULL)
	{
		_InternalAssign(other._pointer, other._counter);
	}

	virtual ~ACLRef()
	{
		_InternalRelease();
	}

	T& operator*() const
	{
		//assert(_ptr && _counter);
		return *_pointer;
	}

	T* operator->() const
	{
		//assert(_ptr && _counter);
		return _pointer;
	}

	ACLRef<T>& operator=(T* pointer)
	{
		if (pointer)
		{
			_InternalRelease();
			_InternalAssign(pointer, new ACLRefCounter());
		}
		return *this;
	}

	ACLRef<T>& operator=(const ACLRef<T>& other)
	{
		if (&other != this)
		{
			_InternalRelease();
			_InternalAssign(other._pointer, other._counter);
		}
		return *this;
	}

	T* Pointer()
	{
		return _pointer;
	}

	ACLRef<T>& Clear()
	{
		_InternalRelease();
		return *this;
	}

	long Count() const
	{
		return (_counter) ? _counter->Count() : 0;
	}

private:
	void _InternalAssign(T* pointer, ACLRefCounter* counter)
	{
		// ASSERT(pointer)
		// ASSERT(counter)

		// AddRef() right away.
		counter->AddRef();

		// Assign our pointer and counter
		_pointer = pointer;
		_counter = counter;
	}

	void _InternalRelease()
	{
		if (_counter &&
			_counter->Release() == 0)
		{
			//::OutputDebugStringA("==> DESTROYING CONTAINED OBJECT!\n");
			
			// It's safe to delete our contained object, ref count has 
			// reached zero.
			SAFE_DELETE(_pointer);
		}

		// Resetting this is ok, its scope is controlled by
		// AddRef() and Release();
		_counter = 0;
	}

	// Data
	ACLRefCounter*	_counter;
	T*			_pointer;
}; // ACLRef<T>
