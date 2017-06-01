
// comTransferDlg.h : 头文件
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



// CcomTransferDlg 对话框
class CcomTransferDlg : public CDialogEx
{
// 构造
public:
	CcomTransferDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COMTRANSFER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	// 界面相关
	NOTIFYICONDATA m_notifyIcon;	// 托盘

	
	// 配置相关
	// [SERVER]
	CString m_ServerIP;		// 服务器IP
	UINT m_ServerPort;		// 服务器端口
	// [COM]
	vector<UINT> m_portConfig;		// 玩家COM口
	UINT m_stopFlag;		// 停止位
	// [LIGHT]
	int m_isLightOn;
	UINT m_lightPort;		// 灯带COM口
	CString m_lightOrderPrefix;		// 灯带命令前缀
	CString m_lightOrderUserWildcard;	// 灯带用户通配符
	CString m_lightOrderOff;	// 灯带关灯命令
	CString m_lightOrderWhite;	// 灯带白等命令
	CString m_lightOrderGreen;	// 灯带绿灯命令
	CString m_lightOrderRed;	// 灯带红灯命令
	int m_timeInterval_writeLightPort;	// 灯带操作延迟
	// [DELAY]
	int m_timeInterval_turnLightOffAuto;	// 答题结束自动关灯延迟
	int m_timeInterval_resumeGame;	// 唤醒游戏时间
	// [ANWSER]
	char m_options[5];	//第一个元素代表A，第二个元素代表B，以此类推
	int m_anwserPosition;	// 答案位置
	// [SET]
	int m_maxUserCom;	// 每个串口最多几个用户	
	int m_timeInterval_sendTCP;	// 转发间隔
	CString m_prefix;	// 转发前缀
	CString m_suffix;	// 转发后缀
	CString m_activateOrder;	// 激活游戏指令
	CString m_startAnwserOrder;	// 开始答题指令
	CString m_stopAnwserOrder;	// 结束答题指令
	// [APPINFO]
	CString m_appVersion;	// 软件版本
	


	// 数据相关
	CJySocket m_socket;		// fell sad today
	BOOL m_isOpen;	// 是否已打开并检测串口
	vector<User> m_users;	// 用户信息
	vector<UINT> m_ports;	// 当前设备上的所有串口
	vector<CSerialPort> m_serialPorts;	// 串口操作类


	UINT m_curPort;	// 当前选择的串口
	UINT m_curBaud;	// 当前波特率
	UINT m_curDataBit;	// 当前数据位
	char m_curParity;	// 当前校验位
	UINT m_curStopBit;	// 当前停止位


	CodeType m_curSendCodeType;	// 当前发码格式
	CodeType m_curRcvCodeType;	// 当前收码格式


	map<UINT, CString> m_rcvStrs;	// 接受COM口数据保存
	vector<char> m_anwsers;	// 当前游戏答案
	BOOL m_isBusyOperLight;	// 是否正在等待操作灯带
	BOOL m_isBusyResumeGame;	// 是否正在等待唤醒游戏
	BOOL m_isPlaying;	// 是否正在游戏中
	int m_curQstIndex;	// 当前题目索引
	int m_curUserIndex;	// 当前灯带COM口用户索引



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

// 实现
protected:
	HICON m_hIcon;

	BOOL DestroyWindow();

	 /*----------------------------------------
         * OnComm函数声明，该函数在串口“字符接收”
         * 消息后进行相应处理，显示问题。
         *----------------------------------------*/
    afx_msg LRESULT OnReceiveChar(WPARAM ch, LPARAM port);
	// 接受TCP数据
	afx_msg LRESULT OnReceiveTCPString(WPARAM wParam, LPARAM lParam);
	// 计时器
	afx_msg LRESULT OnTimerFired(WPARAM wParam, LPARAM lParam);
	/*
	从托盘返回
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
