#include "stdafx.h"
#include "CppUnitTest.h"
#include "ACLStr.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ACLLibUnits
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(StringWithLPCTSTR)
		{
			ACLStr str1("Hello, I'm a String, what are you?");
			// Hello, I'm a String, what are you?
			Assert::AreEqual(str1, "Hello, I'm a String, what are you?");
		}

		TEST_METHOD(AppendLPCTSTR)
		{
			ACLStr str1("Hello, I'm a String, what are you?");
			str1.Append(" I was appended.");
			// Hello, I'm a String, what are you? I was appended.
			Assert::AreEqual(str1, "Hello, I'm a String, what are you? I was appended.");
		}

		TEST_METHOD(AppendSpaceCharacter)
		{
			ACLStr str1("Hello, I'm a String, what are you?");
			str1.Append(32);
			// "Hello, I'm a String, what are you? "
			Assert::AreEqual(str1, "Hello, I'm a String, what are you? ");
		}

		TEST_METHOD(LeftString)
		{
			ACLStr str1("Hello, I'm a String, what are you?");
			ACLStr left(str1.Left(5));
			// Hello
			Assert::AreEqual(left, "Hello");
		}

		TEST_METHOD(RightString)
		{
			ACLStr str1("Hello, I'm a String, what are you?");
			ACLStr right(str1.Right(4));
			// you?
			Assert::AreEqual(right, "you?");
		}

		TEST_METHOD(MidString)
		{
			ACLStr str1("Hello, I'm a String, what are you?");
			ACLStr mid(str1.Mid(6));
			// I'm a String, what are you?
			Assert::AreEqual(mid, " I'm a String, what are you?");
		}

		TEST_METHOD(MidStringCount)
		{
			ACLStr str1("Hello, I'm a String, what are you?");
			ACLStr mid(str1.Mid(6, 12));
			// I'm a String
			Assert::AreEqual(mid, " I'm a Strin");
		}

		TEST_METHOD(BadSubStringLeftLenPlus10)
		{
			ACLStr str("Hello, I'm a String, what are you?");
			ACLStr bss(str.Left(str.Len()+10));
			// bss should be empty
			Assert::AreEqual(bss.Len(), (size_t)0);
		}

		TEST_METHOD(BadSubStringRight400)
		{
			ACLStr str("Hello, I'm a String, what are you?");
			ACLStr bss(str.Right(400));
			// bss should be empty
			Assert::AreEqual(bss.Len(), (size_t)0);
		}

		TEST_METHOD(BadSubStringLeft0)
		{
			ACLStr str("Hello, I'm a String, what are you?");
			ACLStr bss(str.Left(0));
			// bss should be empty
			Assert::AreEqual(bss.Len(), (size_t)0);
		}

		TEST_METHOD(FindLetterM)
		{
			ACLStr str("Hello, I'm a String, what are you?");
			int idx = str.Find('m');
			// idx should be 9
			Assert::AreEqual(idx, 9);
		}

		TEST_METHOD(FindWhatInString)
		{
			ACLStr str("Hello, I'm a String, what are you?");
			ACLStr find("what");

			int idx = str.Find(find);
			// idx should be 21
			Assert::AreEqual(idx, 21);
		}

		TEST_METHOD(FindHCaseInsensitive)
		{
			ACLStr str("Hello, I'm a String, what are you?");
			int idx = str.Find('H', 0);
			// idx should be 0
			Assert::AreEqual(idx, 0);
		}

		TEST_METHOD(StripWhatFromString)
		{
			ACLStr str("Hello, I'm a String, what are you?");
			ACLStr str2(str.StripWord("what"));
			// Hello, I'm a String,  are you?
			Assert::AreEqual(str2, "Hello, I'm a String,  are you?");
		}

		TEST_METHOD(RTrimString)
		{
			ACLStr str("     Hello     ");
			str.RTrim();
			// "     Hello"
			Assert::AreEqual(str, "     Hello");
		}

		TEST_METHOD(LTrimString)
		{
			ACLStr str("     Hello     ");
			str.LTrim();
			// "Hello     "
			Assert::AreEqual(str, "Hello     ");
		}

		TEST_METHOD(TrimString)
		{
			ACLStr str("     Hello     ");
			str.Trim();
			// "Hello"
			Assert::AreEqual(str, "Hello");
		}

		TEST_METHOD(MakeUpperString)
		{
			ACLStr str("Hello, I'm a string.");
			str.MakeUpper();
			// "HELLO, I'M A STRING."
			Assert::AreEqual(str, "HELLO, I'M A STRING.");
		}

		TEST_METHOD(MakeLowerString)
		{
			ACLStr str("Hello, I'm a string.");
			str.MakeLower();
			// "hello, i'm a string."
			Assert::AreEqual(str, "hello, i'm a string.");
		}

		TEST_METHOD(AppendBuffer)
		{
			ACLStr str("Hello");
			str.Append(" Rob");
			str.Append(" 45");
			str.Append(" you are an old guy");
			// "Hello Rob 45 you are an old guy"
			Assert::AreEqual(str, "Hello Rob 45 you are an old guy");
		}

		TEST_METHOD(AppendBufferDifferentTypes)
		{
			ACLStr str("Hello");
			str.Append(" Rob");
			str.Append(32);
			ACLStr s2("45");
			str.Append(s2);
			str.Append(" you are an old guy");
			// "Hello Rob 45 you are an old guy"
			Assert::AreEqual(str, "Hello Rob 45 you are an old guy");
		}
	};
}

