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
#include "ACLThread.h"

// Win32 Thread Procedure.
DWORD WINAPI ACLThreadProc(IN LPVOID aclThread);

ACLThread::ACLThread(void) :
	_threadHandle(NULL),
	_threadId(0L),
	_running(false)
{
}

// virtual
ACLThread::~ACLThread(void)
{
	Stop();
}

void ACLThread::Start()
{
	_running = true;
	_threadHandle = ::CreateThread( 
									NULL,           // default security attributes
									0,              // use default stack size  
									ACLThreadProc,	// thread function name
									this,			// argument to thread function 
									0,              // use default creation flags 
									&_threadId);	// returns the thread identifier 
}

void ACLThread::Stop()
{
	_running = false;
	_Cleanup();
}

DWORD ACLThread::ThreadProc()
{
	DWORD result(0L);
	//while(_running)
	//{
	//}
	_running = false;
	return result;
}

void ACLThread::_Cleanup()
{
	if (_threadHandle)
	{
		// Wait on our thread to complete before we terminate. If 
		// the thread has been signaled, we're done.
		::WaitForSingleObject(_threadHandle, INFINITE);

		// Once the thread has stopped cleanup the thread handle.
		::CloseHandle(_threadHandle);
	}
	_threadHandle = NULL;
}

// Global - Thread Callback
DWORD WINAPI ACLThreadProc(IN LPVOID aclThread) 
{ 
	DWORD result(0L);
	// 
	ACLThread* thread = (ACLThread*)aclThread;
	if (thread)
	{
		result = thread->ThreadProc();
	}

    return result; 
} 