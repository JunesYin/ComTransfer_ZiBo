#include "StdAfx.h"
#include "JySocket.h"
#include "Constants.h"

#include "comTransfer.h"
#include "comTransferDlg.h"

CJySocket::CJySocket(void)
{
}


CJySocket::~CJySocket(void)
{
}


BOOL CJySocket::isAvailable(void)
{
	return m_isAvailable;
}



void CJySocket::OnReceive(int nErrorCode)
{
	char *data = new char[SIZE_COM_BUFFER];
	memset(data, 0, SIZE_COM_BUFFER);
	Receive(data, SIZE_COM_BUFFER);

	((CcomTransferDlg*)AfxGetApp()->m_pMainWnd)->PostMessage(WM_JYSOCKETDATA,(WPARAM)data,(LPARAM)this);
	CSocket::OnReceive(nErrorCode);
}


void CJySocket::OnConnect(int nErrorCode)
{
	m_isAvailable = TRUE;
}


void CJySocket::OnClose(int nErrorCode)
{
	m_isAvailable = FALSE;
}