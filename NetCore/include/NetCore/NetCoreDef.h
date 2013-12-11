/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �����ṹ���������Ͷ���
/// @par ����ļ�
/// Ӧ�ô��ļ�ͷ�������ļ�
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetCoreDef.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/06/14
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __LEE_NET_CORE_DEF_H_H__
#define __LEE_NET_CORE_DEF_H_H__

/// ==============================================
/// @par ���� 
/// ����������ɻص�����
/// @param 
/// [in,out]	<������>		<����˵��>
///	[in]		usNetDataType	���ͺͽ������ݵ��������� �μ�_eNetDataType
/// [in]		ulContextID		����TCP���Ӵ˲�������0��UDP���Ӵ˲���Ϊ0
/// [in]		pszClientIP		�ͻ���IP
/// [in]		usClientPort	�ͻ��˶˿ں�
/// [in]		pData			�����ֶ�
/// [in]		nDatalen		���ݳ���
/// @return 	-
/// @note 		by li.xl 2013/06/14 
/// ==============================================
typedef void (*OnRecvDataCallBack)(USHORT usNetDataType, ULONG ulContextID,
	const char* pszClientIP, USHORT usClientPort, void* pData, int nDatalen);

/// ==============================================
/// @par ���� 
/// ����������ɻص�����
/// @param 
/// [in,out]	<������>		<����˵��>
///	[in]		usNetDataType	���ͺͽ������ݵ��������� �μ�_eNetDataType
/// [in]		ulContextID		����TCP���Ӵ˲�������0��UDP���Ӵ˲���Ϊ0
/// [in]		pszClientIP		�ͻ���IP
/// [in]		usClientPort	�ͻ��˶˿ں�
/// [in]		pData			�����ֶ�
/// [in]		nDatalen		���ݳ���
/// @return 	-
/// @note 		by li.xl 2013/06/14 
/// ==============================================
typedef void (*OnSendDataCallBack)(USHORT usNetDataType, ULONG ulContextID,
	const char* pszClientIP, USHORT usClientPort, void* pData, int nDatalen);

/// ==============================================
/// @par ���� 
/// TCP�ͻ��˳ɹ�����֪ͨ
/// @param 
/// [in,out]	<������>		<����˵��>
/// [in]		ulContextID		�ͻ�������ID
/// [in]		pszClientIP		�ͻ���IP
/// [in]		usClientPort	�ͻ��˶˿ں�
/// @return 	-
/// @note 		by li.xl 2013/06/14 
/// ==============================================
typedef void (*OnConectionCallBack)(ULONG ulContextID, const char* pszClientIP, USHORT usClientPort);

/// ==============================================
/// @par ���� 
/// TCP�ͻ��˶Ͽ�����֪ͨ
/// @param 
/// [in,out]	<������>		<����˵��>
/// [in]		ulContextID		�ͻ�������ID
/// [in]		pszClientIP		�ͻ���IP
/// [in]		usClientPort	�ͻ��˶˿ں�
/// @return 	-
/// @note 		by li.xl 2013/06/14 
/// ==============================================
typedef void (*OnDisConectionCallBack)(ULONG ulContextID, const char* pszClientIP, USHORT usClientPort);

/// ==============================================
/// @par ���� 
/// ��ӡ��־�ص���������
/// @param 
/// [in,out]	<������>		<����˵��>
///	[in]		pszfmt			Ҫ��ӡ����־��Ϣ
/// @return 	-
/// @note 		by li.xl 2013/07/23 
/// ==============================================
typedef void (*OnPrintLog)(const char* pszfmt, ...);

/// ���ͺͽ������ݵ���������
enum _eNetDataType
{
	NDT_None		= 0,	///< δ֪
	NDT_TCPData		= 1,	///< TCP����
	NDT_UDPData		= 2,	///< UDP����
};

/// �����������
enum _eNetCoreNetType
{
	NCNT_None	= 0,		///< �ޣ�δ֪
	NCNT_TCP	= 1,		///< TCP �������
	NCNT_UDP	= 2,		///< UDP �������
	NCNT_Both	= 3,		///< ���� UDP �� TCP
};

/// ��������ʼ���ṹ����Ϣ
typedef struct _tNetInitStruct
{
	USHORT				usServerPort;					///< ����˿ں�
	USHORT				usServerNetType;				///< �������������			(�μ� _eNetCoreNetType)
	USHORT				usMaxIOWorkers;					///< ������߳�			(�˲���Ϊ0��ϵͳ��Ϊ��ѡ�����ŵĹ����߳���)
	USHORT				usPendReadsNum;					///< Ͷ�ݶ�ȡ���ݵ�����
	USHORT				usFreeBufMaxNum;				///< �����л�����			(�˲���Ϊ0��Ĭ�Ͽ��л�������2048)
	USHORT				usFreeConnMaxNum;				///< ������������			(�˲���Ϊ0��Ĭ�Ͽ�����������1024)
	bool				bOrderSend;						///< �Ƿ�˳����
	bool				bOrderRead;						///< �Ƿ�˳���ȡ
	bool				bUDPJoinGroup;					///< �Ƿ�����鲥
	char				szUDPGroupIP[16];				///< �����鲥�ĵ�ַ
	USHORT				usUDPRevcNum;					///< Ͷ��UDP���յ����ݲ�������

	/// �����ʼ��
	_tNetInitStruct()
	{
		e_InitStruct();
	}

	/// ��ʼ���ṹ��
	void e_InitStruct()
	{
		usServerPort = 0;
		usServerNetType = NCNT_None;
		usMaxIOWorkers = 0;
		usPendReadsNum = 4;
		usFreeBufMaxNum = 0;
		usFreeConnMaxNum = 0;
		bOrderSend = true;
		bOrderRead = true;
		bUDPJoinGroup = false;
		memset(szUDPGroupIP, 0x00, sizeof(szUDPGroupIP));
		usUDPRevcNum = 32;
	}

}T_NetInitStruct, *LP_NetInitStruct;

#endif /// __LEE_NET_CORE_DEF_H_H__