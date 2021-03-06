
// comTransferDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "comTransfer.h"
#include "comTransferDlg.h"
#include "afxdialogex.h"


#include "User.h"
#include "Util.h"
#include "Transfer.h"
#include "JySocket.h"
#include "Constants.h"

#include "tinyxml/tinyxml.h"

#include <cassert>
#include <fstream>


using std::ofstream;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcomTransferDlg 对话框




CcomTransferDlg::CcomTransferDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcomTransferDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(ID_ICON);
}

void CcomTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CcomTransferDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	 /*----------------------------------------------
         * WM_COMM_RXCHAR消息<---->OnComm函数   映射声明
         *----------------------------------------------*/
    ON_MESSAGE(WM_COMM_RXCHAR, &CcomTransferDlg::OnReceiveChar)
	// 接受TCP数据
	ON_MESSAGE(WM_JYSOCKETDATA, &CcomTransferDlg::OnReceiveTCPString)
	ON_MESSAGE(WM_TIMER, &CcomTransferDlg::OnTimerFired)
	// 托盘
	ON_MESSAGE(WM_NOTIFY_ICON, &CcomTransferDlg::OnNotifyIcon)
	// 串口参数
	ON_CBN_SELCHANGE(ID_comBo_baudRate, &CcomTransferDlg::OnCbnSelChange_combo_baudRate)
	ON_CBN_SELCHANGE(ID_comBo_dataBit, &CcomTransferDlg::OnCbnSelChange_combo_dataBit)
	ON_CBN_SELCHANGE(ID_comBo_checkBit, &CcomTransferDlg::OnCbnSelChange_combo_parity)
	ON_CBN_SELCHANGE(ID_comBo_stopBit, &CcomTransferDlg::OnCbnSelChange_combo_stopBit)
	ON_BN_CLICKED(ID_button_operPort, &CcomTransferDlg::OnBnClicked_button_operPort)
	// 收码
	ON_BN_CLICKED(ID_button_clear_rcv, &CcomTransferDlg::OnBnClicked_button_clear_r)
	ON_BN_CLICKED(ID_radio_hex_rcv, &CcomTransferDlg::OnBnClicked_radio_hex_rcv)
	ON_BN_CLICKED(ID_radio_asc_rcv, &CcomTransferDlg::OnBnClicked_radio_asc_rcv)
	// 发码
	ON_BN_CLICKED(ID_radio_asc_send, &CcomTransferDlg::OnBnClicked_radio_asc_send)
	ON_BN_CLICKED(ID_radio_hex_send, &CcomTransferDlg::OnBnClicked_radio_hex_send)
	ON_BN_CLICKED(ID_button_send, &CcomTransferDlg::OnBnClicked_button_send)
	ON_BN_CLICKED(ID_button_clear_send, &CcomTransferDlg::OnBnClicked_button_clear_send)
	ON_BN_CLICKED(ID_button_saveLog, &CcomTransferDlg::OnBnClickedbuttonsavelog)
END_MESSAGE_MAP()


// CcomTransferDlg 消息处理程序

BOOL CcomTransferDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MINIMIZE);
	ShowWindow(SW_RESTORE);	// 前台激活

	// TODO: 在此添加额外的初始化代码
	initMore();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcomTransferDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (SC_CLOSE == nID)
	{
		int value = AfxMessageBox(_T("是否想要【退出】程序？"), MB_YESNO | MB_ICONQUESTION);
		if (IDYES == value)
		{
			CDialogEx::OnSysCommand(nID, lParam);
		}
	}
	else if (SC_MINIMIZE == nID)
	{
		showTray();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcomTransferDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//// ------ 设置背景颜色 --- start -----
		//CRect rect;
		//CPaintDC dc(this);
		//GetClientRect(rect);
		//dc.FillSolidRect(rect, RGB(167, 214, 192));
		//dc.FillPath();
		//// ------ 设置背景颜色 --- end -----
		
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcomTransferDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CcomTransferDlg::DestroyWindow()
{
	// 停止串口监听
	stopUserSerialPort();
	User::stopLightPort();

	// 停止TCP连接
	m_socket.ShutDown();
	m_socket.Close();

	// 删除托盘标志
	Shell_NotifyIcon(NIM_DELETE, &m_notifyIcon);
	
	return CDialog::DestroyWindow();
}



LRESULT CcomTransferDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	if (ID_ICON != wParam)
	{
		return 1;
	}

	switch(lParam)
	{
	case WM_RBUTTONUP:	// 右键抬起时弹出菜单
		{
			LPPOINT point = new tagPOINT;
			::GetCursorPos(point);
			CMenu menu;
			menu.CreatePopupMenu();
			menu.AppendMenu(MF_STRING, WM_DESTROY, _T("退出"));
			menu.TrackPopupMenu(TPM_LEFTALIGN, point->x, point->y, this);
			HMENU hMenu = menu.Detach();
			menu.DestroyMenu();
			delete point;


			break;
		}
	case WM_LBUTTONDBLCLK:	// 左键双击处理
		{
			ModifyStyleEx(0,WS_EX_TOPMOST);  
			ShowWindow(SW_SHOWDEFAULT);  
		}
	}

	return 0;
}



