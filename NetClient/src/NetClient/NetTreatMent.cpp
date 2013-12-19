////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �ͻ�������ͨѶҵ����
/// @par ����ļ�
/// NetTreatMent.h
/// @par ������ϸ����
/// <����̨Ӧ�ó������ڵ�>
/// @par ���̰߳�ȫ��
/// <��/��>[��˵��]
/// @par �쳣ʱ��ȫ��
/// <��/��>[��˵��]
/// @note         -
/// 
/// @file         NetTreatMent.cpp
/// @brief        <ģ�鹦�ܼ���>
/// @author       Li.xl
/// @version      1.0
/// @date         2011/05/25
/// @todo         <����Ҫ��������>
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NetTreatMent.h"
#include <fstream>

namespace FWAYNET
{
	CNetTreatment::CNetTreatment()
	{
	}

	CNetTreatment::~CNetTreatment()
	{
	}

	void CNetTreatment::e_SendLoop()
	{
		/// �����̴߳���������
		while(true)
		{
			/// ��֤���ӺϷ���
			if(NULL == m_bConnect || 0 >= (int)m_SendBufferArray.size())
			{
#ifdef WIN32
				Sleep(1);
#else
				usleep(1000);
#endif
				continue;
			}

			/// �ڻ����в���Ҫ���͵�����
			T_BufferStruct* pBuffer = NULL;
			if(0 < (int)m_SendBufferArray.size())
			{
				CLightAutoLock lckSendBufferLock(&m_SendBufferLock);
				pBuffer = (T_BufferStruct* )*m_SendBufferArray.begin();
				if(NULL != pBuffer)
				{
					m_SendBufferArray.erase(m_SendBufferArray.begin());
				}
			}
			else
			{
				/// ���û���ҵ����͵������򷵻�
#ifdef WIN32
				Sleep(1);
#else
				usleep(1000);
#endif
				continue;
			}
			/// ������ݴ�������
			if(NULL != pBuffer)
			{
				/// ��ֹ���߳�ͬʱ��������
				CLightAutoLock lckSend(&m_SendLock);

				e_SendStruct(m_nSocketId, pBuffer->nJobType, pBuffer->nSST, pBuffer->nSize, pBuffer->pBuf);

				if(pBuffer->pBuf)
				{
					delete pBuffer->pBuf;
					pBuffer->pBuf = NULL;
				}
				delete pBuffer;
				pBuffer = NULL;
			}
		}
	}

	void CNetTreatment::e_ReceiveLoop()
	{
		BYTE* pBuffer = NULL;
		BYTE* pBufferTmp = NULL;
		/// ��ȡ��ȡ�����ݳ���
		long lReadCount = 0;
		/// ���ݳ���
		long lDataLen = 0;
		/// �������ͣ�������1���ֽڴ�С
		BYTE BtDataType = 0;
		while(true)
		{
			if(0 >= m_nSocketId)
			{
				/// ��������
				e_ReconnectServer();
#ifdef WIN32
				Sleep(1000);
#else
				usleep(1000000);
#endif
				continue;
			}

			/// ��ȡ���ݳ���
			lReadCount = e_ReceiveData(m_nSocketId, (char* )&lDataLen, sizeof(lDataLen));

			/// û�ж�ȡ������
			if(-1 == lReadCount)
			{
				printf("NetClient Read 1 ReadCount = %d, DataLen = %d This is Error Data\r\n",
					lReadCount, lDataLen);

				/// �Ͽ�����
				e_CloseSocket();
				continue;
			}

			/// ��ȷ��ȡ�������ݳ���
			if(0 < lDataLen)
			{
				/// = �Ƚ���һ���ֽڣ�WINCE�£�����ת���ṹָ��ʱ����ַҪ��4�ֽڵı������룬
				/// = ��Ϊ�и�����ͷ1+4�������Ƚ���һ���ֽڣ������ת���ᰴ4�ֽڽ��ж�����
				/// = ��ȡ�������ͳ���
				lReadCount = e_ReceiveData(m_nSocketId, (char* )&BtDataType, sizeof(BtDataType));
				/// = �����ȡ�����������ͳ��Ⱥͱ�ʡ���������ͳ��Ȳ�һ����Ͽ�����
				if(lReadCount != sizeof(BtDataType))
				{
					printf("NetClient Read 2 ReadCount = %d, DataLen = %d This is Error Data\r\n",
							lReadCount, sizeof(BtDataType));
					
					/// �Ͽ��׽�������
					e_CloseSocket();
					continue;
				}

				/// �����EXTRA_BUFFER_SIZE��С�ڴ�
				pBuffer = new BYTE[lDataLen + EXTRA_BUFFER_SIZE];
				if(NULL == pBuffer)
				{
					continue;
				}
				/// = ֻ�Զ������ڴ��ʼ������Ϊǰ����ڴ�һ���ᱻ���紫������ݸ��ǣ�
				/// = ����memsetʱ������Ҫ��ȥsizeof(DataType)����Ϊ����ǵ������յģ�δ������DataLen��
				memset(&pBuffer[lDataLen - sizeof(BtDataType)], 0x00, EXTRA_BUFFER_SIZE);
			}

			/// ��ȥ�Ѿ�������sizeof(DataType)��С��������������
			lReadCount = e_ReceiveData(m_nSocketId, (char* )pBuffer, lDataLen-sizeof(BtDataType));
			/// �����ȡ�����ݲ���ȷ����ر�����
			if(lReadCount != lDataLen - sizeof(BtDataType))
			{
				printf("NetClient Read 3 ReadCount = %d, DataLen = %d This is Error Data\r\n", 
					lReadCount, lDataLen);
				/// �Ͽ��׽�������
				e_CloseSocket();
				delete[] pBuffer;
				pBuffer = NULL;
				continue;
			}
			/// ���ݲ���ȷ����
			if(-1 == lReadCount)
			{
				delete[] pBuffer;
				pBuffer = NULL;
				continue;
			}
			e_ProcessReceiveData(BtDataType, pBuffer);
			if(NULL != pBuffer)
			{
				delete[] pBuffer;
				pBuffer = NULL;
			}
		}
	}

