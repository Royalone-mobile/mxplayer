////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �ͻ����������Ԥ��
/// @par ����ļ�
/// ���ô��ļ��������ļ�ͷ
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetClientDef.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/12/20
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FWAYNET__NETCLIENTDEF_H_H__
#define __FWAYNET__NETCLIENTDEF_H_H__

/// ==============================================
/// @par ���� 
/// ������յ�������
/// @param 
/// [in,out]	<������>		<����˵��>
///	[in]		usNetType		���ͺͽ������ݵ��������� �μ�_eNetTransfType
/// [in]		pszClientIP		�ͻ���IP
/// [in]		usClientPort	�ͻ��˶˿ں�
/// [in]		sDataType		��������
/// [in]		nOBJType		����/�ṹ����
/// [in]		sOBJCount		����/�ṹ����
/// [in]		sSNum			����/�ṹ���鿪ʼ����(�����б���ʱ��sOBJCount���ʹ��)
/// [in]		sENum			����/�ṹ�����������(�����б�������sOBJCount���ʹ��)
/// [in]		nDatalen		���ݳ���
/// @return 	-
/// @note 		Creat By li.xl 2013/12/21 
/// ==============================================
typedef void (*OnRecvDataCallBack)(USHORT usNetType, const char* pszClientIP, 
	USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount, 
	SHORT sSNum, SHORT sENum, int nDatalen, void* pData);

/// �ͻ��˳�ʼ����Ϣ�ṹ
typedef struct _tInitNetClient
{
	char	szTCPServerIP[16];		///< TCP������IP
	USHORT	usTCPServerPort;		///< TCP�������˿�
	USHORT	usLocalUDPPort;			///< ����UDP�˿ں�
	USHORT	usUDPJoinGroup;			///< �Ƿ����UDP�鲥
	char	szUDPGroupIP[16];		///< �鲥��IP

	_tInitNetClient()
	{
		e_InitStruct();
	}

	void e_InitStruct()
	{
		memset(szTCPServerIP, 0x00, sizeof(szTCPServerIP));
		memset(szUDPGroupIP, 0x00, sizeof(szUDPGroupIP));
		usTCPServerPort = 0;
		usLocalUDPPort = 0;
		usUDPJoinGroup = 0;
	}
}T_InitNetClient, *LP_InitNetClient;

#endif /// __FWAYNET__NETCLIENTDEF_H_H__