/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// ������������
/// @par ����ļ�
/// ���ø��ļ��������ļ�
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         PublicDef.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2012/12/24
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __VAPP_PUBLIC_DEF_H_H__
#define __VAPP_PUBLIC_DEF_H_H__
#include "CommTypes.h"
//windowsƽ̨sdk��һ��ͷ�ļ����ṩһЩ��ȡ������Ϣ�ĺ������磺������tcp/ip��
#include <Iphlpapi.h>
#pragma comment(lib,"iphlpapi.lib")

#pragma warning(disable:4996)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

namespace PUBLICDEF
{
	/// ���������Ϣ
	enum _eWirelessProErrorCode
	{
		WPEC_E200	= 200,						///< 200: �ɹ���
		WPEC_E206	= 206,						///< 206�����Ӵ���
		WPEC_E300	= 300,						///< 300: δ֪�Ĵ���
		WPEC_E301	= 301,						///< 301: ��֤�豸����״̬δͨ��
		WPEC_E302	= 302,						///< 302��Play������֤�������ȷ
		WPEC_E303	= 303,						///< 303���û�δ��¼VAPP
		WPEC_E304	= 304,						///< 304��������ͬ���û���VAPP���ѵ�¼���������ڵ�¼
		WPEC_E305	= 305,						///< 305����������ȷ
		WPEC_E306	= 306,						///< 306������������ļ�·����������128
		WPEC_E401	= 401,						///< 401: ���ϲ����ڣ���ȡ�����ѱ�ȷ��
	};

	/// ��ȡ״̬����
	inline string e_GetStatusDescription(USHORT usStatus)
	{
		switch(usStatus)
		{
		case WPEC_E200:
			{
				return "OK";
			}
		case WPEC_E305:
			{
				return "Param Error.";
			}
		case WPEC_E401:
			{
				return "This Fault Not Exit Or Has Confirmed.";
			}
		default:
			{
				return "Unknown error";
			}
		}
	}

	/// ����������Ϣ
	typedef struct _tServiceConfInfo
	{
		USHORT				usLocalTCPPort;		///< ����TCP �˿�
		USHORT				usLocalUDPPort;		///< ����UDP �˿�
		USHORT				usRoutPort;			///< ·�� �˿�
		char				szInterAddr[20];	///< ���� ��ַ
		char				szGroupAddr[20];	///< �鲥 ��ַ
		char				szRouterIP[20];		///< ·�� �����ַ
		char				szGetWayIP[20];		///< ָ��������IP
		USHORT				usGetWayPort;		///< ָ�������ض˿�
		USHORT				usTransModel;		///< ��������ת��ģʽģʽ
		USHORT				usNetWorkType;		///< �������������

		void e_Init()
		{
			usLocalTCPPort = 0;
			usLocalTCPPort = 0;
			usRoutPort = 0;
			memset(szInterAddr, 0x00, sizeof(szInterAddr));
			memset(szGroupAddr, 0x00, sizeof(szGroupAddr));
			memset(szRouterIP, 0x00, sizeof(szRouterIP));
			memset(szGetWayIP, 0x00, sizeof(szGetWayIP));
			usGetWayPort = 0;
			usTransModel = 1;
			usNetWorkType = 1;
		};

		/// ��ʼ������
		_tServiceConfInfo()
		{
			e_Init();
		}
	}T_ServiceConfInfo, *LP_ServiceConfInfo;

	/// 
	/// @par ���� 
	/// time_tת��ΪData
	/// @param [in,out]	tTime	time_tʱ��
	///	[in]		tTime			
	/// @return 	DATE			DATEʱ��
	/// @note -
	/// @par ʾ����
	/// @code
	/// @deprecated -
	/// 
	static DATE e_TimeToDate(time_t tTime)
	{
		DATE dDate = (tTime + 28800) / (24.0 * 60.0 * 60.0) + 25569;
		return dDate;
	};

