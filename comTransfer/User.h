#pragma once


#include "SerialPort.h"


class User
{
public:
	User(void);
	User(int useID, CString userName);
	~User(void);

	const int getID();
	const CString getName();
	const UINT getPort();
	const BOOL isLock();
	const char getAnwser();

	void setPort(UINT port);
	void lock();
	void unlock();
	void setAnwser(const char anwser);
	BOOL judge(const char rightAnwser);


	static void setLightOrder(CString prefix, CString userWildcard, CString off, 
		CString white, CString green, CString red);
	static void setLightOrderPrefix(CString order);
	static void setLightOrderUserWildcard(CString order);
	static void setLightOrderOff(CString order);
	static void setLightOrderWhite(CString order);
	static void setLightOrderGreen(CString order);
	static void setLightOrderRed(CString order);

	static BOOL initLightPort(HWND pPortOwner, UINT portnr = 1, UINT baud = 9600, 
		char parity = 'N', UINT databits = 8, UINT stopsbits = ONESTOPBIT, 
		DWORD dwCommEvents = EV_RXCHAR | EV_CTS, UINT nBufferSize = 512);
	static BOOL startLightPort();
	static void stopLightPort();

	static const CString turnLightOff(User *user = NULL);
	static const CString turnLightWhite(User *user = NULL);
	static const CString turnLightGreen(User *user = NULL);
	static const CString turnLightRed(User *user = NULL);

private:
	int m_ID;
	CString m_name;
	UINT m_port;
	char m_anwser;
	BOOL m_lock;


	static UINT lightPort;
	static CSerialPort serialPort;
	static CString lightOrderPrefix;
	static CString lightOrderUserWildcard;
	static CString lightOrderOff;
	static CString lightOrderWhite;
	static CString lightOrderGreen;
	static CString lightOrderRed;

	static const CString write(CString &val);

};

