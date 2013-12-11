////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �߳������б�������
/// @par ����ļ�
/// TTaskList.cpp
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         TTaskList.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/11/19
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TPOOL_TTASK_LIST_H_H__
#define __TPOOL_TTASK_LIST_H_H__

#include "common/CommTypes.h"
#include "common/AutoLock.h"
#include "TTask.h"

///< ������ж���
typedef deque<CTTask* >	TQueue;

class CTTaskList
{
public:
	CTTaskList();
	virtual ~CTTaskList();

	/// ==============================================
	/// @par ���� 
	/// ��������ID
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	�����´�������ID
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	ULONG e_CreatTaskID();

	/// ==============================================
	/// @par ���� 
	/// �����߳�����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pfnThreadProcess	�̻߳ص��¼�����ָ��
	/// [in]		ulTaskID			����ID
	/// [in]		usPriority			�������ȼ�
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool e_AddTaskList(OnThreadProcess pfnThreadProcess, ULONG ulTaskID, USHORT usPriority);

	/// ==============================================
	/// @par ���� 
	/// ׷������������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pfnThreadProcess�̴߳���ص�����
	/// [in]		ulTaskID		����ID
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool e_PushTask(OnThreadProcess pfnThreadProcess, ULONG ulTaskID);

	/// ==============================================
	/// @par ���� 
	/// �������б�ͷ����������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pfnThreadProcess�̴߳���ص�����
	/// [in]		ulTaskID		����ID
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool e_FrontTask(OnThreadProcess pfnThreadProcess, ULONG ulTaskID);

	/// ==============================================
	/// @par ���� 
	/// �Ӷ������Ƴ�������Ϣ
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	CTTask	������Ϣ����ָ��
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	CTTask* e_PopTask();

	/// ==============================================
	/// @par ���� 
	/// ɾ������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		ulTaskID		����ID
	/// @return 	bool		ɾ����������Ƿ�ɹ�
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool e_DelTask(ULONG ulTaskID);

	/// ==============================================
	/// @par ���� 
	/// ����������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	����Ƿ�ɹ�
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool e_ClearList();

private:
	// ��ǰ����������
	CLightCritSec	m_CsOrderLock;
	// ��ǰ��������
	ULONG			m_ulCurTaskOrder;
	// ���������
	CLightCritSec	m_CsTaskLock;
	// �������
	TQueue			m_CTaskQueue;
};

#endif	// __TPOOL_TASK_LIST_H_H__