//
//  main.cpp
//  simplemac
//
//  Created by Rob Fahrni on 7/14/13.
//  Copyright (c) 2013 Apple Core Labs. All rights reserved.
//
#include <iostream>
#include "ACLOutMsg.h"
#include "ACLStr.h"

void validate(ACLStr& source, LPCTSTR expected)
{
    if (source == expected)
    {
        printf("GOOD: \"%s\"\n", (LPTSTR)source);
    }
    else
    {
        printf("==> BAD:\n\tsource(\"%s\")\n\texpected(\"%s\")\n", (LPTSTR)source, expected);
    }
}

int main(int argc, const char * argv[])
{
    // Grow/Shrink Buffer
    printf("==> Test Grow/Shrink Buffer <==\n");
    ACLStr gs("Hello"); validate(gs, "Hello");
    gs.Append(" Rob"); validate(gs, "Hello Rob");
    gs.Append(" 45"); validate(gs, "Hello Rob 45");
    gs.Append(" you are an old guy"); validate(gs, "Hello Rob 45 you are an old guy");
    gs = "Blah"; validate(gs, "Blah");
    gs.Append(" Rob"); validate(gs, "Blah Rob");
    gs += " you're a big stinker"; validate(gs, "Blah Rob you're a big stinker");

    printf("\n==> Append Different Types <==\n");
    ACLStr str1("Hello, I'm a String, what are you?"); validate(str1, "Hello, I'm a String, what are you?");
    str1.Append(" I was appended."); validate(str1, "Hello, I'm a String, what are you? I was appended.");
    ACLStr str2("Hi, my name is str2"); validate(str2, "Hi, my name is str2");
    str1.Append(32); validate(str1, "Hello, I'm a String, what are you? I was appended. ");
    str1.Append(str2); validate(str1, "Hello, I'm a String, what are you? I was appended. Hi, my name is str2");
    str1.Append(" what who what how what which"); validate(str1, "Hello, I'm a String, what are you? I was appended. Hi, my name is str2 what who what how what which");
    
    // Sub-string creation.
    printf("\n==> Sub-String Creation <==\n");
    ACLStr src("0123456789a123456789b123456789c");
    ACLStr ss1(src.Left(5)); validate(ss1, "01234");
    ACLStr ss2(src.Right(7)); validate(ss2, "456789c");
    ACLStr ss3(src.Mid(5)); validate(ss3, "56789a123456789b123456789c");
    ACLStr ss4(src.Mid(5, 10)); validate(ss4, "56789a1234");
    
    // Strip
    src = "Who what when when why what";
    ACLStr st0("what"); ACLStr stripped(src.StripWord(st0)); validate(stripped, "Who  when when why ");
    src = "Whatwhatwhat1";
    stripped = src.StripWord(st0); validate(stripped, "What1");
    src = "Whatwhatwhat1what2what3";
    stripped = src.StripWord(st0); validate(stripped, "What123");
    stripped = src.StripWord("hat"); validate(stripped, "Www1w2w3");
    stripped = src.StripWord("1"); validate(stripped, "Whatwhatwhatwhat2what3");
    stripped = src.StripWord("2"); validate(stripped, "Whatwhatwhat1whatwhat3");
    stripped = src.StripWord("3"); validate(stripped, "Whatwhatwhat1what2what");
    src = "HELLOhelloHello";
    stripped = src.StripWord("hello"); validate(stripped, "HELLOHello");
    
    // Trim
    printf("\n=> Trim Test <=\n");
    ACLStr ts0("   string woo!    ");
    ts0.RTrim(); validate(ts0, "   string woo!");
    ACLStr ts1("   string woo!    ");
    ts1.LTrim(); validate(ts1, "string woo!    ");    
    ACLStr ts2("   string woo!    ");
    ts2.Trim();	validate(ts2, "string woo!");
    
    // Make Upper/Lower
    printf("\n=> Make Upper/Lower Test <=\n");
    ACLStr basic("Hello, I'm a basic string");
    basic.MakeUpper(); validate(basic, "HELLO, I'M A BASIC STRING");
    basic = "Hello, I'm a basic string";
    basic.MakeLower(); validate(basic, "hello, i'm a basic string");

    // BAD sub-strings
    printf("\n=> START: Bad sub-string\n");
    ACLStr bss1(str2.Left(str2.Len()+10));
    printf("bss1 = \"%s\"\n", (LPCTSTR)bss1);
    ACLStr bss2(str2.Right(400));
    printf("bss2 = \"%s\"\n", (LPCTSTR)bss2);
    ACLStr bss3(str2.Left(0));
    printf("bss3 = \"%s\"\n", (LPCTSTR)bss3);
    printf("<< END: Bad sub-string\n");
    
    // Find
    int f1 = str2.Find('m');
    printf("f1 location = %d in \"%s\"\n", f1, (LPCTSTR)str2);
    ACLStr f("was");
    int f2 = str2.Find(f);
    printf("f2 location = %d in \"%s\"\n", f2, (LPCTSTR)str2);
    int f3 = str2.Find('H', 0);
    printf("f3 location = %d in \"%s\"\n", f3, (LPCTSTR)str2);
    
//    printf("\n=> Make Upper/Lower Test <=\n");
//    f.FormatInternal("Internal format test: %d", 1); validate(f, "Internal format test: 1");
//    f.FormatInternal("Internal format test: %s", "One"); validate(f, "Internal format test: One");
//    ACLStr ff(ACLStr::Format("External format test: %d", 99)); validate(ff, "Internal format test: 99");
//    ACLStr ff2(ACLStr::Format("External format test: %s", "NineNine")); validate(ff2, "Internal format test: NineNine");
    
    return 0;
}

