#include "StdAfx.h"
#include "User.h"
#include "Util.h"

#include "Constants.h"

#include <cassert>


UINT User::lightPort = 0;
CSerialPort User::serialPort = CSerialPort();
CString User::lightOrderPrefix = LIGHTORDER_PREFIX_DEFAULT;
CString User::lightOrderUserWildcard = LIGHTORDER_USERWILDCARD_DEFAULT;
CString User::lightOrderOff = LIGHTORDER_OFF_DEFAULT;
CString User::lightOrderWhite = LIGHTORDER_WHITE_DEFAULT;
CString User::lightOrderGreen = LIGHTORDER_GREEN_DEFAULT;
CString User::lightOrderRed = LIGHTORDER_RED_DEFAULT;

User::User(void)
{
	m_ID = -1;
	m_name = _T("");
	
	m_port = 0;
	m_anwser = ANWSER_DEFAULT;
	m_lock = FALSE;
}


User::User(int userID, CString userName)
{
	assert(userName.GetLength() > 0);

	m_ID = userID;
	m_name = userName;

	m_port = 0;
	m_anwser = ANWSER_DEFAULT;
	m_lock = FALSE;
}


User::~User(void)
{
}


const int User::getID()
{
	return m_ID;
}


const CString User::getName()
{
	return m_name;
}


const UINT User::getPort()
{
	return m_port;
}

void User::setPort(UINT port)
{
	m_port = port;
}

const BOOL User::isLock()
{
	return m_lock;
}

const char User::getAnwser()
{
	return m_anwser;
}



void User::lock()
{
	m_lock = TRUE;
}

void User::unlock()
{
	m_lock = FALSE;
}

void User::setAnwser(const char anwser)
{
	m_anwser = anwser;
}

BOOL User::judge(const char rightAnwser)
{
	return (rightAnwser == m_anwser);
}






// -----------------------------------------------------------
// Àà·½·¨
// -----------------------------------------------------------
void User::setLightOrder(CString prefix, CString userWildcard, CString off, 
	CString white, CString green, CString red)
{
	setLightOrderPrefix(prefix);
	setLightOrderUserWildcard(userWildcard);
	setLightOrderOff(off);
	setLightOrderWhite(white);
	setLightOrderGreen(green);
	setLightOrderRed(red);
}

void User::setLightOrderPrefix(CString order)
{
	lightOrderPrefix = order;
}

void User::setLightOrderUserWildcard(CString order)
{
	lightOrderUserWildcard = order;
}

void User::setLightOrderOff(CString order)
{
	lightOrderOff = order;
}

void User::setLightOrderWhite(CString order)
{
	lightOrderWhite = order;
}

void User::setLightOrderGreen(CString order)
{
	lightOrderGreen = order;
}

void User::setLightOrderRed(CString order)
{
	lightOrderRed = order;
}


BOOL User::initLightPort(HWND pPortOwner, UINT portnr /* = 1 */, UINT baud /* = 9600 */, 
	char parity /* = 'N' */, UINT databits /* = 8 */, UINT stopsbits /* = ONESTOPBIT */, 
	DWORD dwCommEvents /* = EV_RXCHAR | EV_CTS */, UINT nBufferSize /* = 512 */)
{
	lightPort = portnr;
	return serialPort.InitPort(pPortOwner, lightPort, baud,
		parity, databits, stopsbits,
		dwCommEvents, nBufferSize);
}

BOOL User::startLightPort()
{
	return serialPort.StartMonitoring();
}

void User::stopLightPort()
{
	serialPort.SuspendMonitoring();
	serialPort.ClosePort();
}



const CString User::turnLightOff(User *user /* = NULL */)
{
	CString userName;

	if (NULL == user)
	{
		userName = lightOrderUserWildcard;
	}
	else
	{
		if (user->getName().GetLength() < 2)
		{
			userName.Format(_T("0%s"), user->getName());
		}
		else
		{
			userName = user->getName();
		}
	}

	CString val;
	val.Format(_T("%s %s %s"), lightOrderPrefix, userName, lightOrderOff);

	return write(val);
}

const CString User::turnLightWhite(User *user /* = NULL */)
{
	CString userName;

	if (NULL == user)
	{
		userName = lightOrderUserWildcard;
	}
	else
	{
		if (user->getName().GetLength() < 2)
		{
			userName.Format(_T("0%s"), user->getName());
		}
		else
		{
			userName = user->getName();
		}
	}

	CString val;
	val.Format(_T("%s %s %s"), lightOrderPrefix, userName, lightOrderWhite);

	return write(val);
}

const CString User::turnLightGreen(User *user /* = NULL */)
{
	CString userName;

	if (NULL == user)
	{
		userName = lightOrderUserWildcard;
	}
	else
	{
		if (user->getName().GetLength() < 2)
		{
			userName.Format(_T("0%s"), user->getName());
		}
		else
		{
			userName = user->getName();
		}
	}

	CString val;
	val.Format(_T("%s %s %s"), lightOrderPrefix, userName, lightOrderGreen);

	return write(val);
}

const CString User::turnLightRed(User *user /* = NULL */)
{
	CString userName;

	if (NULL == user)
	{
		userName = lightOrderUserWildcard;
	}
	else
	{
		if (user->getName().GetLength() < 2)
		{
			userName.Format(_T("0%s"), user->getName());
		}
		else
		{
			userName = user->getName();
		}
	}

	CString val;
	val.Format(_T("%s %s %s"), lightOrderPrefix, userName, lightOrderRed);

	return write(val);
}


const CString User::write(CString &val)
{
	if (val.GetLength() < 1)
	{
		return _T("");
	}

	char data[SIZE_COM_BUFFER] = {0};
	int len = str2Hex(val, data);

	serialPort.WriteToPort(data, len);

	return val;
}