/*
	* OnOnReceiveChar函数实现
	* 用于接收单个字符
	* 触发条件: 当输入缓冲区有字符时，有消息出发
	* 执行结果：字符接受，并显示在ID为ID_editCtrl的文本编辑框上
*/
LRESULT CcomTransferDlg::OnReceiveChar(WPARAM ch, LPARAM port)
{
	if (m_lightPort == port)
	{
		return 0;
	}

	if (m_isBusyOperLight || m_isBusyResumeGame)
	{
		return 0;
	}

	CString &rcvStr = m_rcvStrs[port];
	if (m_stopFlag != ch)
	{
		rcvStr.AppendFormat(_T("%02x "), ch);
	}
	else
	{
		rcvStr.AppendFormat(_T("%02x"), ch);
		rcvStr.MakeUpper();

		CString msg;
		msg.Format(_T("RECEIVE----%s----FROM COM%d"), rcvStr, port);

		if ('0' == rcvStr.GetAt(18) && '0' == rcvStr.GetAt(19))
		{
			msg.Append(_T(" => 忽略无效数据"));
			updateShowData(msg);	
			rcvStr = _T("");

			return 0;
		}

		updateShowData(msg);

		sendToServer(rcvStr, port);

		rcvStr = _T("");
	}

	return 0;
}


LRESULT CcomTransferDlg::OnReceiveTCPString(WPARAM wParam, LPARAM lParam)
{
	if (m_isBusyOperLight || m_isBusyResumeGame)
	{
		return -1;
	}

	char *data = (char *)wParam;
	CString val = CString(data);

	CString msg;
	msg.Format(_T("RECEIVE----%s----FROM SERVER"), val);
	updateShowData(msg);

	if (-1 < val.Find(m_activateOrder))
	{
		activateGame(val);
	}
	else if (-1 < val.Find(m_startAnwserOrder))
	{
		startAnwser(val);
	}
	else if (-1 < val.Find(m_stopAnwserOrder))
	{
		stopAnwser(val);
	}

	return 0;
}



/*
 * 计时
*/
LRESULT CcomTransferDlg::OnTimerFired(WPARAM wParam, LPARAM lParam)
{

	const char *data = (const char *)wParam;
	BOOL needDeleteData = FALSE;
	BOOL needKillTimer = TRUE;

	if (0 == strcmp(data, ORDER_TURNLIGHTOFFAUTO_DELAY))
	{
		m_isBusyOperLight = FALSE;
		turnLightOff();

		if (m_curQstIndex >= (int)m_anwsers.size() - 1)
		{	
			// 为避免接受数据显示文本框内容过于庞大，导致所占内存过多
			// 每轮游戏结束后（最后一题灯带自动关闭之后）自动保存当轮游戏log，并清除接受数据文本框内容

			updateShowData(_T("游戏结束"));

			// 保存log
			OnBnClickedbuttonsavelog();
			// 清除接受数据显示文本框内容
			OnBnClicked_button_clear_r();
		}
	}
	else if (0 == strcmp(data, ORDER_RESUMEGAME_DELAY))
	{
		resunmeGame();
	}
	else if (0 == strcmp(data, ORDER_WRITELIGHTPORT_DELAY))
	{
		KillTimer(wParam);
		needKillTimer = FALSE;

		judgeToWriteLightPort();
	}
	else
	{
		m_socket.Send(data, strlen(data));
		needDeleteData = TRUE;
	}

	if (needKillTimer)
	{
		KillTimer(wParam);
	}

	if (needDeleteData)
	{
		delete data;
	}

	return 0;
}


