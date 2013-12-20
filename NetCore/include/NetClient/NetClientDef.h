////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �ͻ����������������ʽ
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

#ifndef __FWAYNET__NETCLIENTDEF_H__
#define __FWAYNET__NETCLIENTDEF_H__

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
typedef void (*OnRecvDataCallBack)(USHORT usNetType, ULONG ulContextID, const char* pszClientIP, 
	USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount, 
	SHORT sSNum, SHORT sENum, int nDatalen, void* pData);

/// �����������
enum _eCmdOBJType
{
	COBJT_None = 0,
	COBJT_Test = 1,
};

/// ���Խṹ��
typedef struct _tTestStruct
{
	char	szTime[32];
	char	szAddr[64];
	char	szName[32];
}T_TestStruct, *LP_TestStruct;

#endif /// __FWAYNET__NETCLIENTDEF_H__
