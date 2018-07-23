#pragma once
#include <iostream>
#include <vector>
#include <cvt/wstring>
#include <codecvt>
using namespace std;
using namespace stdext::cvt;

class CURLCode
{
public:
	CURLCode(){};
	virtual ~CURLCode(){};
	static string Encode(const wchar_t* lpszData);
	static string MyEncode(const char* lpszData);
	static string Decode(string inTmp);
	static string myDecode(const wchar_t* lpszData);
    static string UTF8ToString(const std::string & utf8Data);
    static string UnicodeToAscii(const std::wstring str);
	static void StringToHex(BYTE *in, string& str, int nLength);

private:
	static inline BYTE toHex(const BYTE &x){ return x > 9 ? x + 55 : x + 48; }
	static inline BYTE fromHex(const BYTE &x){ return x > 64 ? x - 55 : x - 48; }
};
