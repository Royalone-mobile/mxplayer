////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �ͻ��������б�������
/// @par ����ļ�
/// ContextList.cpp
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         ContextList.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/06/14
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __LEE_CONTEXT_LIST_MANAGER_H_H__
#define __LEE_CONTEXT_LIST_MANAGER_H_H__

#include "BufferPool.h"
#include "NetIOCPDef.h"

/// ����Hashӳ��
typedef map<ULONG, T_ClientContext*, greater<ULONG>>		CContextMapULONGToPtr;

typedef vector<T_ClientContext* >							CContextPtrArray;

class CContextList
{
public:
	/// ==============================================
	/// @par ���� 
	/// �����������������б��캯��
	/// @param 
	/// @return 	-
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	CContextList();

	/// ==============================================
	/// @par ���� 
	/// ����ָ���������б��캯��
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		usConFreeNum	ָ�����������
	/// @return 	-
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	CContextList(USHORT usConFreeNum);

	/// ==============================================
	/// @par ���� 
	/// ��������
	/// @param 
	/// @return 	-
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	virtual ~CContextList();

public:

	/// ==============================================
	/// @par ���� 
	/// ��ʼ�������б�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pBufferPool		���ݻ���������ָ��
	/// @return 	-
	/// @note 		by li.xl 2013/06/25 
	/// ==============================================
	void e_InitContextList(CBufferPool* pBufferPool);

	/// ==============================================
	/// @par ���� 
	/// ���ÿ�����������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		usConFreeNum	ָ�����������
	/// @return 	-
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	void e_SetFreeConNum(USHORT usConFreeNum);

	/// ==============================================
	/// @par ���� 
	/// ��ȡ���������б��С
	/// @param 
	/// @return 	USHORT	���ص�����
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	USHORT e_GetFreeListSize();

	/// ==============================================
	/// @par ���� 
	/// ��ȡʹ�������б��С
	/// @param 
	/// @return 	USHORT	���ص�����
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	USHORT e_GetBusyListSize();

	/// ==============================================
	/// @par ���� 
	/// ���б��л�ȡ����������Ϣ
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		psttContext		������Ϣ����ָ��
	/// @return 	bool			��ȡ�ɹ�����ʧ��
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	bool e_AllocateConntext(T_ClientContext*& psttContext);

	/// ==============================================
	/// @par ���� 
	/// �ͷ�ʹ��������Ϣ
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		psttContext		������Ϣ����ָ��
	/// @return 	bool			�ͷųɹ�����ʧ��
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	bool e_ReleaseConntext(T_ClientContext*& psttContext);

	/// ==============================================
	/// @par ���� 
	/// ��ȡ����ʹ�õĿͻ���������Ϣ
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		ulContextID		������ϢID
	/// @return 	T_ClientContext	���ص�������Ϣ����
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	T_ClientContext* e_GetConntext(ULONG ulContextID);

private:
	/// ==============================================
	/// @par ���� 
	/// ����������Ϣ�������б�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		psttContext		������Ϣ����ָ��
	/// @return 	-
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	bool i_AddContextToFreeList(T_ClientContext* psttContext);

	/// ==============================================
	/// @par ���� 
	/// ���������Ϣ�б�
	/// @param 
	/// @return 	-
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	void i_ClearContextList();

	/// ==============================================
	/// @par ���� 
	/// ����������ϢID
	/// @param 
	/// @return 	�������ɳɹ���������Ϣ�Ա��
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	ULONG i_BuildContextID();

	/// ==============================================
	/// @par ���� 
	/// �ͷ�Buffer�����б�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pBufferMap		����ӳ���б�
	/// @return 	-
	/// @note 		by li.xl 2013/06/25 
	/// ==============================================
	void i_ReleaseBufferMap(CBufferMapToPtr* pBufferMap);
private:
	/// ������������(Ϊ NO_NUM_FREE_LIST ��ʾ�޹̶������б�)
	USHORT					m_usConFreeNum;
	/// �����б���
	CLightCritSec			m_CFreeListCrit;
	/// �����б�
	CContextPtrArray		m_ContextFreeArray;
	/// �����б���
	CLightCritSec			m_CBusyListCrit;
	/// �����б�
	CContextMapULONGToPtr	m_ContextBusyArray;
	/// ������Ϣ�Ա�ű���
	ULONG					m_ulContextID;
	/// �������ݻ���������ָ��
	CBufferPool*			m_pBufferManager;
};

#endif	/// __LEE_CONTEXT_LIST_MANAGER_H_H__