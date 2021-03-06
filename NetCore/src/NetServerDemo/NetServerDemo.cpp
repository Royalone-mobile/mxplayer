// NetServerDemo.cpp : 定义控制台应用程序的入口点。
//

#include "Common/CommTypes.h"
#include "NetServer/NetServer.h"
#include "common/NetCoreDef.h"
#include "common/NetBusinessDef.h"

CNetServer m_NetServer;

static void e_PrintLog(const char* pszfmt, ...)
{
	/// 临时字符串
	char szTmp[2048] = {0};
	va_list argptr;
	va_start(argptr, pszfmt);
	memset(szTmp, 0x00, sizeof(szTmp));
	vsnprintf_s(szTmp, sizeof(szTmp) - 1, pszfmt, argptr);
	va_end(argptr);
	printf("%s\r\n", szTmp);
}


/// 处理接收数据
static void e_OnRecvDataCallBack(USHORT usNetType, ULONG ulContextID, const char* pszClientIP, 
	USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount, 
	SHORT sSNum, SHORT sENum, int nDatalen, void* pData)
{
	START_DEBUG_INFO;
	/// 收到字符串数据
	if(JDT_StringData == sDataType)
	{
		e_PrintLog("收到 [%s] 数据，ClientIP = %s, ClientPort = %d， ClientString: \r\n%s\r\n", 
			(NTT_TCPData == usNetType) ? "TCP" : "UDP",
			pszClientIP, usClientPort, (char* )pData);

		string strTemp = "This Is FWAY Net Server.";

		m_NetServer.e_ITCPSendStringData(ulContextID, (char* )strTemp.c_str(), strTemp.length());
	}
	else
	{
		e_PrintLog("收到二进制 [%s] 数据，ClientIP = %s, ClientPort = %d，\r\n", 
			(NTT_TCPData == usNetType) ? "TCP" : "UDP",
			pszClientIP, usClientPort);
	}
	END_DEBUG_INFO;
}

/// 处理发送完成数据
static void e_OnSendDataCallBack(USHORT usNetType, ULONG ulContextID, const char* pszClientIP,
	USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount,
	SHORT sSNum, SHORT sENum, int nDatalen, void* pData)
{
	START_DEBUG_INFO
	END_DEBUG_INFO
}

/// 处理连接成功通知
static void e_OnConectionCallBack(ULONG ulContextID, const char* pszClientIP, USHORT usClientPort)
{
	START_DEBUG_INFO
	/// 客户端连接成功
	e_PrintLog("客户端连接成功：ClientIP = %s, ClientPort = %d\r\n", pszClientIP, usClientPort);

	/// 测试发送数据
	char szTemp[512];
	memset(szTemp, 0x00, sizeof(szTemp));
	strncpy_s(szTemp, "Hello this is lily", sizeof(szTemp) - 1); 
	m_NetServer.e_ITCPSendStringData(ulContextID, szTemp, strlen(szTemp));
	END_DEBUG_INFO
}

/// 处理断开连接通知
static void e_OnDisConectionCallBack(ULONG ulContextID, const char* pszClientIP, USHORT usClientPort)
{
	START_DEBUG_INFO
	/// 客户端断开连接
	e_PrintLog("客户端断开连接：ClientIP = %s, ClientPort = %d\r\n", pszClientIP, usClientPort);
	END_DEBUG_INFO
}

int main(int argc, char* argv[])
{
	/// 初始化 Windows 网络模块
	WSADATA wsData;
	if(0 != WSAStartup(MAKEWORD(2, 2), &wsData))
	{
		printf("初始化sockte库错误.", GetLastError());
	}

	T_NetInitStruct sttNetInit;
	sttNetInit.usServerPort = 6880;
	sttNetInit.usServerNetType = NSNT_Both;
	sttNetInit.usMaxIOWorkers = 6;
	sttNetInit.usPendReadsNum = 6;
	sttNetInit.bOrderSend = true;
	sttNetInit.bOrderRead = true;
	sttNetInit.usFreeConnMaxNum = 512;
	sttNetInit.usFreeBufMaxNum = 2048;
	sttNetInit.usUDPRevcNum = 256;
	sttNetInit.bUDPJoinGroup = true;
	strncpy_s(sttNetInit.szUDPGroupIP, "239.255.0.1", sizeof(sttNetInit.szUDPGroupIP) - 1);
	/// 启动服务
	m_NetServer.e_IStartServer(&sttNetInit, e_OnRecvDataCallBack, e_OnSendDataCallBack,
		e_OnConectionCallBack, e_OnDisConectionCallBack, e_PrintLog);

	char szChar;
	while(szChar = getchar())
	{
		if('0' == szChar)
		{
			m_NetServer.e_IStopdServer();
			break;
		}
		else if('1' == szChar)
		{
			/// 测试大量发包
			ULONG ulNums = 0;
			char szData[2048];
			while(0 <= ulNums)
			{
				memset(szData, 0x00, sizeof(szData));
				snprintf(szData, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
					"<MANZAIVideo>\r\n"\
					"<Version>1.0</Version>\r\n"\
					"<Sequence>%lu</Sequence>\r\n"\
					"<CommandType>ASK</CommandType>\r\n"\
					"<Command>LOGIN</Command>\r\n"\
					"<ClientID>ec9e8bf5-b462-4af3-be51-88bbbccd5527</ClientID>\r\n"\
					"</MANZAIVideo>\r\n", ++ulNums);

				m_NetServer.e_IUDPSendStringData("239.255.0.1", 6880, szData, strlen(szData));
				Sleep(5);
			}
		}
	}

	/// 终止Winsock 2 DLL (Ws2_32.dll) 的使用
	WSACleanup();
	return 0;
}