	/// 
	/// @par ���� 
	/// <Dataת��Ϊtime_t>
	/// @param 
	///	[in,out]	<������>		<����˵��>
	///	[in]		DATE			DATEʱ��
	/// @return 	time_t			time_tʱ��
	/// @note -
	/// @par ʾ����
	/// @code
	/// @deprecated -
	/// 
	static time_t e_DateToTime(DATE dDate)
	{
		time_t tTime = (time_t)((dDate - 25569) * (24 * 60 * 60) - 28800);
		return tTime;
	};

	/// 
	/// @par ���� 
	/// ͨ�������ջ�ȡ�����������еڼ���
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		nYear			��
	/// [in]		nMonth			��
	/// [in]		nDay			��
	/// @return 	int				�������ڼ�
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	static int e_GetWeekDay(int nYear, int nMonth, int nDay)
	{
		int ny = nYear;
		int nm = nMonth;
		int nd = nDay;
		int nc = 0;
		int nw = 0;
		int weekday = 0;
		char ch = 0;

		if(1 == nm || 2 == nm)
		{
			ny = ny - 1;
			nc = (int)( ny / 100 );
			if( nm == 1 )
			{ nm = 13; }
			else
			{ nm = 14; }
			ny = ny - nc * 100;
		}
		else
		{
			nc = (int) ( ny / 100 );
			ny = ny - nc * 100;
		}

		nw = (int)(nc / 4) - 2 * nc + ny + (int)(ny / 4) + (int)(26 * (nm + 1) / 10) + nd - 1;
		weekday = nw % 7;
		return weekday;
	}

	static string e_GetServiceID(const char* pszServiceIP, USHORT usServicePort)
	{
		/// ��֤���ݺϷ���
		if(NULL == pszServiceIP || 0 >= usServicePort)
		{
			return "";
		}

		char szTemp[64];
		memset(szTemp, 0x00, sizeof(szTemp));
		snprintf(szTemp, sizeof(szTemp) - 1, "%s:%d", pszServiceIP, usServicePort);
		return szTemp;
	}

	/// 
	/// @par ���� 
	/// ͨ�������ջ�ȡ�����Ǹ����еڼ���
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		nYear			��
	/// [in]		nMonth			��
	/// [in]		nDay			��
	/// @return 	int				һ���еĵڼ���
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	static int e_GetYearDay(int nYear, int nMonth, int nDay)
	{
		int nSum = 0;
		int nLeap = 0;

		/// �ȼ���ĳ����ǰ�·ݵ�������
		switch(nMonth)
		{
		case 1: nSum = 0; break;
		case 2: nSum = 31; break;
		case 3: nSum = 59; break;
		case 4: nSum = 90; break;
		case 5: nSum = 120; break;
		case 6: nSum = 151; break;
		case 7: nSum = 181; break;
		case 8: nSum = 212; break;
		case 9: nSum = 243; break;
		case 10: nSum = 273; break;
		case 11: nSum = 304; break;
		case 12: nSum = 334; break;
		}
		/// �ټ���ĳ�������
		nSum = nSum + nDay;
		/// �ж��ǲ�������
		if(nYear % 400 == 0 
			|| ( nYear % 4 == 0 && nYear % 100 != 0 ))
		{
			nLeap = 1;
		}
		else
		{
			nLeap = 0;
		}
		/// ������������·ݴ���2,������Ӧ�ü�һ��
		if( 1 == nLeap && nMonth > 2)
		{
			nSum++;
		}

		return nSum;
	}

