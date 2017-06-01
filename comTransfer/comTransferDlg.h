
// comTransferDlg.h : ͷ�ļ�
//

#pragma once

#include "User.h"
#include "Util.h"
#include "JySocket.h"
#include "SerialPort.h"



#include <vector>
#include <map>


using std::vector;
using std::map;


#define WM_NOTIFY_ICON			WM_USER + 16



// CcomTransferDlg �Ի���
class CcomTransferDlg : public CDialogEx
{
// ����
public:
	CcomTransferDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_COMTRANSFER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	// �������
	NOTIFYICONDATA m_notifyIcon;	// ����

	
	// �������
	// [SERVER]
	CString m_ServerIP;		// ������IP
	UINT m_ServerPort;		// �������˿�
	// [COM]
	vector<UINT> m_portConfig;		// ���COM��
	UINT m_stopFlag;		// ֹͣλ
	// [LIGHT]
	int m_isLightOn;
	UINT m_lightPort;		// �ƴ�COM��
	CString m_lightOrderPrefix;		// �ƴ�����ǰ׺
	CString m_lightOrderUserWildcard;	// �ƴ��û�ͨ���
	CString m_lightOrderOff;	// �ƴ��ص�����
	CString m_lightOrderWhite;	// �ƴ��׵�����
	CString m_lightOrderGreen;	// �ƴ��̵�����
	CString m_lightOrderRed;	// �ƴ��������
	int m_timeInterval_writeLightPort;	// �ƴ������ӳ�
	// [DELAY]
	int m_timeInterval_turnLightOffAuto;	// ��������Զ��ص��ӳ�
	int m_timeInterval_resumeGame;	// ������Ϸʱ��
	// [ANWSER]
	char m_options[5];	//��һ��Ԫ�ش���A���ڶ���Ԫ�ش���B���Դ�����
	int m_anwserPosition;	// ��λ��
	// [SET]
	int m_maxUserCom;	// ÿ��������༸���û�	
	int m_timeInterval_sendTCP;	// ת�����
	CString m_prefix;	// ת��ǰ׺
	CString m_suffix;	// ת����׺
	CString m_activateOrder;	// ������Ϸָ��
	CString m_startAnwserOrder;	// ��ʼ����ָ��
	CString m_stopAnwserOrder;	// ��������ָ��
	// [APPINFO]
	CString m_appVersion;	// ����汾
	


	// �������
	CJySocket m_socket;		// fell sad today
	BOOL m_isOpen;	// �Ƿ��Ѵ򿪲���⴮��
	vector<User> m_users;	// �û���Ϣ
	vector<UINT> m_ports;	// ��ǰ�豸�ϵ����д���
	vector<CSerialPort> m_serialPorts;	// ���ڲ�����


	UINT m_curPort;	// ��ǰѡ��Ĵ���
	UINT m_curBaud;	// ��ǰ������
	UINT m_curDataBit;	// ��ǰ����λ
	char m_curParity;	// ��ǰУ��λ
	UINT m_curStopBit;	// ��ǰֹͣλ


	CodeType m_curSendCodeType;	// ��ǰ�����ʽ
	CodeType m_curRcvCodeType;	// ��ǰ�����ʽ


	map<UINT, CString> m_rcvStrs;	// ����COM�����ݱ���
	vector<char> m_anwsers;	// ��ǰ��Ϸ��
	BOOL m_isBusyOperLight;	// �Ƿ����ڵȴ������ƴ�
	BOOL m_isBusyResumeGame;	// �Ƿ����ڵȴ�������Ϸ
	BOOL m_isPlaying;	// �Ƿ�������Ϸ��
	int m_curQstIndex;	// ��ǰ��Ŀ����
	int m_curUserIndex;	// ��ǰ�ƴ�COM���û�����



	void initMore();
	BOOL loadConfig();

	void showTray();
	void setRcvCodeType(CodeType codeType);
	void setSendCodeType(CodeType codeType);
	void updateShowData(const CString &val);
	BOOL operWork();


	BOOL startUserSerialPort();
	void stopUserSerialPort();

	
	void write();
	void sendToServer(const CString &val, UINT port);


	void activateGame(CString &val);
	void startAnwser(CString &val);
	void stopAnwser(CString &val);
	void resunmeGame();
	void judgeToWriteLightPort();

	void lockUser(User *user = NULL);
	void unlockUser(User *user = NULL);

	void turnLightOff(User *user = NULL);
	void turnLightWhite(User *user = NULL);
	void turnLightGreen(User *user = NULL);
	void turnLightRed(User *user = NULL);

// ʵ��
protected:
	HICON m_hIcon;

	BOOL DestroyWindow();

	 /*----------------------------------------
         * OnComm�����������ú����ڴ��ڡ��ַ����ա�
         * ��Ϣ�������Ӧ������ʾ���⡣
         *----------------------------------------*/
    afx_msg LRESULT OnReceiveChar(WPARAM ch, LPARAM port);
	// ����TCP����
	afx_msg LRESULT OnReceiveTCPString(WPARAM wParam, LPARAM lParam);
	// ��ʱ��
	afx_msg LRESULT OnTimerFired(WPARAM wParam, LPARAM lParam);
	/*
	�����̷���
	*/
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelChange_combo_baudRate();
	afx_msg void OnCbnSelChange_combo_dataBit();
	afx_msg void OnCbnSelChange_combo_parity();
	afx_msg void OnCbnSelChange_combo_stopBit();
	afx_msg void OnBnClicked_button_operPort();

	afx_msg void OnBnClicked_button_clear_r();
	afx_msg void OnBnClicked_radio_hex_rcv();
	afx_msg void OnBnClicked_radio_asc_rcv();
	afx_msg void OnBnClicked_radio_hex_rcv_radio_asc_rcv();
	afx_msg void OnBnClicked_radio_asc_send();
	afx_msg void OnBnClicked_radio_hex_send();
	afx_msg void OnBnClicked_radio_hex_send_button_send();
	afx_msg void OnBnClicked_button_send();
	afx_msg void OnBnClicked_button_clear_send();
	afx_msg void OnBnClickedbuttonsavelog();
};
