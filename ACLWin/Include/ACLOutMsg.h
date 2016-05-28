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
#ifndef _ACLOUTMSG_H
#define _ACLOUTMSG_H

#ifdef _DEBUG
	#define DBGPrintf(m)			ACLDebugMsg(__FILE__, __LINE__, _ACLBuildMsg m);
	#define ELSEIF_DBGPrintf(m) 	else { ACLDebugMsg(__FILE__, __LINE__, _ACLBuildMsg m); }

	//	This function is really only for use in the above macros.
	//
	char* _ACLBuildMsg(char* msg, ...);
	
	//	This method is used to spit out debug messages to the console/debug window,
	//	use the DBGPrintf and ELSEIF_DBGPrintf macros above in your code.
	//
	void ACLDebugMsg(char* pFileName, int lineNum, char* msg);
#else
	#define DBGPrintf(m)
	#define ELSEIF_DBGPrintf(m)
#endif	//	DEBUG

#endif	//	_ACLOUTMSG_H
