////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �߳�������Ϣ����
/// @par ����ļ�
/// TTask.cpp
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         TTask.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/11/19
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TPOOL_CTTTASK_H_H__
#define __TPOOL_CTTTASK_H_H__

#include "common/CommTypes.h"
#include "TPool/TPoolDef.h"

class CTTask
{
public:
	CTTask(void* pFunction =  NULL);
	virtual ~CTTask();

	/// ==============================================
	/// @par ���� 
	/// ��ȡ����ID
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	����ID
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	ULONG e_GetTaskID();

	/// ==============================================
	/// @par ���� 
	/// ��������ID
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		ulTaskID		����ID
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	void e_SetTaskID(ULONG ulTaskID);

	/// ==============================================
	/// @par ���� 
	/// ��ȡ�������ȼ�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	�������ȼ����μ�_eThreadTaskPriority
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	USHORT e_GetPriority();

	/// ==============================================
	/// @par ���� 
	/// �豸�������ȼ�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		usPriority		���ȼ���Ϣ
	/// = �μ� _eThreadTaskPriority
	/// @return 	-
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	void e_SetPriority(USHORT usPriority);

	/// ==============================================
	/// @par ���� 
	/// �߳�������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	������ɵ�״̬
	/// @note 		Creat By li.xl 2013/11/19 
	/// ==============================================
	virtual bool e_TTaskProcess();

private:
	// ����ID
	ULONG	m_ulTaskID;
	// �������ȼ�
	USHORT	m_usPriority;
protected:
	// �߳�����ص��¼�����
	OnThreadProcess m_pfnThreadProcess;
};

#endif	// __TPOOL_CTTTASK_H_H__