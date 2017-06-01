#pragma once

#include <afxsock.h>

class CJySocket : public CSocket
{
public:
	CJySocket(void);
	~CJySocket(void);


	BOOL isAvailable();

private:
	BOOL m_isAvailable;


protected:
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