/*
初始化
*/
void CcomTransferDlg::initMore()
{
	// 当前未打开任何串口
	m_isOpen = FALSE;
	m_isBusyOperLight = FALSE;
	m_isBusyResumeGame = FALSE;
	m_isPlaying = FALSE;
	m_curQstIndex = -1;

	// 获取该设备串口
	EnumerateSerialPorts(m_ports);
	assert(m_ports.size() > 0);

	BOOL flagLoadConfig = loadConfig();
	assert(flagLoadConfig);
	

	// 窗口标题
	CString sTitle;
	sTitle.Format( _T("串口翻译器（%s）"), m_appVersion);
	SetWindowText(sTitle);


	CString sComInfo = _T("当前："); 
	for (int i = 0; i < (int)m_portConfig.size(); ++i)
	{
		sComInfo.AppendFormat(_T(" COM%d"), m_portConfig[i]);
	}
	sComInfo.AppendFormat(_T("\r\n灯带： COM%d"), m_lightPort);
	((CStatic *)GetDlgItem(ID_label_comInfo))->SetWindowText(sComInfo);


	// **********************************
	// 初始化串口参数 start
	// **********************************

	
	// 初始化 波特率下拉框
	CComboBox *comboBox = (CComboBox *)GetDlgItem(ID_comBo_baudRate);
	for (int i = 0; i < sizeof(bauds) / sizeof(bauds[0]); ++i)
	{
		CString baud;
		baud.Format(_T("%ld"), bauds[i]);
		comboBox->AddString(baud);
	}
	comboBox->SetCurSel(10);
	m_curBaud = bauds[10];


	// 初始化 数据位下拉框
	comboBox = (CComboBox *)GetDlgItem(ID_comBo_dataBit);
	for (int i = 0; i < sizeof(dataBits) / sizeof(dataBits[0]); ++i)
	{
		CString dataBit;
		dataBit.Format(_T("%d"), dataBits[i]);
		comboBox->AddString(dataBit);
	}
	comboBox->SetCurSel(3);
	m_curDataBit = dataBits[3];
	

	// 初始化 校验码下拉框
	comboBox = (CComboBox *)GetDlgItem(ID_comBo_checkBit);
	comboBox->SetCurSel(0);
	m_curParity = parities[0];


	// 初始化 停止位下拉框
	comboBox = (CComboBox *)GetDlgItem(ID_comBo_stopBit);
	comboBox->SetCurSel(0);
	m_curStopBit = ONESTOPBIT;

	// **********************************
	// 初始化串口参数 end
	// **********************************


	// **********************************
	// 初始化发码 start
	// **********************************

	// 选择默认发码格式 - 16进制
	((CButton *)GetDlgItem(ID_radio_hex_send))->SetCheck(TRUE);
	m_curSendCodeType = CodeType_hex;

	// 选择是否定时发送 - 否
	((CButton *)GetDlgItem(ID_check_timing))->SetCheck(FALSE);
	((CEdit *)GetDlgItem(ID_editCtrl_time))->SetWindowText(_T("50"));

	((CButton *)GetDlgItem(ID_button_send))->EnableWindow(FALSE);




	// **********************************
	// 初始化发码 end
	// **********************************



	// **********************************
	// 初始化收码 start
	// **********************************

	((CButton *)GetDlgItem(ID_radio_hex_rcv))->SetCheck(TRUE);
	setRcvCodeType(CodeType_hex);




	// **********************************
	// 初始化收码 start
	// **********************************


	BOOL flag = operWork();
#ifndef DEBUG
	if (flag)
	{
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
#endif
}


/*
 * 配置读取
*/
BOOL CcomTransferDlg::loadConfig()
{
	// -------------------------------------------------
	// [SERVER]
	// -------------------------------------------------
	// 服务器IP配置
	::GetPrivateProfileString(_T("SERVER"), _T("IP"), IP_SERVER_DEFAULT, m_ServerIP.GetBuffer(20), 20, PATH_CONFIG);

	// 服务器端口配置
	m_ServerPort = ::GetPrivateProfileInt(_T("SERVER"), _T("PORT"), PORT_SERVER_DEFAULT, PATH_CONFIG);


	// -------------------------------------------------
	// [COM]
	// -------------------------------------------------
	// 串口配置
	LPWSTR buffer = new WCHAR[20];
	int len = ::GetPrivateProfileString(_T("COM"), _T("COM"), COM_DEFAULT, buffer, 20, PATH_CONFIG);
	CString strComSet = buffer;
	vector<CString> vecComSet;
	separateString(strComSet, ',', vecComSet);
	for (vector<CString>::iterator iter = vecComSet.begin(); iter < vecComSet.end(); ++iter)
	{
		UINT port = _wtoi(*iter);
		BOOL isExist = FALSE;
		for (int i = 0; i < (int)m_ports.size(); ++i)
		{
			if (port == m_ports[i])
			{
				isExist = TRUE;
				break;
			}
		}

		if (!isExist)
		{
			CString msg;
			msg.Format(_T("COM%d不存在"), port);
			MessageBox(msg, _T("错误"));
			return FALSE;
		}

		m_portConfig.push_back(port);
		m_serialPorts.push_back(CSerialPort());

		m_rcvStrs[port] = CString(_T(""));
	}
	delete [] buffer;

	// 停止位配置
	buffer = new WCHAR[20];
	len =  ::GetPrivateProfileString(_T("COM"), _T("STOP"), STOPBIT_DEFAULT, buffer, 20, PATH_CONFIG);
	CString stopFlag = buffer;
	m_stopFlag = hexStrToInt(stopFlag);
	delete [] buffer;


	// -------------------------------------------------
	// [LIGHT]
	// -------------------------------------------------
	// 灯带
	m_isLightOn = ::GetPrivateProfileInt(_T("LIGHT"), _T("ON"), 0, PATH_CONFIG);
	if (m_isLightOn)
	{
		// 灯带串口
		m_lightPort = ::GetPrivateProfileInt(_T("LIGHT"), _T("COM"), 1, PATH_CONFIG);
		BOOL isExist = FALSE;
		for (int i = 0; i < (int)m_ports.size(); ++i)
		{
			if (m_lightPort == m_ports[i])
			{
				isExist = TRUE;
				break;
			}
		}
		if (!isExist)
		{
			CString msg;
			msg.Format(_T("COM%d不存在"), m_lightPort);
			MessageBox(msg, _T("错误"));
			return FALSE;
		}

		// 灯带命令前缀
		buffer = new WCHAR[1 << 7];
		len = ::GetPrivateProfileString(_T("LIGHT"), _T("PREFIX"), LIGHTORDER_PREFIX_DEFAULT, buffer, 1 << 7, PATH_CONFIG);
		m_lightOrderPrefix = buffer;
		delete [] buffer;

		// 灯带命令通配符
		buffer = new WCHAR[1 << 7];
		len = :: GetPrivateProfileString(_T("LIGHT"), _T("USERWILDCARD"), LIGHTORDER_USERWILDCARD_DEFAULT, buffer, 1 << 7, PATH_CONFIG);
		m_lightOrderUserWildcard = buffer;
		delete [] buffer;

		// 关灯命令
		buffer = new WCHAR[1 << 7];
		len = ::GetPrivateProfileString(_T("LIGHT"), _T("OFF"), LIGHTORDER_OFF_DEFAULT, buffer, 1 << 7, PATH_CONFIG);
		m_lightOrderOff = buffer;
		delete [] buffer;

		// 白灯命令
		buffer = new WCHAR[1 << 7];
		len = ::GetPrivateProfileString(_T("LIGHT"), _T("WHITE"), LIGHTORDER_WHITE_DEFAULT, buffer, 1 << 7, PATH_CONFIG);
		m_lightOrderWhite = buffer;
		delete [] buffer;

		// 绿灯
		buffer = new WCHAR[1 << 7];
		len = ::GetPrivateProfileString(_T("LIGHT"), _T("GREEN"), LIGHTORDER_GREEN_DEFAULT, buffer, 1 << 7, PATH_CONFIG);
		m_lightOrderGreen = buffer;
		delete [] buffer;

		// 红灯命令
		buffer = new WCHAR[1 << 7];
		len = ::GetPrivateProfileString(_T("LIGHT"), _T("RED"), LIGHTORDER_RED_DEFAULT, buffer, 1 << 7, PATH_CONFIG);
		m_lightOrderRed = buffer;
		delete [] buffer;

		m_timeInterval_writeLightPort = ::GetPrivateProfileInt(_T("LIGHT"), _T("TIMEINTERVAL"), LIGHTOPER_TIMEINTERVAL_DEFAULT, PATH_CONFIG);


		User::setLightOrder(m_lightOrderPrefix, m_lightOrderUserWildcard, m_lightOrderOff,
			m_lightOrderWhite, m_lightOrderGreen, m_lightOrderRed);
	}



	// -------------------------------------------------
	// [DELAY]
	// -------------------------------------------------
	// 灯带延迟关闭时间
	m_timeInterval_turnLightOffAuto = ::GetPrivateProfileInt(_T("DELAY"), _T("TIMEOUTLIGHT"), TIMEINTERVAL_TURNLIGHTOFFAUTO_DEFAULT, PATH_CONFIG);

	// 唤醒游戏延迟
	m_timeInterval_resumeGame = ::GetPrivateProfileInt(_T("DELAY"), _T("TIMEOUTRESTART"), TIMEINTERVAL_RESUMEGAME_DEFAULT, PATH_CONFIG);


	// -------------------------------------------------
	// [ANWSER]
	// -------------------------------------------------
	// 答案选项配置
	buffer = new WCHAR[10];
	len = ::GetPrivateProfileString(_T("ANWSER"), _T("A"), A_OPTION_DEFAULT, buffer, 10, PATH_CONFIG);
	m_options[0] = WcharToChar(buffer[0]);
	delete [] buffer;

	buffer = new WCHAR[10];
	len = ::GetPrivateProfileString(_T("ANWSER"), _T("B"), B_OPTION_DEFAULT, buffer, 10, PATH_CONFIG);
	m_options[1] = WcharToChar(buffer[0]);
	delete [] buffer;

	buffer = new WCHAR[10];
	len = ::GetPrivateProfileString(_T("ANWSER"), _T("C"), C_OPTION_DEFAULT, buffer, 10, PATH_CONFIG);
	m_options[2] = WcharToChar(buffer[0]);
	delete [] buffer;

	buffer = new WCHAR[10];
	len = ::GetPrivateProfileString(_T("ANWSER"), _T("D"), D_OPTION_DEFAULT, buffer, 10, PATH_CONFIG);
	m_options[3] = WcharToChar(buffer[0]);
	delete [] buffer;

	m_options[4] = '\0';

	m_anwserPosition = ::GetPrivateProfileInt(_T("ANWSER"), _T("POSITIONANWSER"), ANWSER_POSITION_DEFAULT, PATH_CONFIG);


	// -------------------------------------------------
	// [SET]
	// -------------------------------------------------
	// 串口最大用户数
	m_maxUserCom = ::GetPrivateProfileInt(_T("SET"), _T("MAXUSERCOM"), MAXUSER_COM_DEFAULT, PATH_CONFIG);
	// 发送延迟
	m_timeInterval_sendTCP = ::GetPrivateProfileInt(_T("SET"), _T("TIMEOUTSEND"), TIMEINTERVAL_SEND_DEFAULT, PATH_CONFIG);
	// 前缀配置
	buffer = new WCHAR [50];
	len = ::GetPrivateProfileString(_T("SET"), _T("PREFIX"), PREFIX_RESULT_DEFAULT, buffer, 50, PATH_CONFIG);
	m_prefix = buffer;
	delete [] buffer;

	// 后缀配置
	buffer = new WCHAR[10];
	len = ::GetPrivateProfileString(_T("SET"), _T("SUFFIX"), SUFFIX_RESULT_DEFAULT, buffer, 10, PATH_CONFIG);
	m_suffix = buffer;
	delete [] buffer;

	// 激活游戏命令
	buffer = new WCHAR[20];
	len = ::GetPrivateProfileString(_T("SET"), _T("ACTIVATEORDER"), ACTIVATEGAME_ORDER_DEFAULT, buffer, 20, PATH_CONFIG);
	m_activateOrder = buffer;
	delete [] buffer;

	// 开始答题命令
	buffer = new WCHAR[20];
	len = ::GetPrivateProfileString(_T("SET"), _T("STARTORDER"), STARTANWER_ORDER_DEFAULT, buffer, 20, PATH_CONFIG);
	m_startAnwserOrder = buffer;
	delete [] buffer;

	// 结束答题命令
	buffer = new WCHAR[20];
	len = :: GetPrivateProfileString(_T("SET"), _T("STOPORDER"), STOPANWSER_ORDER_DEFAULT, buffer, 20, PATH_CONFIG);
	m_stopAnwserOrder = buffer;
	delete [] buffer;


	// -------------------------------------------------
	// [APPINFO]
	// -------------------------------------------------
	buffer = new WCHAR[20];
	len = ::GetPrivateProfileString(_T("APPINFO"), _T("VERSION"), _T(""), buffer, 20, PATH_CONFIG);
	m_appVersion = buffer;
	delete [] buffer;


	// 用户配置
	buffer = new WCHAR[20];
	len = ::GetPrivateProfileString(_T("USER"), _T("NAME"), COM_DEFAULT, buffer, 20, PATH_CONFIG);
	CString strUser = buffer;
	vector<CString> vecUserSet;
	separateString(strUser, ';', vecUserSet);
	for (int i = 0; i < (int)vecUserSet.size(); ++i)
	{
		CString strComUser = vecUserSet[i];
		vector<CString> vecComUserSet;
		separateString(strComUser, ',', vecComUserSet);
		for (int j = 0; j < (int)vecComUserSet.size(); ++j)
		{
			User user = User(m_portConfig[i] * m_maxUserCom + j, vecComUserSet[j]);
			user.setPort(m_portConfig[i]);
			m_users.push_back(user);
		}
	}

	return true;
}


/*
 * 显示托盘
*/
void CcomTransferDlg::showTray()
{
	ShowWindow(SW_HIDE);

	CString sTitle;
	GetWindowText(sTitle);

	m_notifyIcon.cbSize = sizeof(NOTIFYICONDATA);	// 分配空间
	m_notifyIcon.hWnd = m_hWnd;
	m_notifyIcon.uID = ID_ICON;
	m_notifyIcon.uCallbackMessage = WM_NOTIFY_ICON;	// 激活函数赋值
	m_notifyIcon.hIcon = AfxGetApp()->LoadIcon(ID_ICON);
	m_notifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;

	lstrcpy(m_notifyIcon.szTip, sTitle);
	lstrcpy(m_notifyIcon.szInfoTitle, sTitle);
	lstrcpy(m_notifyIcon.szInfo, _T("已最小化到托盘，双击显示/右键退出"));

	Shell_NotifyIcon(NIM_ADD, &m_notifyIcon);
}


/*
	* 收码格式设置
*/
void CcomTransferDlg::setRcvCodeType(CodeType codeType)
{
	if (codeType == m_curRcvCodeType)
	{
		return;
	}

	m_curRcvCodeType = codeType;

	CString text;
	CEdit *editCtrl = (CEdit *)GetDlgItem(ID_editCtrol_showData);
	editCtrl->GetWindowText(text);

	CString str;

	if (CodeType_hex == m_curRcvCodeType)
	{
		str = convertAscStrToHexStr(text);
	}
	else
	{
		str = convertHexStrToAscStr(text);
	}

	editCtrl->SetWindowText(str);
}

/*
	* 发码格式设置
*/
void CcomTransferDlg::setSendCodeType(CodeType codeType)
{
	if (codeType == m_curSendCodeType) 
	{
		return;
	}

	m_curSendCodeType = codeType;

	CString text;
	CEdit *editCtrl = (CEdit *)GetDlgItem(ID_editCtrl_data);
	editCtrl->GetWindowText(text);

	CString str;

	if (CodeType_hex == m_curSendCodeType)
	{
		str = convertAscStrToHexStr(text);
	}
	else
	{
		str = convertHexStrToAscStr(text);
	}

	editCtrl->SetWindowText(str);
}



/*
* 更新显示框
*/
void CcomTransferDlg::updateShowData(const CString &val)
{
	CString tmpVal;
	if (CodeType_asc == m_curRcvCodeType)
	{
		tmpVal = convertHexStrToAscStr(val);
	}
	else
	{
		tmpVal = val;
	}


	CString text;
	CEdit *editCtrl_showData = (CEdit *)GetDlgItem(ID_editCtrol_showData);
	editCtrl_showData->GetWindowText(text);

	CTime time = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(),
		time.GetHour(), time.GetMinute(), time.GetSecond());

	text.AppendFormat(_T("%s：%s\r\n"), strTime, tmpVal);

	editCtrl_showData->SetWindowText(text);
	// 滚动屏
	editCtrl_showData->LineScroll(editCtrl_showData->GetLineCount(), 0);

	((CButton *)GetDlgItem(ID_button_saveLog))->EnableWindow(TRUE);
}