	/// 
	/// @par ���� 
	/// ����ʽΪ[�������� ���� ���� ʱʱ �ַ� ����]���ַ���ת��Ϊ���ڸ�ʽ, 
	/// �пո���Ի���������ַ�
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		pTime			����ʱ���ַ���
	/// @return 	time_t			����time_tʱ��
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	static time_t e_StringToTime(const char* pTime)
	{
		tm InTimetm;
		time_t InTime = 0;
		char szInYear[10];
		char szInMon[10];
		char szInDay[10];
		char szInHour[10];
		char szInMunite[10];
		char szInSec[10];

		if(NULL == pTime)
		{
			return InTime;
		}

		memset(szInYear, 0x0, sizeof(szInYear));
		memset(szInMon, 0x0, sizeof(szInMon));
		memset(szInDay, 0x0, sizeof(szInDay));
		memset(szInHour, 0x0, sizeof(szInHour));
		memset(szInMunite, 0x0, sizeof(szInMunite));
		memset(szInSec, 0x0, sizeof(szInSec));
		string strAll = pTime;
		if (0 == strAll.length())
		{
			return InTime;
		}
		string strTmp = "";
		strTmp = strAll.substr(0, 4);
		snprintf(szInYear, sizeof(szInYear), "%s", strTmp.c_str());

		strTmp = strAll.substr(5, 2);
		snprintf(szInMon, sizeof(szInMon), "%s", strTmp.c_str());

		strTmp = strAll.substr(8, 2);
		snprintf(szInDay, sizeof(szInDay), "%s", strTmp.c_str());

		strTmp = strAll.substr(11, 2);
		snprintf(szInHour, sizeof(szInHour), "%s", strTmp.c_str());

		strTmp = strAll.substr(14, 2);
		snprintf(szInMunite, sizeof(szInMunite), "%s", strTmp.c_str());

		strTmp = strAll.substr(17, 2);
		snprintf(szInSec, sizeof(szInSec), "%s", strTmp.c_str());

		memset(&InTimetm, 0x0, sizeof(InTimetm));
		InTimetm.tm_year = atoi(szInYear) - 1900;
		InTimetm.tm_mon = atoi(szInMon) - 1;
		InTimetm.tm_mday = atoi(szInDay);
		InTimetm.tm_hour = atoi(szInHour);
		InTimetm.tm_min = atoi(szInMunite);
		InTimetm.tm_sec = atoi(szInSec);
		/// ����
		InTimetm.tm_wday = e_GetWeekDay(InTimetm.tm_year+1900,InTimetm.tm_mon+1,InTimetm.tm_mday);
		/// ����
		InTimetm.tm_yday = e_GetYearDay(InTimetm.tm_year+1900,InTimetm.tm_mon+1,InTimetm.tm_mday);
		InTimetm.tm_isdst = 0;
		InTime = mktime(&InTimetm);
		return InTime;
	}

	/// 
	/// @par ���� 
	/// ��ȡtm ʱ��ṹ��
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note -
	/// @par ʾ����
	/// @code
	/// @deprecated -
	/// 
	static void e_GetTmStruct(tm* m_tm, time_t& t_time)
	{
#ifdef WIN32
		localtime_s(m_tm, &t_time);
#else
		tm* ttm = NULL;

		ttm = localtime(&t_time);

		if(NULL != ttm)
		{
			memcpy(m_tm, ttm, sizeof(tm));
		}
#endif
	};
	/// 
	/// @par ���� 
	/// ʱ������ת��Ϊstring���ĸ�����
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		t_time			time_tʱ��
	/// [in,out]	pTime			ת������ַ���
	/// [in]		nszSize			ת������ַ�����С
	/// [in]		flag			Ҫת���ɵ��ַ�������
	/// @return 		-
	/// @note -
	/// @par ʾ����
	/// @code
	/// @deprecated - 
	/// 
	static void e_TimeToString(time_t t_time, char* pTime, int nszSize, int flag = 4)
	{
		int nYear = 0, mon = 0, nDay = 0, hour = 0, min = 0, sec = 0;
		struct tm m_tm;
		e_GetTmStruct(&m_tm, t_time);

		nYear = (int)m_tm.tm_year + 1900;
		mon = (int)m_tm.tm_mon + 1;
		nDay = (int)m_tm.tm_mday;
		hour = (int)m_tm.tm_hour;
		min = (int)m_tm.tm_min;
		sec = (int)m_tm.tm_sec;
		switch(flag)
		{
		case 1:
			{ 
				snprintf(pTime, nszSize, "%04d%02d%02d%02d%02d%02d", nYear, mon, nDay, hour, min, sec); 
				break; 
			}
		case 2:
			{ 
				snprintf(pTime, nszSize, "%04d%02d%02d", nYear, mon, nDay); 
				break; 
			}
		case 3:
			{ 
				snprintf(pTime, nszSize, "%02d%02d%02d", hour, min, sec); 
				break; 
			}
		case 4:
			{ 
				snprintf(pTime, nszSize, "%04d-%02d-%02d %02d:%02d:%02d", nYear, mon, nDay, hour, min, sec); 
				break; 
			}
		case 5:
			{ 
				snprintf(pTime, nszSize, "%04d-%02d-%02d", nYear, mon, nDay); 
				break; 
			}
		case 6:
			{ 
				snprintf(pTime, nszSize, "%nd:%nd:%nd", hour, min, sec); 
				break; 
			}
		case 7:
			{
				hour = (int)t_time / (60 * 60);
				t_time = (int)t_time % ( 60 * 60 );
				min = (int)t_time / 60;
				t_time = (int)t_time % 60;
				sec = (int)t_time;
				snprintf(pTime, nszSize, "%nd:%nd:%nd", hour, min, sec);
				break;
			}
		case 8:
			{
				/// ����Ϊ��
				hour = (int)t_time / (60 * 60);
				snprintf(pTime, nszSize, "%nd", hour*( 60* 60 )+(min*60)+sec);
				break;
			}
		case 9:
			{
				nYear = nYear % 100;
				/// ������ں�ʱ��
				snprintf(pTime, nszSize, "%02d%02d%02d000", nYear, mon, nDay);
				break;
			}
		default:
			{
				snprintf(pTime, nszSize, "%04d%02d%02d%02d%02d%02d", nYear, mon, nDay, hour, min, sec);
				break;
			}
		}
	};

