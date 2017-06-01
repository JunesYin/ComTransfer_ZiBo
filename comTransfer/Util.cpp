

#include "stdafx.h"
#include "Util.h"


#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>


using namespace std;


template <class T>
const string convertToString(const T &valr)
{
	string result;
	std::ostringstream ss;
	ss << val;
	ss >> result;
	return result;
}



const int hexCharToInt(const char c)
{
	if ('0' <= c && c <= '9') return c - 0x30;
	if ('A' <= c && c <= 'F') return c - 'A' + 10;
	if ('a' <= c && c <= 'f') return c - 'a' + 10;

	return 0x10;
}


const int hexStrToInt(const CString &val)
{
	if (val.IsEmpty())
	{
		return 0;
	}

	int rst = 0;
	for (int i = 0; i < val.GetLength(); ++i)
	{
		rst = rst * 16 + hexCharToInt(WcharToChar(val[i]));
	}

	return rst;
}


const CString convertAscStrToHexStr(const CString &val)
{
	if (val.GetLength() < 1)
	{
		return _T("");
	}

	CString rst;
	CString str = _T("0123456789ABCDEF");

	for (int i = 0; i < val.GetLength(); ++i)
	{
		int b = 0x0f & (val[i] >> 4);
		rst.AppendChar(str[b]);

		b = 0x0f & val[i];
		rst.AppendChar(str[b]);

		rst.AppendChar(' ');
	}

	return rst;
}


const CString convertHexStrToAscStr(const CString &val)
{
	if (val.GetLength() < 1)
	{
		return _T("");
	}

	CString rst;
	int i = 0, len = val.GetLength();
	while (i < len)
	{
		if (' ' == val[i])
		{
			++i;
			continue;
		}

		rst.AppendChar((char)((hexCharToInt( WcharToChar(val[i]) ) << 4) | hexCharToInt( WcharToChar(val[i+1]) )));

		i += 2;
	}

	return rst;
}



int str2Hex(const CString &val, char *data)
{
	int t, t1;
	int rlen = 0, len = val.GetLength();

	// data.setSize(len / 2)
	for (int i = 0; i < len; )
	{
		char l, h = WcharToChar(val[i]);
		if (' ' == h)
		{
			++i;
			continue;
		}

		++i;
		if (i > len)
		{
			break;;
		}

		l = WcharToChar(val[i]);
		t = hexCharToInt(h);
		t1 = hexCharToInt(l);
		if (16 == t || 16 == t1)
		{
			break;
		}
		else
		{
			t = t * 16 + t1;
		}
		++i;
		data[rlen] = (char)t;
		++rlen;
	}

	return rlen;
}



WCHAR * charToWchar(char *s)
{
	int len = MultiByteToWideChar(CP_ACP, 0, s, strlen(s), NULL, 0);
	WCHAR *rst = new WCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, s, strlen(s), rst, len);
	rst[len] = '\0';

	return rst;
}



char * WcharToChar(WCHAR *s)
{
	int len = WideCharToMultiByte(CP_ACP, 0, s, wcslen(s), NULL, 0, NULL, NULL);
	char *rst = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, s, wcslen(s), rst, len, NULL, NULL);
	rst[len] = '\0';

	return rst;
}



WCHAR charToWchar(char ch)
{
	char *tmp = new char[2];
	tmp[0] = ch;
	tmp[1] = '\0';

	WCHAR *rst = charToWchar(tmp);

	return rst[0];
}


char WcharToChar(WCHAR wch)
{
	WCHAR *tmp = new WCHAR[2];
	tmp[0] = wch;
	tmp[1] = '\0';

	char *rst = WcharToChar(tmp);

	return rst[0];
}


int separateString(const CString &val, const char separator, vector<CString> &vecName)
{
	if (val.IsEmpty())
	{
		return 0;
	}

	CString tmpVal = val;

	int idx = tmpVal.Find(separator);
	int count = 0;
	while (0 < idx)
	{
		vecName.push_back(tmpVal.Left(idx));
		count++;
		tmpVal.Delete(0, idx+1);
		idx = tmpVal.Find(separator);
	}
	
	vecName.push_back(tmpVal);
	count++;

	return count;
}