BOOL CcomTransferDlg::operWork()
{
	CButton *button = (CButton *)GetDlgItem(ID_button_operPort);
	CStatic *lbMsg = (CStatic *)GetDlgItem(ID_label_msg);
	BOOL flagEnable = TRUE;

	if (!m_isOpen)
	{
		if (!startUserSerialPort())
		{
			return FALSE;
		}

		if (! User::initLightPort(m_hWnd, m_lightPort, m_curBaud,
			m_curParity, m_curDataBit, m_curStopBit,
			EV_RXCHAR | EV_CTS, SIZE_COM_BUFFER))
		{
			CString msg;
			msg.Format(_T("COM%d被占用"), m_lightPort);
			MessageBox(msg, _T("错误"));
			return FALSE;
		}

		if (!User::startLightPort())
		{
			CString msg;
			msg.Format(_T("监听COM%d失败"), m_lightPort);
			MessageBox(msg, _T("错误"));
			return FALSE;
		}


		if (!m_socket.Create())
		{
			CString msg;
			msg.Format(_T("创建Socket失败，错误码: %d"), m_socket.GetLastError());
			MessageBox(msg, _T("错误"));

			stopUserSerialPort();
			User::stopLightPort();

			return FALSE;
		}

		if (!m_socket.Connect(m_ServerIP, m_ServerPort))
		{
			MessageBox(_T("连接主机失败！！！"), _T("错误"));

			stopUserSerialPort();
			User::stopLightPort();

			m_socket.Close();

			return FALSE;
		}

		m_isOpen = TRUE;
		button->SetWindowText(_T("关闭串口"));
		flagEnable = FALSE;
		lbMsg->SetWindowText(_T("串口正常"));
	}
	else
	{
		stopUserSerialPort();

		User::stopLightPort();

		m_socket.ShutDown();
		m_socket.Close();

		m_isOpen = FALSE;
		button->SetWindowText(_T("打开串口"));
		flagEnable = TRUE;
		lbMsg->SetWindowText(_T(""));


		for (int i = 0; i < (int)m_portConfig.size(); ++i)
		{
			m_rcvStrs[m_portConfig[i]] = _T("");
		}
	}

	((CComboBox *)GetDlgItem(ID_comBo_baudRate))->EnableWindow(flagEnable);
	((CComboBox *)GetDlgItem(ID_comBo_dataBit))->EnableWindow(flagEnable);
	((CComboBox *)GetDlgItem(ID_comBo_checkBit))->EnableWindow(flagEnable);
	((CComboBox *)GetDlgItem(ID_comBo_stopBit))->EnableWindow(flagEnable);

	((CButton *)GetDlgItem(ID_button_send))->EnableWindow(!flagEnable);

	return TRUE;
}