	static void e_GetSysTime(string& strCurrentTime)
	{
		char szTime[64];
		memset(szTime, 0x00, sizeof(szTime));
#ifdef WIN32
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		snprintf(szTime, sizeof(szTime) - 1, "%d-%02d-%02d %02d:%02d:%02d.%03d", 
			SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,
			SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);
#else
		struct timeval tv;
		gettimeofday(&tv, NULL);
		long ltime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif

		strCurrentTime = szTime;
	}

	/// 
	/// @par ���� 
	/// ����ϵͳʱ��(�������紫�䴦�����ʱ�䶼������1����)
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		tmStruct		Ҫ���õ�tm�ṹ��	
	/// @return 	bool			����ϵͳʱ��ɹ�����ʧ��
	/// @note -
	/// @par ʾ����
	/// @code
	/// @deprecated - 
	/// 
	static bool e_SetSystemTime(tm& tmStruct)
	{
		time_t tTime = mktime(&tmStruct);

#ifdef WIN32

		SYSTEMTIME SystemTime;

		SystemTime.wYear = tmStruct.tm_year + 1900;
		SystemTime.wMonth = tmStruct.tm_mon + 1;
		SystemTime.wDay = tmStruct.tm_mday;
		SystemTime.wHour = tmStruct.tm_hour;
		SystemTime.wMinute = tmStruct.tm_min;
		SystemTime.wSecond = tmStruct.tm_sec + 1;
		SystemTime.wDayOfWeek = tmStruct.tm_wday;
		SystemTime.wMilliseconds = 0;

		BOOL bRet = SetLocalTime(&SystemTime);
		return (bRet == TRUE);

#else

		struct timeval tv;
		tv.tv_sec = tTime + 1;
		tv.tv_usec = 0;
		settimeofday(&tv, 0);
		return true;

#endif
	};

	/// �����ڴ滻�㵥λ
	#define MEMDIV 1024

	/// 
	/// @par ���� 
	/// ��ȡ�����ڴ�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// @return 	Int				�����ڴ��С����λM
	/// @note 		-
	/// @par ʾ����
	/// @code 		-
	/// @endcode 
	/// @see		-
	/// @deprecated	- 
	/// 
	inline int e_GetSystemMemoryStat()
	{
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof (statex);
		//cout<<"statex.dwLingth :"<<statex.dwLength<<endl;
		GlobalMemoryStatusEx (&statex);

		int nMemoryAvail = (int)(statex.ullAvailPhys / (MEMDIV * MEMDIV));

		//cout<<"�����ڴ����������"<< nMemoryAvail <<"M"<<endl;
		//cout<<"�����ڴ���������"<<statex.ullTotalPhys/MEMDIV/MEMDIV<<"M"<<endl;
		//cout<<"�����ڴ���������"<<statex.ullTotalVirtual/MEMDIV/MEMDIV<<"M"<<endl;
		//cout<<"�����ڴ����������"<<statex.ullAvailVirtual/MEMDIV/MEMDIV<<"M"<<endl;
		return nMemoryAvail;
	}