void EnumerateSerialPorts(vector<UINT> &ports)
{
	//Make sure we clear out any elements which may already be in the array
	//ports.RemoveAll();
	ports.clear();

	//Determine what OS we are running on
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bGetVer = GetVersionEx(&osvi);

	//On NT use the QueryDosDevice API
	if (bGetVer && (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT))
	{
		//Use QueryDosDevice to look for all devices of the form COMx. This is a better
		//solution as it means that no ports have to be opened at all.
		TCHAR szDevices[65535];
		DWORD dwChars = QueryDosDevice(NULL, szDevices, 65535);
		if (dwChars)
		{
			int i=0;

			for (;;)
			{
				//Get the current device name
				TCHAR* pszCurrentDevice = &szDevices[i];

				//If it looks like "COMX" then
				//add it to the array which will be returned
				int nLen = _tcslen(pszCurrentDevice);
				if (nLen > 3 && _tcsnicmp(pszCurrentDevice, _T("COM"), 3) == 0)
				{
					//Work out the port number
					int nPort = _ttoi(&pszCurrentDevice[3]);
					//ports.Add(nPort);
					ports.push_back(nPort);
				}

				// Go to next NULL character
				while(szDevices[i] != _T('\0'))
					i++;

				// Bump pointer to the next string
				i++;

				// The list is double-NULL terminated, so if the character is
				// now NULL, we're at the end
				if (szDevices[i] == _T('\0'))
					break;
			}
		}
		else
			TRACE(_T("Failed in call to QueryDosDevice, GetLastError:%d\n"), GetLastError());
	}
	else
	{
		//On 95/98 open up each port to determine their existence

		//Up to 255 COM ports are supported so we iterate through all of them seeing
		//if we can open them or if we fail to open them, get an access denied or general error error.
		//Both of these cases indicate that there is a COM port at that number. 
		for (UINT i=1; i<256; i++)
		{
			//Form the Raw device name
			CString sPort;
			sPort.Format(_T("\\\\.\\COM%d"), i);

			//Try to open the port
			BOOL bSuccess = FALSE;
			HANDLE hPort = ::CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
			if (hPort == INVALID_HANDLE_VALUE)
			{
				DWORD dwError = GetLastError();

				//Check to see if the error was because some other app had the port open or a general failure
				if (dwError == ERROR_ACCESS_DENIED || dwError == ERROR_GEN_FAILURE)
					bSuccess = TRUE;
			}
			else
			{
				//The port was opened successfully
				bSuccess = TRUE;

				//Don't forget to close the port, since we are going to do nothing with it anyway
				CloseHandle(hPort);
			}

			//Add the port number to the array which will be returned
			if (bSuccess)
				//ports.Add(i);
				ports.push_back(i);
		}
	}

	sort(ports.begin(), ports.end());
}


/*
void EnumerateSerialPorts(CUIntArray& ports)
{
	//Make sure we clear out any elements which may already be in the array
	ports.RemoveAll();

	//Determine what OS we are running on
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bGetVer = GetVersionEx(&osvi);

	//On NT use the QueryDosDevice API
	if (bGetVer && (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT))
	{
		//Use QueryDosDevice to look for all devices of the form COMx. This is a better
		//solution as it means that no ports have to be opened at all.
		TCHAR szDevices[65535];
		DWORD dwChars = QueryDosDevice(NULL, szDevices, 65535);
		if (dwChars)
		{
			int i=0;

			for (;;)
			{
				//Get the current device name
				TCHAR* pszCurrentDevice = &szDevices[i];

				//If it looks like "COMX" then
				//add it to the array which will be returned
				int nLen = _tcslen(pszCurrentDevice);
				if (nLen > 3 && _tcsnicmp(pszCurrentDevice, _T("COM"), 3) == 0)
				{
					//Work out the port number
					int nPort = _ttoi(&pszCurrentDevice[3]);
					ports.Add(nPort);
				}

				// Go to next NULL character
				while(szDevices[i] != _T('\0'))
					i++;

				// Bump pointer to the next string
				i++;

				// The list is double-NULL terminated, so if the character is
				// now NULL, we're at the end
				if (szDevices[i] == _T('\0'))
					break;
			}
		}
		else
			TRACE(_T("Failed in call to QueryDosDevice, GetLastError:%d\n"), GetLastError());
	}
	else
	{
		//On 95/98 open up each port to determine their existence

		//Up to 255 COM ports are supported so we iterate through all of them seeing
		//if we can open them or if we fail to open them, get an access denied or general error error.
		//Both of these cases indicate that there is a COM port at that number. 
		for (UINT i=1; i<256; i++)
		{
			//Form the Raw device name
			CString sPort;
			sPort.Format(_T("\\\\.\\COM%d"), i);

			//Try to open the port
			BOOL bSuccess = FALSE;
			HANDLE hPort = ::CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
			if (hPort == INVALID_HANDLE_VALUE)
			{
				DWORD dwError = GetLastError();

				//Check to see if the error was because some other app had the port open or a general failure
				if (dwError == ERROR_ACCESS_DENIED || dwError == ERROR_GEN_FAILURE)
					bSuccess = TRUE;
			}
			else
			{
				//The port was opened successfully
				bSuccess = TRUE;

				//Don't forget to close the port, since we are going to do nothing with it anyway
				CloseHandle(hPort);
			}

			//Add the port number to the array which will be returned
			if (bSuccess)
				ports.Add(i);
		}
	}
}
*/



