/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// ���紦��ģ��
/// @par ����ļ�
/// ���ô��ļ�ͷ�������ļ�ͷ
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetIOCPDef.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/06/08
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __LEE_NET_IOCP_DEF_H_H__
#define __LEE_NET_IOCP_DEF_H_H__

#include "Common/CommTypes.h"
#include "Common/AutoLock.h"
#include "common/NetCoreDef.h"

class CNetBuffer;
typedef map<ULONG, CNetBuffer*, greater<ULONG>>		CBufferMapToPtr;

/// ������߳�����
#define MAX_PROCESSER_NUMBERS	20
/// �����޹̶������������б�
#define	NO_NUM_FREE_LIST		0
/// ���ӳ�����
#define NETCORE_TCP_CONPOOL_NUM  1024
/// ���������
#define NETCORE_BUFFER_POOL_NUM	 2048

/// IO�����̲߳�������
enum _eIOProcessType
{
	IOPT_None			= 0,						///< δ֪�Ĳ�������
	IOPT_TACCEPT		= 1,						///< TCP�ɹ�����
	IOPT_TCPRECV		= 2,						///< TCP׼����������
	IOPT_TRECVED		= 3,						///< TCP�����������
	IOPT_TCPSEND		= 4,						///< TCP׼����������
	IOPT_TSENDED		= 5,						///< TCP�����������
	IOPT_UDPRECV		= 6,						///< UDP�����������
	IOPT_UDPSEND		= 7,						///< UDP�����������
};

/// �ͻ������ӽṹ��
typedef struct _tClientContext
{	
	ULONG					ulContextID;			///< ������ϢID
	SOCKET					sSocketID;				///< �����׽���	
	char					szClientIP[16];			///< �ͻ�������IP
	USHORT					usClientPort;			///< �ͻ��˶˿ں�
	SHORT					sNumOfIORef;			///< IO���ü���
	USHORT					usReadSeqNum;			///< ���յ����кű���
	USHORT					usSendSeqNum;			///< ���͵����кű���
	USHORT					usCurReadSeqNum;		///< ��ǰ���յ����к�
	USHORT					usCurSendSeqNum;		///< ��ǰ���յ����к�
	CLightCritSec			ContextCritSec;			///< ������(�޸Ļ��߶�ȡʱʹ��)
	CBufferMapToPtr			CReadBufferMap;			///< ���յĻ������򼯺�
	CBufferMapToPtr			CSendBufferMap;			///< ���յĻ������򼯺�
	CNetBuffer*				pCurrentBuffer;			///< ��ǰ����������

	/// �����ʼ��
	_tClientContext()
	{
		e_InitStruct();
	}

	/// ��ʼ������
	void e_InitStruct()
	{
		ulContextID = 0;
		sSocketID = NULL;		
		memset(szClientIP, 0x00, sizeof(szClientIP));
		usClientPort = 0;
		sNumOfIORef = 0;
		usReadSeqNum = 0;
		usSendSeqNum = 0;
		usCurReadSeqNum = 0;
		usCurSendSeqNum = 0;
		CReadBufferMap.clear();
		CSendBufferMap.clear();
		pCurrentBuffer = NULL;
	}

	/// ���ӽ�����������
	void e_AddReadSeqNum()
	{
		ContextCritSec.e_Lock();
		usReadSeqNum++;
		ContextCritSec.e_Unlock();
	}

	/// ���ӷ�����������
	void e_AddSendSeqNum()
	{
		ContextCritSec.e_Lock();
		usSendSeqNum++;
		ContextCritSec.e_Unlock();
	}

	/// ���ӵ�ǰ�������ݵ����к�
	void e_InCreaseCurReadSeqNum()
	{
		ContextCritSec.e_Lock();
		usCurReadSeqNum++;
		ContextCritSec.e_Unlock();
	}

	/// ���ӵ�ǰ�������ݵ����к�
	void e_InCreaseCurSendSeqNum()
	{
		ContextCritSec.e_Lock();
		usCurSendSeqNum++;
		ContextCritSec.e_Unlock();
	}

	/// ����IO���ü���
	SHORT e_EnterRefNum()
	{
		return ++sNumOfIORef;
	}

	/// ����IO���ü���
	SHORT e_ExitRefNum()
	{
		return --sNumOfIORef;
	}

}T_ClientContext, *LP_ClientContext;

#endif	/// __LEE_NET_IOCP_DEF_H_H__