/*
 * 初始化并监听所有用户
*/
BOOL CcomTransferDlg::startUserSerialPort()
{
	for (int i = 0; i < (int)m_portConfig.size(); ++i)
	{
		UINT port = m_portConfig[i];
		CSerialPort &serialPort = m_serialPorts[i];
		BOOL flagInitPort = serialPort.InitPort(m_hWnd, port, m_curBaud,
			m_curParity, m_curDataBit, m_stopFlag,
			EV_RXCHAR | EV_CTS, SIZE_COM_BUFFER);
		if (!flagInitPort)
		{
			CString msg;
			msg.Format(_T("COM%d被占用"), port);
			MessageBox(msg, _T("错误"));
			return FALSE;
		}


		BOOL flagStart = serialPort.StartMonitoring();
		if (!flagStart)
		{
			CString msg;
			msg.Format(_T("监听COM%d失败"), port);
			MessageBox(msg, _T("错误"));
			return FALSE;
		}
	}

	return TRUE;
}


/*
 * 关闭所有用户
*/
void CcomTransferDlg::stopUserSerialPort()
{
	for (int i = 0; i < (int)m_serialPorts.size(); ++i)
	{
		m_serialPorts[i].ClosePort();
	}
}





/*
* 往串口写数据数据
*/
void CcomTransferDlg::write()
{
	CString text;
	((CEdit *)GetDlgItem(ID_editCtrl_data))->GetWindowText(text);

	CString tmp;
	if (CodeType_asc == m_curSendCodeType)
	{
		tmp = convertAscStrToHexStr(text);
	}
	else
	{
		tmp = text;
	}

	char data[SIZE_COM_BUFFER];
	int len = str2Hex(tmp, data);

	//m_serialPort.WriteToPort(data, len);
}


