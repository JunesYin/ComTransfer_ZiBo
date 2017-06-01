#pragma once


//

#include "stdafx.h"



#include <string>
#include <vector>

using std::string;
using std::vector;




enum CodeType
{
	CodeType_hex = 0,
	CodeType_asc
};
typedef enum CodeType CodeType;



template <class T>
const string convertToString(const T &valr);

const int hexStrToInt(const CString &val);


const CString convertAscStrToHexStr(const CString &val);

const CString convertHexStrToAscStr(const CString &val);

int str2Hex(const CString & val, char *data);



WCHAR * charToWchar(char *s);
char * WcharToChar(WCHAR *s);
WCHAR charToWchar(char ch);
char WcharToChar(WCHAR wch);


int separateString(const CString &val, const char separator, vector<CString> &vecName);

/*
获取当前设备的串口
*/
void EnumerateSerialPorts(vector<UINT> &ports);
//void EnumerateSerialPorts(CUIntArray& ports);