	/// 
	/// @par ���� 
	/// �Ա�ʱ��Ƭ
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		time1			��һ��ϵͳʱ��Ƭ
	///	[in]		time2			�ڶ���ϵͳʱ��Ƭ
	/// @return 	-
	/// @note 		-
	/// @par ʾ����
	/// @code 		-
	/// @endcode 
	/// @see		-
	/// @deprecated	- 
	/// 
	inline INT64 e_CompareFileTime(FILETIME time1, FILETIME time2)
	{
		/// ���Ծ����ֵǿ��ת��Ϊ�޷���
		INT64 a = (UINT64)time1.dwHighDateTime << 32 | time1.dwLowDateTime;
		INT64 b = (UINT64)time2.dwHighDateTime << 32 | time2.dwLowDateTime;
		return (b - a);
	};

	/// 
	/// @par ���� 
	/// ��ȡCPUʹ����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// @return 	int				ʹ���ʰٷֱ�
	/// @note 		-
	/// @par ʾ����
	/// @code 		-
	/// @endcode 
	/// @see		-
	/// @deprecated	- 
	/// 
	inline int e_GetSystemCupStat()
	{ 
		/// �����¼��ź�
		HANDLE hEvent;
		BOOL res;

		FILETIME preidleTime;
		FILETIME prekernelTime;
		FILETIME preuserTime;

		FILETIME idleTime;
		FILETIME kernelTime;
		FILETIME userTime;

		/// ��ȡϵͳʱ��Ƭ
		res = GetSystemTimes(&preidleTime, &prekernelTime, &preuserTime);

		/// ��ʼֵΪ nonsignaled ������ÿ�δ������Զ�����Ϊnonsignaled
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		/// ���û�л�ȡ���źţ��ȴ�1500���뷵��
		WaitForSingleObject(hEvent, 1500);

		/// �ٴλ�ȡϵͳʱ��Ƭ
		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

		INT64 idle	 = e_CompareFileTime(preidleTime, idleTime);
		INT64 kernel = e_CompareFileTime(prekernelTime, kernelTime);
		INT64 user	 = e_CompareFileTime(preuserTime, userTime);

		/// CPU������:
		int nUseCpu = (int)((kernel + user - idle) * 100 / (kernel + user));
		///// CPU������:
		//int nCpuIdle = (int)((idle) * 100 / (kernel + user));
		///// ��ӡ
		//printf("CPU������: %d%%, ������ %d%%\r\n", nUseCpu, nCpuIdle);
		return nUseCpu;
	};

	/// 
	/// @par ���� 
	/// ��ȡTCP�˿�״̬
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		nPort			�˿ں�
	/// @return 	-
	/// @note 		-
	/// @par ʾ����
	/// @code 		-
	/// @endcode 
	/// @see		-
	/// @deprecated	- 
	/// 
	inline bool e_GetPortTCPStatus(USHORT nPort, ULONG& ulSize)
	{
		Sleep(10);
		bool bRet = false;
		PMIB_TCPTABLE pTcpTable = NULL;
		MIB_TCPTABLE TcpTable;
		ulSize = sizeof(MIB_TCPTABLE);
		int Res = GetTcpTable(&TcpTable, &ulSize, FALSE);
		if(ERROR_INSUFFICIENT_BUFFER == Res)
		{
			pTcpTable = (MIB_TCPTABLE* )malloc(ulSize + 1);
			if(NULL != pTcpTable)
			{
				if(NO_ERROR == GetTcpTable(pTcpTable, &ulSize, FALSE))
				{
					DWORD nCount = pTcpTable[0].dwNumEntries;
					for(UINT i = 0; i < nCount; i++)
					{
						MIB_TCPROW TcpRow = pTcpTable[0].table[i];
						DWORD temp1 = TcpRow.dwLocalPort;
						int temp2 = temp1 / 256 + (temp1 % 256) * 256;
						if(temp2 == nPort)
						{
							free(pTcpTable);
							pTcpTable = NULL;
							return bRet;
						}
					}
				}
				else
				{
					free(pTcpTable);
					pTcpTable = NULL;
					printf("e_GetPortTCPStatus 1 Error = %d, Res = %d\r\n", GetLastError(), Res);
					return bRet;
				}
				free(pTcpTable);
				pTcpTable = NULL;
				return true;
			}
			return bRet;
		}
		else
		{
			printf("e_GetPortTCPStatus 2 Error = %d, Res = %d\r\n", GetLastError(), Res);
			return false;
		}		
	};