/*
* 向服务器发数据
*/
void CcomTransferDlg::sendToServer(const CString &val, UINT port)
{
	if (val.GetLength() < 1)
	{
		return;
	}

	for (int i = 0; i < (int)m_users.size(); ++i)
	{
		User &user = m_users[i];

		if (user.isLock())
		{
			continue;
		}

		if (port == user.getPort())
		{
			CString data; 
			CString text;
			if (m_anwsers.size() < 1)
			{	// 激活游戏
				data = transfer(val, user.getID() - user.getPort() * m_maxUserCom, user.getName(),
					m_options, m_prefix, m_suffix);

				if (data.GetLength() < 1)
				{
					continue;
				}

				text.Format(_T("玩家%s激活游戏"), user.getName());
				updateShowData(text);
			}
			else
			{	// 答题
				char anwser;
				BOOL isRight = transfer(val, user.getID() - user.getPort() * m_maxUserCom, user.getName(),
					m_options, m_prefix, m_suffix,
					m_anwsers[m_curQstIndex], data, anwser);

				if (data.GetLength() < 1)
				{
					continue;
				}

				text.Format(_T("玩家%s答题（第%d题）：%c => %s"), user.getName(), m_curQstIndex + 1, 
					anwser, isRight ? _T("正确" : _T("错误")));
				updateShowData(text);

				// 锁定此用户
				lockUser(&user);

				user.setAnwser(anwser);

				isRight ? turnLightGreen(&user) : turnLightRed(&user);
			}

			int len = data.GetLength() + 1;
			char *buffer = WcharToChar(data.GetBuffer(len));

			SetTimer((UINT_PTR)buffer, m_timeInterval_sendTCP * (user.getID() -  user.getPort() * m_maxUserCom), NULL);

			text.Format(_T("SEND----%s----TO SERVER"), data);
			updateShowData(text);
		}
	}
}



void CcomTransferDlg::activateGame(CString &val)
{
	m_isPlaying = TRUE;

	int index_order = val.Find(m_activateOrder);
	int index_suffix = val.Find(m_suffix);
	
	CString strQuestions = val.Mid(m_activateOrder.GetLength(), index_suffix - m_activateOrder.GetLength());
	vector<CString> vecQuestionSet;
	separateString(strQuestions, ',', vecQuestionSet);

	TiXmlDocument dataXml(DATA_PATH);
	if (!dataXml.LoadFile())
	{
		MessageBox(_T("“data.xml不存在”"), _T("错误"));
		return;
	}


	// 
	if (!m_anwsers.empty())
	{
		m_anwsers.clear();
	}


	TiXmlElement *root = dataXml.RootElement();
	for (vector<CString>::iterator iter = vecQuestionSet.begin(); iter < vecQuestionSet.end(); ++iter)
	{
		int index = 0;
		TiXmlNode *question = NULL;
		for (TiXmlNode *item = root->FirstChild("data");
			NULL != item;
			item = item->NextSibling("data"), ++index)
		{
			if (index == _wtoi(*iter))
			{
				question = item;
				break;
			}
		}


		TiXmlNode *child = question->FirstChild();
		int j = 0; 
		while (j++ < m_anwserPosition - 1)
		{
			child = question->IterateChildren(child);
		}

		const char *answer = child->ToElement()->GetText();

		m_anwsers.push_back(answer[0]);
	}


	CString text = _T("游戏开始");
	updateShowData(text);

	text = _T("选定题目：");
	for (int i = 0; i < (int)vecQuestionSet.size(); ++i)
	{
		if (i < (int)vecQuestionSet.size() - 1)
		{
			text.AppendFormat(_T("%d、"), _wtoi(vecQuestionSet[i]) + 1);
		}
		else
		{
			text.AppendFormat(_T("%d"), _wtoi(vecQuestionSet[i]) + 1);
		}
	} 
	text.Append(_T("   答案："));
	for (vector<char>::iterator iter = m_anwsers.begin(); iter < m_anwsers.end(); ++iter)
	{
		text.AppendFormat(_T("%c"), *iter);
	}

	updateShowData(text);

	// 锁定所有用户
	lockUser();

	// 所有用户亮白灯
	turnLightWhite();

}

