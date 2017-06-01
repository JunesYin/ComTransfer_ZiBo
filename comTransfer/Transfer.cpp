#include "StdAfx.h"
#include "Transfer.h"

#include "Util.h"
#include "Constants.h"



/*
* val: 0xFF 0x12 0x34 0x56 0x78 0x01 [����/����ֵ] [�������/�������] 0xAA
0xFF 0x12 0x34 0x56 0x78 0x01 0x80 0x80 0xAA
FF 12 34 56 78 01 80 80 AA
0x88 => 1000 1000
0x44 => 0100 0100
0x80 => 1000 0000


*userFlag: int -- �û���־ -- 0�����һλ�û�����λ���ݣ���1����ڶ�λ�û�����λ���ݣ�

��������Ϊ (anwser_ + ���˳�� + ��Ҵ� + #)
(�����һ�����ѡ��A �ͷ���anwser_1A#)
*/
const CString transfer(const CString &val, const int &userID, const CString &userName,
	char options[4], const CString &prefix, const CString &suffix)
{
	if (val.GetLength() < 1)
	{
		return _T("");
	}

	char ch = WcharToChar(val.GetAt(18+userID));

	CString sAnswer;
	if (ch == options[0])
	{
		sAnswer = _T("A");
	}
	else if (ch == options[1])
	{
		sAnswer = _T("B");
	}
	else if (ch == options[2])
	{
		sAnswer = _T("C");
	}
	else if (ch == options[3])
	{
		sAnswer = _T("D");
	}
	
	if (sAnswer.GetLength() < 1)
	{
		return _T("");
	}


	CString rst;
	rst.Format(_T("%s%s%s%s"), prefix, userName, sAnswer, suffix);

	return rst;
}


const BOOL transfer(const CString &val, const int &userID, const CString &userName, 
	char options[4], const CString &prefix, const CString &suffix,
	const char rightAnwser, CString &data, char &anwser)
{
	if (val.GetLength() < 1)
	{
		data = _T("");
		return FALSE;
	}


	char ch = WcharToChar(val.GetAt(18+userID));

	char cAnwser = ANWSER_DEFAULT;
	if (ch == options[0])
	{
		cAnwser = 'A';
	}
	else if (ch == options[1])
	{
		cAnwser = 'B';
	}
	else if (ch == options[2])
	{
		cAnwser = 'C';
	}
	else if (ch == options[3])
	{
		cAnwser = 'D';
	}

	if (ANWSER_DEFAULT == cAnwser)
	{
		data = _T("");
		return FALSE;
	}

	anwser = cAnwser;
	data.Format(_T("%s%s%c%s"), prefix, userName, cAnwser, suffix);

	return (rightAnwser == cAnwser);
}