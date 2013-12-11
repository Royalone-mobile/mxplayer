////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �̳߳ص��ýӿ�ģ��
/// @par ����ļ�
/// ���ô��ļ�ͷ�������ļ�
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         ITPool.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/11/19
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TPOOL_CITPOOL_H_H__
#define __TPOOL_CITPOOL_H_H__

#include "TPoolDef.h"

#ifdef TPOOL_EXPORTS
	#define IPOOL_API __declspec(dllexport)
#else
	#define IPOOL_API __declspec(dllimport)
#endif /// TPOOL_EXPORTS

class IPOOL_API CITPool
{
public:
	CITPool();
	~CITPool();

	/// ==============================================
	/// @par ���� 
	/// ��ʼ���̳߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		usThreadCount	�̳߳��߳�����
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool e_IInitTPool(USHORT usThreadCount);

	/// ==============================================
	/// @par ���� 
	/// ��ȡ����ID��ȫ��Ψһ
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-		
	/// @return 	ULONG		���ɵ�����IDΨһ��ʶ��
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	ULONG e_IGetTTaskID();

	/// ==============================================
	/// @par ���� 
	/// �����߳�����
	/// @param 
	/// [in,out]	<������>			<����˵��>
	///	[in]		pfnThreadProcess	�̻߳ص��¼�����ָ��
	/// [in]		ulTaskID			����ID
	/// [in]		usPriority			�������ȼ�
	/// @return 	���������߳������Ƿ�ɹ�
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool e_IAddPoolTask(OnThreadProcess pfnThreadProcess, 
		ULONG ulTaskID, USHORT usPriority = ETTP_NORMAL);

	/// ==============================================
	/// @par ���� 
	/// ɾ���߳�����[����߳�����δ�����ɴ��߳������б���ɾ��]
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		ulTaskID		����ID
	/// @return 	����ɾ���߳������Ƿ�ɹ�
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool e_IDelPoolTask(ULONG ulTaskID);
};

#endif	// __TPOOL_CITPOOL_H_H__