	void CNetTreatment::e_ProcessReceiveData(BYTE BtDataType, void* pData)
	{
		UINT nStructType = 0;
		BYTE* pBtDataTmp = (BYTE* )pData;
		
		switch((_eJobDataType)BtDataType)
		{
		case JDT_TextData:
			{
				/// �����ı���Ϣ
				break;
			}
		case JDT_StructData:
		case JDT_ArrStructData:
			{
				/// ���÷�������ʱ��
				m_tSendTime = 0;

				memcpy(&nStructType, pBtDataTmp, sizeof(nStructType));
				/// ����ָ�������ṹ���������ݣ�ָ����������
				pBtDataTmp += sizeof(nStructType);
				/// = DataLenΪ���ݰ����ݴ�С����ȥBYTE DataType���ݰ����ͣ���ȥsizeof(StructType)�ṹ�����ͣ�
				/// = ���Ϊ��������ʵ�ʴ�С
				e_PackageReceiveStruct(nStructType, pBtDataTmp);
				break;
			}
		}
	}

	void CNetTreatment::e_PackageReceiveStruct(UINT nSendStructType, void* pData)
	{
		/// ������յ��Ľṹ��
		T_RSStructData* psttRSStructData = (T_RSStructData* )pData;

		if(NULL == psttRSStructData)
		{
			printf("Get The T_RSStructData Error.\r\n");
			return;
		}

		/// ������ڻص�����
		if(NULL != m_pReceiveStructCallBack)
		{
			/// �������ݸ��ص�����
			m_pReceiveStructCallBack(psttRSStructData->nSRStructType,
			psttRSStructData->nDataCount, psttRSStructData->szData);
		}
	}

	/// �õ���ǰ����Ŀ¼·��
	string CNetTreatment::e_GetFilePath()
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

	/// ��ȡ�����ļ���ָ�����ӵ�Ip
	bool CNetTreatment::e_GetNetServiceIp(char* pszServiceIp, int nSizeIp)
	{
		START_DEBUG_INFO;
		/// ��ȡ����Ip
		if(NULL == pszServiceIp)
		{
			END_DEBUG_INFO
			return false;
		}
		/// ��ȡ·��
		string strFilePath = e_GetFilePath();

		strFilePath += "ClientConfig.ini";
		/// �������ļ�׼����ȡ
		ifstream fsln(strFilePath.c_str());

		/// �����ȡ��������һ�е��ַ���
		string strLine = "";
		strLine.clear();
		/// �����ȡ���õ��м�����ַ���
		string strTemp,strTempThe;
		strTemp.clear();
		strTempThe.clear();

		int nstrIndex = 0;
		if(NULL != fsln)
		{
			while(getline(fsln, strLine))
			{
				if(0 < strLine.length())
				{
					nstrIndex = 0;
					if(0 >= strlen(pszServiceIp))
					{
						nstrIndex = strLine.find("NetSerciceIP:");
						/// ����ҵ��ַ���β��û���ҵ�
						if(nstrIndex != string::npos)
						{
							strTempThe = "NetSerciceIP:";
							nstrIndex += strTempThe.length();
							strTemp = strLine.substr(nstrIndex, strLine.length());
							/// ȥ���ַ�����β�Ŀո�
							strTemp.erase(strTemp.find_last_not_of(" ") + 1);
							strTemp.erase(0, strTemp.find_first_not_of(" "));
							snprintf(pszServiceIp, nSizeIp, "%s", strTemp.c_str());
							continue;
						}
					}
				}
			}
		}
		else
		{
			printf("\r\n��ȡ���������ļ�ʧ��...\r\n");
			END_DEBUG_INFO;
			return false;
		}

		if('\0' == pszServiceIp[0] || '\r' == pszServiceIp[0] || '\n' == pszServiceIp[0])
		{
			END_DEBUG_INFO;
			return false;
		}


		END_DEBUG_INFO;
		return true;
	}
}