// NetDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "Common/CommTypes.h"
#include "Common/AutoLock.h"
#include "NetCore.h"
#include "Common/LogFile.h"
#include "common/xml/Markup.h"


/// ��ȡ��־��ӡ����
CLogFileEx* m_pLogFile = new CLogFileEx("Log/NetDemo", CLogFileEx :: DAY);

CNetCore m_CNetCore;

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

	if(NULL != m_pLogFile)
	{
		m_pLogFile->Log(szTmp);
	}
	else
	{
		printf("%s\r\n", szTmp);
	}
}


/// �����������
static void e_OnRecvDataCallBack(USHORT usNetDataType, ULONG ulContextID,
	const char* pszClientIP, USHORT usClientPort, void* pData, int nDatalen)
{
	START_DEBUG_INFO;
	/// �յ�TCP����
	if(NDT_TCPData == usNetDataType)
	{
		//char szTemp[2048];
		//memset(szTemp, 0x00, sizeof(szTemp));
		//memcpy(szTemp, pData, nDatalen);

		char* pszData = NULL;
		pszData = (char* )pData;
		/// pszData[nDatalen] = '\0';
		pszData += 5;
		CMarkup XmlTemp;
		if(false == XmlTemp.SetDoc(pszData))
		{
			e_PrintLog("XML �����������յ�TCP���ݴ��󡿣�ClientIP = %s, ClientPort = %d nDatalen = %d, �쳣����:\r\n\r\n %s",
				pszClientIP, usClientPort, nDatalen, pszData);
		}
		//else
		//{
		//	//e_PrintLog("�յ�TCP���ݣ�ClientIP = %s, ClientPort = %d\r\n",
		//	//	pszClientIP, usClientPort);

		//	/// e_PrintLog("%s", pszData + 3);
		//}
		/// m_CNetCore.e_ITCPSendData(ulContextID, (PBYTE)pData, nDatalen);
	}
	/// �յ�UDP����
	else if(NDT_UDPData == usNetDataType)
	{
		e_PrintLog("�յ�UDP���ݣ�ClientIP = %s, ClientPort = %d\r\n",
			pszClientIP, usClientPort);

		/// m_CNetCore.e_IUDPSendData(pszClientIP, usClientPort, (PBYTE)pData, nDatalen);
	}
	END_DEBUG_INFO;
}

/// �������������
static void e_OnSendDataCallBack(USHORT usNetDataType, ULONG ulContextID,
	const char* pszClientIP, USHORT usClientPort, void* pData, int nDatalen)
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

	/// ���Է�������
	T_TestStruct sttTest;
	
	strcpy_s(sttTest.szText, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
		"<MANZAIVideo>\r\n"\
		"<Version>1.0</Version>\r\n"\
		"<Sequence>64</Sequence>\r\n"\
		"<CommandType>ASK</CommandType>\r\n"\
		"<Command>LOGIN</Command>\r\n"\
		"<ClientID>ec9e8bf5-b462-4af3-be51-88bbbccd5527</ClientID>\r\n"\
		"</MANZAIVideo>\r\n");
	sttTest.usSize = strlen(sttTest.szText) + 3;

	BYTE BtTemp = 1;

	char szTemp[1024];
	memset(szTemp, 0x00, sizeof(szTemp));
	memcpy(szTemp, &sttTest.usSize, sizeof(USHORT));
	memcpy(szTemp + sizeof(USHORT), &sttTest.usSize, sizeof(USHORT));
	memcpy(szTemp + sizeof(USHORT) * 2, &BtTemp, sizeof(BYTE));
	memcpy(szTemp + sizeof(USHORT) * 2 + 1, sttTest.szText, strlen(sttTest.szText));
	int nSendlen = sttTest.usSize + 2;
	m_CNetCore.e_ITCPSendData(ulContextID, (PBYTE)szTemp, nSendlen);
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
	sttNetInit.usServerNetType = NCNT_Both;
	sttNetInit.usMaxIOWorkers = 6;
	sttNetInit.usPendReadsNum = 6;
	sttNetInit.bOrderSend = true;
	sttNetInit.bOrderRead = true;
	sttNetInit.usFreeConnMaxNum = 512;
	sttNetInit.usFreeBufMaxNum = 2048;
	sttNetInit.usUDPRevcNum = 256;
	sttNetInit.bUDPJoinGroup = true;
	strcpy(sttNetInit.szUDPGroupIP, "239.255.0.1");
	/// ��������
	m_CNetCore.e_IStartServer(&sttNetInit, e_OnRecvDataCallBack, e_OnSendDataCallBack,
		e_OnConectionCallBack, e_OnDisConectionCallBack, e_PrintLog);

	char szChar;
	while(szChar = getchar())
	{
		if('0' == szChar)
		{
			m_CNetCore.e_IStopdServer();
			break;
		}
		else if('1' == szChar)
		{
			/// ���Դ�������
			ULONG ulNums = 0;
			char szData[2048];
			/// while(0 <= ulNums)
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

				m_CNetCore.e_IUDPSendData("239.255.0.1", 6880, (PBYTE)szData, 2048);

				/// Sleep(5);
			}
		}
	}

	if(NULL != m_pLogFile)
	{
		delete m_pLogFile;
		m_pLogFile = NULL;
	}
	/// ��ֹWinsock 2 DLL (Ws2_32.dll) ��ʹ��
	WSACleanup();
	return 0;
}