	/// 
	/// @par ���� 
	/// ��ȡUDP�˿�״̬
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		nPort			�˿ں�
	/// @return 	-
	/// @note 		-
	/// @par ʾ����
	/// @code 		-
	/// @endcode 
	/// @see		-
	/// @deprecated	- 
	/// 
	inline bool e_GetPortUDPStatus(USHORT nPort, ULONG& ulSize)
	{
		Sleep(10);
		bool bRet = false;
		PMIB_UDPTABLE pUdpTable = NULL;
		MIB_UDPTABLE UdpTable;
		ulSize = sizeof(MIB_UDPTABLE);
		int Res =  GetUdpTable(&UdpTable, &ulSize, FALSE);
		if(ERROR_INSUFFICIENT_BUFFER == Res)
		{
			pUdpTable = (MIB_UDPTABLE* )malloc(ulSize + 1);
			if(NULL != pUdpTable)
			{
				if(NO_ERROR == GetUdpTable(pUdpTable, &ulSize, FALSE))
				{
					DWORD nCount = pUdpTable[0].dwNumEntries;
					for(UINT i = 0; i < nCount; i++) 
					{
						MIB_UDPROW UDPRow = pUdpTable[0].table[i];
						DWORD temp1 = UDPRow.dwLocalPort;
						int temp2 = temp1 / 256 + (temp1 % 256) * 256;
						if(temp2 == nPort)
						{
							free(pUdpTable);
							pUdpTable = NULL;
							return bRet;
						}
					}
				}
				else
				{
					free(pUdpTable);
					pUdpTable = NULL;
					printf("e_GetPortUDPStatus 1 Error = %d\r\n", GetLastError());
					return bRet;
				}
				free(pUdpTable);
				pUdpTable = NULL;
				return true;
			}
			return bRet;
		}
		else
		{
			printf("e_GetPortUDPStatus 2 Error = %d\r\n", GetLastError());
			return false;
		}
	};

	/// 
	/// @par ���� 
	/// �滻�ַ����е��ַ�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		-
	/// @par ʾ����
	/// @code 		-
	/// @endcode 
	/// @see		-
	/// @deprecated	- 
	/// 
	static string e_ReplaceAll(string str, string pattern, string dstPattern)
	{
		std::string retStr="";
		string::size_type pos;

		int szStr = str.length();
		int szPattern = pattern.size();
		int i=0;
		int l_count=0;
		// replace all
		int count = szStr;

		for(i=0; i<szStr; i++)
		{
			pos=str.find(pattern,i);

			if(pos == std::string::npos)
			{
				break;
			}
			if(szStr > (int)pos)
			{
				std::string s=str.substr(i,pos-i);
				retStr += s;
				retStr += dstPattern;
				i=pos+pattern.length()-1;
				if(++l_count >= count)
				{
					i++;
					break;
				}
			}
		}
		retStr += str.substr(i);
		return retStr;
	}

