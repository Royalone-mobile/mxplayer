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

#ifndef __FWAYNET_LOTTERY_NETCLIENTDEF_H__
#define __FWAYNET_LOTTERY_NETCLIENTDEF_H__

/// �����������
enum _eCmdOBJType
{
	COBJT_None = 0,
	COBJT_Test = 1,
};

/// ���սṹ���ݻص�����
typedef void (*OnRecvDataCallBack)(UINT nSttType, int nCount, void* pData);

/// ���Խṹ��
typedef struct _tTestStruct
{
	char	szTime[32];
	char	szAddr[64];
	char	szName[32];
}T_TestStruct, *LP_TestStruct;

#endif /// __FWAYNET_LOTTERY_NETCLIENTDEF_H__
