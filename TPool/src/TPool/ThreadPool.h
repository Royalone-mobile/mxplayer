////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �̳߳ع������
/// @par ����ļ�
/// CThreadPool.cpp
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         ThreadPool.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/11/19
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TPOOL_CTHREAD_POOL_H_H__
#define __TPOOL_CTHREAD_POOL_H_H__

#include "ThreadBase.h"
#include "TTaskList.h"

///< �����̶߳���
typedef list<CThreadBase *>		ThreadList;
typedef deque<CThreadBase *>	ThreadQueue;

/// �̳߳��߳��������
#define MAX_THREAD_POOL		500

class CThreadPool
{
public:
	CThreadPool();
	virtual ~CThreadPool();

	/// ==============================================
	/// @par ���� 
	/// ��ʼ���̳߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		usThreadCount	�߳�����
	/// @return 	��ʼ���ɹ����
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool e_InitTPool(USHORT usThreadCount);

	/// ==============================================
	/// @par ���� 
	/// ��ȡ����Ψһ���к�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	ULONG			����IDΨһ��ʶ
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	ULONG e_GetTaskOrderID();

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
	bool e_AddTTask(OnThreadProcess pfnThreadProcess, 
		ULONG ulTaskID, USHORT usPriority = ETTP_NORMAL);

	/// ==============================================
	/// @par ���� 
	/// ɾ���߳�����[����߳�����δ�����ɴ��߳������б���ɾ��]
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		ulTaskID		����ID
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool e_DelTTask(ULONG ulTaskID);

	/// ==============================================
	/// @par ���� 
	/// ��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pThread			�̶߳���ָ��
	/// = ����̴߳��ڣ���ʹ�ø��̶߳���������
	/// = ����̲߳����ڣ���ʹ�ÿ����߳��б��е�ĳ�̴߳�������
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool e_ProcessTask(CThreadBase* pThread = NULL);

	/// ==============================================
	/// @par ���� 
	/// �̴߳������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pThread			�̶߳���ָ��
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool e_Processed(CThreadBase* pThread);

private:
	/// ==============================================
	/// @par ���� 
	/// ���ӵ�ʹ���߳��б�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pThread			�̶߳���ָ��
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool i_AddToBusyList(CThreadBase* pThread);

	/// ==============================================
	/// @par ���� 
	/// ��ʹ���߳��б���ɾ��
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pThread			�̶߳���ָ��
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool i_DelFromBusyList(CThreadBase* pThread);

	/// ==============================================
	/// @par ���� 
	/// ���ӵ������б�β��
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pThread			�̶߳���ָ��
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	bool i_PushToFreeList(CThreadBase* pThread);

	/// ==============================================
	/// @par ���� 
	/// �ӿ����б�ͷ����ȡ�̶߳���ָ��
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	 -
	/// @return 	CThreadBase*	�̶߳���ָ��
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	CThreadBase* i_PopFromFreeList();

	/// ==============================================
	/// @par ���� 
	/// �����̳߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	void i_DestroyPool();

private:
	// �������
	CTTaskList		m_CTaskList;
	// ʹ���߳��б���
	CLightCritSec	m_CsTBusyLock;
	// ʹ���߳��б�
	ThreadList		m_TBusyList;
	// �����߳��б���
	CLightCritSec	m_CsTFreeLock;
	// �����߳��б�
	ThreadQueue		m_TFreeList;
	// �߳�����
	USHORT			m_usThreadCount;
};

#endif	// __TPOOL_CTHREAD_POOL_H_H__