/*
// Grow/Shrink Buffer
{
DBGPrintf(("\n==> Append Different Types <==\n"));
ACLStr str1("Hello, I'm a String, what are you?"); validate(str1, "Hello, I'm a String, what are you?");
str1.Append(" I was appended."); validate(str1, "Hello, I'm a String, what are you? I was appended.");
ACLStr str2("Hi, my name is str2"); validate(str2, "Hi, my name is str2");
str1.Append(32); validate(str1, "Hello, I'm a String, what are you? I was appended. ");
str1.Append(str2); validate(str1, "Hello, I'm a String, what are you? I was appended. Hi, my name is str2");
str1.Append(" what who what how what which"); validate(str1, "Hello, I'm a String, what are you? I was appended. Hi, my name is str2 what who what how what which");

// Sub-string creation.
DBGPrintf(("\n==> Sub-String Creation <==\n"));
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
DBGPrintf(("\n=> Trim Test <=\n"));
ACLStr ts0("   string woo!    ");
ts0.RTrim(); validate(ts0, "   string woo!");
ACLStr ts1("   string woo!    ");
ts1.LTrim(); validate(ts1, "string woo!    ");
ACLStr ts2("   string woo!    ");
ts2.Trim();	validate(ts2, "string woo!");

// Make Upper/Lower
DBGPrintf(("\n=> Make Upper/Lower Test <=\n"));
ACLStr basic("Hello, I'm a basic string");
basic.MakeUpper(); validate(basic, "HELLO, I'M A BASIC STRING");
basic = "Hello, I'm a basic string";
basic.MakeLower(); validate(basic, "hello, i'm a basic string");

// BAD sub-strings
DBGPrintf(("\n=> START: Bad sub-string\n"));
ACLStr bss1(str2.Left(str2.Len()+10));
DBGPrintf(("bss1 = \"%s\"\n", (LPCTSTR)bss1));
ACLStr bss2(str2.Right(400));
DBGPrintf(("bss2 = \"%s\"\n", (LPCTSTR)bss2));
ACLStr bss3(str2.Left(0));
DBGPrintf(("bss3 = \"%s\"\n", (LPCTSTR)bss3));
DBGPrintf(("<< END: Bad sub-string\n"));

// Find
int f1 = str2.Find('m');
DBGPrintf(("f1 location = %d in \"%s\"\n", f1, (LPCTSTR)str2));
ACLStr f("was");
int f2 = str2.Find(f);
DBGPrintf(("f2 location = %d in \"%s\"\n", f2, (LPCTSTR)str2));
int f3 = str2.Find('H', 0);
DBGPrintf(("f3 location = %d in \"%s\"\n", f3, (LPCTSTR)str2));
}
//    printf("\n=> Make Upper/Lower Test <=\n");
//    f.FormatInternal("Internal format test: %d", 1); validate(f, "Internal format test: 1");
//    f.FormatInternal("Internal format test: %s", "One"); validate(f, "Internal format test: One");
//    ACLStr ff(ACLStr::Format("External format test: %d", 99)); validate(ff, "Internal format test: 99");
//    ACLStr ff2(ACLStr::Format("External format test: %s", "NineNine")); validate(ff2, "Internal format test: NineNine");
{
ACLString string1("String #1");
DBGPrintf(("string1 = \"%s\"\n", (LPCTSTR)string1));
ACLString string2(string1);
DBGPrintf(("string2 = \"%s\"\n", (LPCTSTR)string2));
ACLString string3(string2);
DBGPrintf(("string3 = \"%s\"\n", (LPCTSTR)string3));

string2 += " wacka, wacka, wacka";
DBGPrintf(("string2 POST change = \"%s\"\n", (LPCTSTR)string2));

ACLString string4("String 4, completey new");
DBGPrintf(("string4 = \"%s\"\n", (LPCTSTR)string4));
string4 += " wacka, wacka, WACKA";
DBGPrintf(("string4 POST change = \"%s\"\n", (LPCTSTR)string4));
}
*/