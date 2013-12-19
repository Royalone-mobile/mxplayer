////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// <����ũ���ͻ�������ṹ����ö�ٶ���>
/// @par ����ļ�
/// CommTypes.h 
/// @par ������ϸ����
/// <����ṹ����ö�ٶ���>
/// @par ���̰߳�ȫ��
/// <��/��>[��˵��]
/// @par �쳣ʱ��ȫ��
/// <��/��>[��˵��]
/// @note         -
/// 
/// @file         NetClientDef.h
/// @brief        <ģ�鹦�ܼ���>
/// @author       Li.xl
/// @version      1.0
/// @date         2011/05/25
/// @todo         <����Ҫ��������>
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FWAYNET_LOTTERY_NETCLIENTDEF_H__
#define __FWAYNET_LOTTERY_NETCLIENTDEF_H__

namespace FWAYNET
{
	/// ���սṹ���ݻص�����
	typedef void (*OnRecvDataCallBack)(UINT nSttType, int nCount, void* pData);

	/// ���Խṹ��
	typedef struct _tTestStruct
	{
		char	szTime[32];
		char	szAddr[64];
		char	szName[32];
	}T_TestStruct, *LP_TestStruct;
}

#endif /// __FWAYNET_LOTTERY_NETCLIENTDEF_H__