void CcomTransferDlg::startAnwser(CString &val)
{
	if (!m_isPlaying)
	{
		MessageBox(_T("游戏还未开始，无法开始答题"), _T("错误"));
		return;
	}

	int index_order = val.Find(m_startAnwserOrder);
	int index_suffix = val.Find(m_suffix);

	CString strQuestionIndex = val.Mid(m_startAnwserOrder.GetLength(), index_suffix - m_startAnwserOrder.GetLength());
	m_curQstIndex = _wtoi(strQuestionIndex);

	CString text;
	text.Format(_T("第%d题，答题开始"), m_curQstIndex + 1);
	updateShowData(text);


	// 解锁所有用户
	unlockUser();

	// 所有用户关灯
	turnLightOff();

}

void CcomTransferDlg::stopAnwser(CString &val)
{
	if (!m_isPlaying)
	{
		MessageBox(_T("游戏还未开始，无法结束答题"), _T("错误"));
		return;
	}

	int index_order = val.Find(m_stopAnwserOrder);
	int index_suffix = val.Find(m_suffix);

	CString strQuetionIndex = val.Mid(m_stopAnwserOrder.GetLength(), index_suffix - m_stopAnwserOrder.GetLength());
	int index = _wtoi(strQuetionIndex);

	if (index != m_curQstIndex)
	{
		MessageBox(_T("结束题目序号错误"), _T("错误"));
		return;
	}

	CString text;
	text.Format(_T("第%d题答题结束，判断玩家答案并亮灯"), m_curQstIndex + 1);
	updateShowData(text);

	m_curUserIndex = 0;
	SetTimer((UINT_PTR)ORDER_WRITELIGHTPORT_DELAY, m_timeInterval_writeLightPort, NULL);
	// 此处逻辑改到函数judgeToWriteLightPort()中，延迟处理
	/*
	for (vector<User>::iterator iter = m_users.begin(); iter < m_users.end(); ++iter)
	{
		BOOL isRight = iter->judge(m_anwsers[m_curQstIndex]);
		text.Format(_T("玩家%s：%s"), iter->getName(),isRight ? _T("正确") : _T("错误"));
		updateShowData(text);

		isRight ? turnLightGreen(&(*iter)) : turnLightRed(&(*iter));

		Sleep(m_timeInterval_writeLightPort);

		iter->setAnwser(ANWSER_DEFAULT);	
	}
	*/

	// 锁定所有用户
	lockUser();

	// 一定时间后关灯
	SetTimer((UINT_PTR)ORDER_TURNLIGHTOFFAUTO_DELAY, m_timeInterval_turnLightOffAuto, NULL);

	// 最后一题
	if (m_curQstIndex >= (int)m_anwsers.size() - 1)
	{
		m_isPlaying = FALSE;

		text.Format(_T("锁定游戏，%d秒后解锁"), m_timeInterval_resumeGame / 1000);
		updateShowData(text);
		m_isBusyResumeGame = TRUE;

		SetTimer((UINT_PTR)ORDER_RESUMEGAME_DELAY, m_timeInterval_resumeGame, NULL);
	}
}



void CcomTransferDlg::resunmeGame()
{
	m_curQstIndex = -1;
	m_anwsers.clear();

	m_isBusyResumeGame = FALSE;
	updateShowData( _T("解锁游戏"));

	// 解锁所有用户
	unlockUser();
}
	

void CcomTransferDlg::judgeToWriteLightPort()
{
	User &user = m_users[m_curUserIndex++];
	BOOL isRight = user.judge(m_anwsers[m_curQstIndex]);
	CString text;
	text.Format(_T("玩家%s答案%c（正确答案%c）：%s"), user.getName(), user.getAnwser(),
		m_anwsers[m_curQstIndex], isRight ? _T("正确") : _T("错误"));
	updateShowData(text);

	isRight ? turnLightGreen(&user) : turnLightRed(&user);

	user.setAnwser(ANWSER_DEFAULT);

	if (m_curUserIndex < (int)m_users.size())
	{
		SetTimer((UINT_PTR)ORDER_WRITELIGHTPORT_DELAY, m_timeInterval_writeLightPort, NULL);
	}
	
}



void CcomTransferDlg::lockUser(User *user /* = NULL */)
{
	CString text = _T("锁定用户：");

	if (NULL == user)
	{
		for (vector<User>::iterator iter = m_users.begin(); iter < m_users.end(); ++iter)
		{
			iter->lock();

			text.AppendFormat(_T(" 玩家%s"), iter->getName());
		}
	}
	else
	{
		user->lock();

		text.AppendFormat(_T(" 玩家%s"), user->getName());
	}

	updateShowData(text);
}

void CcomTransferDlg::unlockUser(User *user /* = NULL */)
{
	CString text = _T("解锁用户：");

	if (NULL == user)
	{
		for (vector<User>::iterator iter = m_users.begin(); iter < m_users.end(); ++iter)
		{
			iter->unlock();

			text.AppendFormat(_T(" 玩家%s"), iter->getName());
		}
	}
	else
	{
		user->lock();

		text.AppendFormat(_T(" 玩家%s"), user->getName());
	}

	updateShowData(text);
}


