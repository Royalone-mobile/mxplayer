// NetDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "Common/CommTypes.h"
#include "NetServer/NetServer.h"
#include "common/NetCoreDef.h"

CNetServer m_NetServer;

typedef struct _tTestStruct
{
	USHORT	usSize;
	char	szText[1024 * 7];
}T_TestStruct, *LP_TestStruct;

static void e_PrintLog(const char* pszfmt, ...)
{
	/// ��ʱ�ַ���
	char szTmp[2048] = {0};
	va_list argptr;
	va_start(argptr, pszfmt);
	memset(szTmp, 0x00, sizeof(szTmp));
	vsnprintf_s(szTmp, sizeof(szTmp) - 1, pszfmt, argptr);
	va_end(argptr);
	printf("%s\r\n", szTmp);
}


/// �����������
static void e_OnRecvDataCallBack(USHORT usNetType, ULONG ulContextID, const char* pszClientIP, 
	USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount, 
	SHORT sSNum, SHORT sENum, int nDatalen, void* pData)
{
	START_DEBUG_INFO;
	/// �յ�TCP����
	if(NTT_TCPData == usNetType)
	{

	}
	/// �յ�UDP����
	else if(NTT_UDPData == usNetType)
	{
		e_PrintLog("�յ�UDP���ݣ�ClientIP = %s, ClientPort = %d\r\n",
			pszClientIP, usClientPort);

		/// m_CNetCore.e_IUDPSendData(pszClientIP, usClientPort, (PBYTE)pData, nDatalen);
	}
	END_DEBUG_INFO;
}

/// �������������
static void e_OnSendDataCallBack(USHORT usNetType, ULONG ulContextID, const char* pszClientIP,
	USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount,
	SHORT sSNum, SHORT sENum, int nDatalen, void* pData)
{
	START_DEBUG_INFO
	/// ����TCP����
	/*if(NDT_TCPData == usNetDataType)
	{
		e_PrintLog("����TCP���ݣ�ClientIP = %s, ClientPort = %d, nDatalen = %d\r\n. \r\n\r\n",
			pszClientIP, usClientPort, nDatalen);
	}
	/// ����UDP����
	else if(NDT_UDPData == usNetDataType)
	{
		e_PrintLog("����UDP���ݣ�ClientIP = %s, ClientPort = %d, nDatalen = %d\r\n. \r\n\r\n",
			pszClientIP, usClientPort, nDatalen);
	}*/
	END_DEBUG_INFO
}

/// �������ӳɹ�֪ͨ
static void e_OnConectionCallBack(ULONG ulContextID, const char* pszClientIP, USHORT usClientPort)
{
	START_DEBUG_INFO
	/// �ͻ������ӳɹ�
	e_PrintLog("�ͻ������ӳɹ���ClientIP = %s, ClientPort = %d\r\n", pszClientIP, usClientPort);

	char szTemp[512];
	memset(szTemp, 0x00, sizeof(szTemp));

	strncpy_s(szTemp, "Hello this is lily", sizeof(szTemp) - 1); 

	m_NetServer.e_ITCPSendStringData(ulContextID, szTemp, strlen(szTemp));
	/// �ͷ�����
	/// m_CNetCore.e_ICloseTCPContext(ulContextID);
	END_DEBUG_INFO
}

/// ����Ͽ�����֪ͨ
static void e_OnDisConectionCallBack(ULONG ulContextID, const char* pszClientIP, USHORT usClientPort)
{
	START_DEBUG_INFO
	/// �ͻ��˶Ͽ�����
	e_PrintLog("�ͻ��˶Ͽ����ӣ�ClientIP = %s, ClientPort = %d\r\n", pszClientIP, usClientPort);
	END_DEBUG_INFO
}

int main(int argc, char* argv[])
{
	/// ��ʼ�� Windows ����ģ��
	WSADATA wsData;
	if(0 != WSAStartup(MAKEWORD(2, 2), &wsData))
	{
		printf("��ʼ��sockte�����.", GetLastError());
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
	/// ��������
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
			/// ���Դ�������
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

	/// ��ֹWinsock 2 DLL (Ws2_32.dll) ��ʹ��
	WSACleanup();
	return 0;
}