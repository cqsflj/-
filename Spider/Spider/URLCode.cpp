#include "stdafx.h"
#include "URLCode.h"


//byte* 转字符串
void CURLCode::StringToHex(BYTE *in, string& str, int nLength)
{
	BYTE high, low;
	while (nLength-- > 0)
	{
		high = (*in) >> 4;
		low = (*in) & 0xF;
		str += high > 9 ? high - 10 + 'a' : high + '0';
		str += low > 9 ? low - 10 + 'a' : low + '0';
		in++;
	}
	str += '\0';
}

string CURLCode::Encode(const wchar_t* lpszData)
{
	string strResult = "";

	unsigned char* pInTmp = (unsigned char*)lpszData;
	// do encoding
	while (*pInTmp)
	{
		if (isalnum(*pInTmp))
			strResult += *pInTmp;
		else
		if (isspace(*pInTmp))
			strResult += '+';
		else
		{
			strResult += '%';
			strResult += toHex(*pInTmp >> 4);
			strResult += toHex(*pInTmp % 16);
		}
		pInTmp++;
	}
	return strResult;
}

string CURLCode::MyEncode(const char* lpszData)
{
	string strResult = "";

	unsigned char* pInTmp = (unsigned char*)lpszData;
	// do encoding
	while (*pInTmp)
	{
		if (isalnum(*pInTmp))
			strResult += *pInTmp;
		else
		if (isspace(*pInTmp))
			strResult += '+';
		else
		{
			strResult += "%25";
			strResult += toHex(*pInTmp >> 4);
			strResult += toHex(*pInTmp % 16);
		}
		pInTmp++;
	}
	return strResult;
}

string CURLCode::Decode(string inTmp)
{
	string strResult = "";
	int inlen = inTmp.length();
	for (int i = 0; i<inlen; i++)
	{
		if (inTmp.at(i) == '%')
		{
			i++;
			char c = fromHex(inTmp.at(i++));
			c = c << 4;
			c += fromHex(inTmp.at(i));
			strResult += c;
		}
		else if (inTmp.at(i) == '+')
			strResult += ' ';
		else
			strResult += inTmp.at(i);
	}
	return strResult;
}

string CURLCode::myDecode(const wchar_t* lpszData)
{
	string strResult = "";
	int nLength = wcslen(lpszData) * 2 + 1;

	unsigned char* pInTmp = (unsigned char*)lpszData;

	while (*pInTmp)
	{
		strResult += *pInTmp;

		pInTmp++;
		pInTmp++;
	}

	return strResult;
}
std::string CURLCode::UnicodeToAscii(const std::wstring str)
{
    int	iTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
    std::vector<char> vecText(iTextLen, '\0');
    ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, &(vecText[0]), iTextLen, NULL, NULL);

    std::string strText = &(vecText[0]);

    return strText;
}

std::string CURLCode::UTF8ToString(const std::string & utf8Data)
{
    //先将UTF-8转换成Unicode
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::wstring wString = conv.from_bytes(utf8Data);
    //在转换成string
    return UnicodeToAscii(wString);
}