void CcomTransferDlg::turnLightOff(User *user /* = NULL */)
{
	CString strUsers;

	if (NULL == user)
	{
		for (vector<User>::iterator iter = m_users.begin(); iter < m_users.end(); ++iter)
		{
			strUsers.AppendFormat(_T("玩家%s "), iter->getName());
		}
	}
	else
	{
		strUsers.AppendFormat(_T("玩家%s "), user->getName());
	}

	CString content = User::turnLightOff(user);

	CString text;
	text.Format(_T("SEND----%s----TO %s => 关灯"), content, strUsers);
	updateShowData(text);
}

void CcomTransferDlg::turnLightWhite(User *user /* = NULL */)
{
	CString strUsers;

	if (NULL == user)
	{
		for (vector<User>::iterator iter = m_users.begin(); iter < m_users.end(); ++iter)
		{
			strUsers.AppendFormat(_T("玩家%s "), iter->getName());
		}
	}
	else
	{
		strUsers.AppendFormat(_T("玩家%s "), user->getName());
	}

	CString content = User::turnLightWhite(user);

	CString text;
	text.Format(_T("SEND----%s----TO %s => 白灯"), content, strUsers);
	updateShowData(text);
}

void CcomTransferDlg::turnLightGreen(User *user /* = NULL */)
{
	CString strUsers;

	if (NULL == user)
	{
		for (vector<User>::iterator iter = m_users.begin(); iter < m_users.end(); ++iter)
		{
			strUsers.AppendFormat(_T("玩家%s "), iter->getName());
		}
	}
	else
	{
		strUsers.AppendFormat(_T("玩家%s "), user->getName());
	}

	CString content = User::turnLightGreen(user);

	CString text;
	text.Format(_T("SEND----%s----TO %s => 绿灯"), content, strUsers);
	updateShowData(text);
}

void CcomTransferDlg::turnLightRed(User *user /* = NULL */)
{
	CString strUsers;

	if (NULL == user)
	{
		for (vector<User>::iterator iter = m_users.begin(); iter < m_users.end(); ++iter)
		{
			strUsers.AppendFormat(_T("玩家%s "), iter->getName());
		}
	}
	else
	{
		strUsers.AppendFormat(_T("玩家%s "), user->getName());
	}

	CString content = User::turnLightRed(user);

	CString text;
	text.Format(_T("SEND----%s----TO %s => 红灯"), content, strUsers);
	updateShowData(text);
}





/*
下拉框 - 波特率
*/
void CcomTransferDlg::OnCbnSelChange_combo_baudRate()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = ((CComboBox *)GetDlgItem(ID_comBo_baudRate))->GetCurSel();

	m_curBaud = bauds[index];
}


/*
下拉框 - 数据位
*/
void CcomTransferDlg::OnCbnSelChange_combo_dataBit()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = ((CComboBox *)GetDlgItem(ID_comBo_dataBit))->GetCurSel();

	m_curDataBit = dataBits[index];
}


/*
下拉框 - 校验位
*/
void CcomTransferDlg::OnCbnSelChange_combo_parity()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = ((CComboBox *)GetDlgItem(ID_comBo_checkBit))->GetCurSel();

	m_curParity = parities[index];
}


/*
下拉框 - 停止位
*/
void CcomTransferDlg::OnCbnSelChange_combo_stopBit()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = ((CComboBox *)GetDlgItem(ID_comBo_stopBit))->GetCurSel();

	m_curStopBit = (UINT)index;
}


/*
按钮 - 打开串口/关闭串口
*/
void CcomTransferDlg::OnBnClicked_button_operPort()
{
	// TODO: 在此添加控件通知处理程序代码

	operWork();
}





void CcomTransferDlg::OnBnClicked_button_clear_r()
{
	// TODO: 在此添加控件通知处理程序代码

	((CEdit *)GetDlgItem(ID_editCtrol_showData))->SetWindowText(_T(""));

	((CButton *)GetDlgItem(ID_button_saveLog))->EnableWindow(FALSE);
}


void CcomTransferDlg::OnBnClicked_radio_hex_rcv()
{
	// TODO: 在此添加控件通知处理程序代码

	setRcvCodeType(CodeType_hex);
}


void CcomTransferDlg::OnBnClicked_radio_asc_rcv()
{
	// TODO: 在此添加控件通知处理程序代码

	setRcvCodeType(CodeType_asc);
}




void CcomTransferDlg::OnBnClicked_radio_asc_send()
{
	// TODO: 在此添加控件通知处理程序代码

	setSendCodeType(CodeType_asc);
}


void CcomTransferDlg::OnBnClicked_radio_hex_send()
{
	// TODO: 在此添加控件通知处理程序代码

	setSendCodeType(CodeType_hex);
}



void CcomTransferDlg::OnBnClicked_button_send()
{
	// TODO: 在此添加控件通知处理程序代码

	write();
}


void CcomTransferDlg::OnBnClicked_button_clear_send()
{
	// TODO: 在此添加控件通知处理程序代码

	((CEdit *)GetDlgItem(ID_editCtrl_data))->SetWindowText(_T(""));
}


void CcomTransferDlg::OnBnClickedbuttonsavelog()
{
	// TODO: 在此添加控件通知处理程序代码

	CString text;
	((CEdit *)GetDlgItem(ID_editCtrol_showData))->GetWindowText(text);


	char *buffer = WcharToChar(text.GetBuffer(text.GetLength()));


	ofstream fout(LOG_PATH);
	fout << buffer;
	fout.close();

	delete buffer;
}