	/// ==============================================
	/// @par ���� 
	/// �����ַ����еڼ��γ���Ŀ���ַ�����λ��
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		by li.xl 2013/04/27 
	/// ==============================================
	inline int e_Instr(char* pszSrc, char* pszDes, USHORT usIndex)
	{
		int nFindeIndex = -1;
		/// ��֤����
		if(NULL == pszSrc || NULL == pszDes)
		{
			return nFindeIndex;
		}

		bool bIsFind = false;
		/// ��ֵ����Դ
		char* pszCharTemp = pszSrc;
		while(NULL != (pszCharTemp = strstr(pszCharTemp, pszDes)))
		{
			nFindeIndex++;
			if(nFindeIndex == usIndex)
			{
				bIsFind = true;
				break;
			}
		}
		/// ����Ҷ�Ӧλ�õ��ַ����ִ�
		if(true == bIsFind)
		{
			nFindeIndex = pszCharTemp - pszSrc;
		}
		return nFindeIndex;
	}

	/// ==============================================
	/// @par ���� 
	/// ���õ�ǰ����Ŀ¼
	/// @return 	-
	/// @note 		by li.xl 2013/04/12 
	/// ==============================================
	inline void e_SetCurDirectory()
	{
		/// ���õ�ǰ����Ŀ¼
		char szBuf[512];
		char szDirectPath[512];
		memset(szDirectPath, 0x00, sizeof(szDirectPath));
		/// �õ���ǰģ��·��
		::GetModuleFileName(NULL, szDirectPath, sizeof(szDirectPath) - 1);
		char* pszLastPoint = strrchr(szDirectPath, '\\');
		if(NULL != pszLastPoint)
		{
			UINT nSize = pszLastPoint - szDirectPath;
			if(0 < nSize)
			{
				strncpy_s(szBuf, szDirectPath, nSize);
				szBuf[nSize] = 0;
			}
		}
		::SetCurrentDirectory(szBuf);
	}

	/// ==============================================
	/// @par ���� 
	/// �����༶Ŀ¼
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// @return 	-
	/// @note 		by li.xl 2013/04/19 
	/// ==============================================
	inline int CreatDir(char* pszDir)
	{
		int i = 0;
		int iRet;
		int iLen = strlen(pszDir);

		//��ĩβ��/
		if (pszDir[iLen - 1] != '\\' && pszDir[iLen - 1] != '/')
		{
			pszDir[iLen] = '/';
			pszDir[iLen + 1] = '\0';
		}

		// ����Ŀ¼
		for (i = 0;i <= iLen;i ++)
		{
			if (pszDir[i] == '\\' || pszDir[i] == '/')
			{ 
				pszDir[i] = '\0';

				//���������,����
				iRet = _access(pszDir,0);
				if (iRet != 0)
				{
					iRet = _mkdir(pszDir);
					if (iRet != 0)
					{
						return -1;
					} 
				}
				//֧��linux,������\����/
				pszDir[i] = '/';
			} 
		}
		return 0;
	}


	/// 
	/// @par ���� 
	/// ��ȡ��ǰ�ļ�����Ŀ¼
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// @return 	string			�����ļ�·��
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	static string e_GetFilePath()
	{
		/// �����ȡ�ַ��ַ���
		char szExePath[512];
		memset(szExePath, 0x00, sizeof(szExePath));
		/// ��ȡ����ĵ�ַ����
		int nPathLen = sizeof(szExePath);
		/// �����ȡ���صı���
		DWORD ldwReturn = 0;
		/// �����ַ�ָ��
		char* pPos = NULL;
		/// ����·�����Ŀ¼�ַ�
		char cInchar = '\\';

		/// ��ȡ·��
#ifndef WIN32
		ldwReturn = readlink( "/proc/self/exe", szExePath, nPathLen);
		cInchar = '/';
#else
		ldwReturn = GetModuleFileNameA(NULL, szExePath, nPathLen);
		cInchar = '\\';
#endif

		if(0 < (int)ldwReturn && nPathLen > (int)ldwReturn)
		{
			pPos = strrchr(szExePath, cInchar);
			if(NULL != pPos)
			{
				*++pPos = '\0';
			}
		}
		/// �����ػ�ȡ����·���ַ���
		string strExePath = szExePath;
		return strExePath;
	}
}
#endif	/// __VAPP_PUBLIC_DEF_H_H__