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
///	[in]		usNetType		���ͺͽ������ݵ��������� �μ�_eNetTransfType
/// [in]		ulContextID		����TCP���Ӵ˲�������0��UDP���Ӵ˲���Ϊ0
/// [in]		pszClientIP		�ͻ���IP
/// [in]		usClientPort	�ͻ��˶˿ں�
/// [in]		sDataType		��������
/// [in]		nOBJType		����/�ṹ����
/// [in]		sOBJCount		����/�ṹ����
/// [in]		sSNum			����/�ṹ���鿪ʼ����(�����б���ʱ��sOBJCount���ʹ��)
/// [in]		sENum			����/�ṹ�����������(�����б�������sOBJCount���ʹ��)
/// [in]		nDatalen		���ݳ���
/// [in]		pData			��������
/// @return 	-
/// @note 		by li.xl 2013/06/14 
/// ==============================================
typedef void (*OnRecvDataCallBack)(USHORT usNetType, ULONG ulContextID, const char* pszClientIP, 
	USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount, 
	SHORT sSNum, SHORT sENum, int nDatalen, void* pData);

/// ==============================================
/// @par ���� 
/// ����������ɻص�����
/// @param 
/// [in,out]	<������>		<����˵��>
///	[in]		usNetType		���ͺͽ������ݵ��������� �μ�_eNetTransfType
/// [in]		ulContextID		����TCP���Ӵ˲�������0��UDP���Ӵ˲���Ϊ0
/// [in]		pszClientIP		�ͻ���IP
/// [in]		usClientPort	�ͻ��˶˿ں�
/// [in]		sDataType		��������
/// [in]		nOBJType		����/�ṹ����
/// [in]		sOBJCount		����/�ṹ����
/// [in]		sSNum			����/�ṹ���鿪ʼ����(�����б���ʱ��sOBJCount���ʹ��)
/// [in]		sENum			����/�ṹ�����������(�����б�������sOBJCount���ʹ��)
/// [in]		nDatalen		���ݳ���
/// [in]		pData			��������
/// @return 	-
/// @note 		by li.xl 2013/06/14 
/// ==============================================
typedef void (*OnSendDataCallBack)(USHORT usNetType, ULONG ulContextID, const char* pszClientIP,
	USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount,
	SHORT sSNum, SHORT sENum, int nDatalen, void* pData);

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

/// ���紫������
enum _eNetTransfType
{
	NTT_None		= 0,	///< δ֪
	NTT_TCPData		= 1,	///< TCP����
	NTT_UDPData		= 2,	///< UDP����
};

/// �����������
enum _eNetServerNetType
{
	NSNT_None	= 0,		///< �ޣ�δ֪
	NSNT_TCP	= 1,		///< TCP �������
	NSNT_UDP	= 2,		///< UDP �������
	NSNT_Both	= 3,		///< ���� UDP �� TCP
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
		usServerNetType = NSNT_None;
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