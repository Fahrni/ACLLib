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
#ifndef _ACLPLAT_H
#define _ACLPLAT_H

#if defined(WIN32)
    #define IS_WINDOWS  1
    #include "ACLWinTypes.h"
#elif defined(MACOSX)
    #define IS_MAC      1
    #include "ACLMacTypes.h"
#elif defined(LINUX)
    #define IS_LINUX    1
    #include "ACLLinuxTypes.h"
#else
    #pragma error "Please define a platform; WIN32, MACOSX, LINUX"
#endif // platform

//	String defines.
//
#define MAX_STRING		256
#define MID_SIZE		50
#define TITLE_SIZE		30
#define TINY_STRING		5

//	Helpful defines for characters.
//
#define CH_QUOTE		'"'
#define CH_SEMICOLON	';'
#define CH_EQ			'='
#define CH_NL			'\n'
#define CH_SPACE		32
#define CH_NOTHING		-1
#define CH_CR			10
#define CH_LF			13
#define CH_UNDERBAR		'_'
#define CH_MINUS		'-'
#define CH_PERIOD		'.'
#define CH_COMMA		','

#define STR_QUOTE		"\""
#define STR_SEMICOLON	";"
#define STR_EQ			"="
#define STR_NL			"\n"
#define STR_UNDERBAR	"_"
#define STR_MINUS		"-"
#define STR_PERIOD		"."
#define STR_COMMA		","


#endif
