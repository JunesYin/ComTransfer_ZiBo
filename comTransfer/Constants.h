

#pragma once



#include "stdafx.h"



// 配置文件路径
#ifdef _DEBUG
static const CString PATH_CONFIG = _T("../../comTransfer/config.ini");
static const char *DATA_PATH = "../../comTransfer/data.xml";
static const char *LOG_PATH = "../../comTransfer/log.txt";
#else
static const CString PATH_CONFIG = _T("./config.ini");
static const char *DATA_PATH = "./data.xml";
static const char *LOG_PATH = "./log.txt";

#endif // _DEBUG

// 缓存区长度
static const UINT SIZE_COM_BUFFER = 1 << 7;
// 用户名最大长度
static const int MAX_NAME_LEGNTH = 1 << 5;
// 用户默认答案
static const char ANWSER_DEFAULT = '#';
// 
static const char *ORDER_TURNLIGHTOFFAUTO_DELAY = "ORDER_TURNLIGHTOFFAUTO_DELAY";
static const char *ORDER_RESUMEGAME_DELAY = "ORDER_RESUMEGAME_DELAY";
static const char *ORDER_WRITELIGHTPORT_DELAY = "ORDER_WRITELIGHTPORT_DELAY";


// 用户名默认配置
static const CString NAME_USER_DEFAULT = _T("1");


// 服务器默认信息
static const CString IP_SERVER_DEFAULT = _T("127.0.0.1");
static const int PORT_SERVER_DEFAULT = 3000;


// 默认串口号
static const CString COM_DEFAULT = _T("1");
// 默认停止位
static const CString STOPBIT_DEFAULT = _T("AA");


// 灯带命令前缀
static const CString LIGHTORDER_PREFIX_DEFAULT = _T("AA");
// 灯带命令用户通配符
static const CString LIGHTORDER_USERWILDCARD_DEFAULT = _T("FF");
// 关灯命令后缀
static const CString LIGHTORDER_OFF_DEFAULT = _T("01 0B 00 00 00 00 00 32 01 00 00 00 00 33 55");
// 白灯命令后缀
static const CString LIGHTORDER_WHITE_DEFAULT = _T("01 0B 00 00 00 00 00 32 01 FF FF FF 00 30 55");
// 绿灯命令后缀
static const CString LIGHTORDER_GREEN_DEFAULT = _T("01 0B 00 00 00 00 00 32 01 00 FF 00 00 32 55");
// 红灯命令后缀
static const CString LIGHTORDER_RED_DEFAULT = _T("01 0B 00 00 00 00 00 32 01 FF 00 00 00 32 55");
// 灯带控制延迟
static const int LIGHTOPER_TIMEINTERVAL_DEFAULT = 5;


// 灯带操作延迟
static const int TIMEINTERVAL_TURNLIGHTOFFAUTO_DEFAULT = 2000;
// 唤醒游戏延迟
static const int TIMEINTERVAL_RESUMEGAME_DEFAULT = 15000;


// 选项默认配置
static const CString A_OPTION_DEFAULT = _T("8");
static const CString B_OPTION_DEFAULT = _T("4");
static const CString C_OPTION_DEFAULT = _T("2");
static const CString D_OPTION_DEFAULT = _T("1");
static const int ANWSER_POSITION_DEFAULT = 6;


// COM口最大用户数
static const int MAXUSER_COM_DEFAULT = 2;
// 默认暂停时间
static const int TIMEINTERVAL_SEND_DEFAULT = 30;
// 默认前缀
static const CString PREFIX_RESULT_DEFAULT = _T("answer_");
static const CString SUFFIX_RESULT_DEFAULT = _T("#");
// 激活游戏命令
static const CString ACTIVATEGAME_ORDER_DEFAULT = _T("setquest_");
// 开始答题命令
static const CString STARTANWER_ORDER_DEFAULT = _T("quest_");
// 结束答题命令
static const CString STOPANWSER_ORDER_DEFAULT = _T("close_");




// 波特率
static const UINT bauds[22] = {
	75,
	150,
	300,
	600,
	1200,
	1800,
	2400,
	3600,
	4800,
	7200,
	9600,
	14400,
	19200,
	28800,
	38400,
	57600,
	115200,
	230400,
	460800,
	614400,
	921600,
	1228800
};

// 数据位
static const UINT dataBits[4] = {
	5, 6, 7, 8
};

// 校验码
static const char parities[] = {
	'n',	// none
	'e',	// even
	'o',	// odd
	'm',	// mark
	's'		// space
};
