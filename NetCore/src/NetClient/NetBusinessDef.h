////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// ҵ������Ϣ����ģ��
/// @par ����ļ�
/// ���ô��ļ�ͷ�������ļ�
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetBusinessDef.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/12/21
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FWAY_NETCORE_NETBUSSINESS_H_H__
#define __FWAY_NETCORE_NETBUSSINESS_H_H__

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

#endif	/// __FWAY_NETCORE_NETBUSSINESS_H_H__