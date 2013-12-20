///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// ���ݻ����б������󣨻���أ�
/// @par ����ļ�
/// BufferPool.cpp
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         BufferPool.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/06/14
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __LEE_CONTEST_BUFFER_POOL_MANAGER_H_H__
#define __LEE_CONTEST_BUFFER_POOL_MANAGER_H_H__

#include "NetBuffer.h"

/// ���建�������������
typedef vector<CNetBuffer* >						CBufferPtrArray;

class CBufferPool
{
public:
	/// ==============================================
	/// @par ���� 
	/// Ĭ�Ϲ��캯��
	/// @return 	-
	/// @note 		by li.xl 2013/06/19 
	/// ==============================================
	CBufferPool();

	/// ==============================================
	/// @par ���� 
	/// ����ָ���������б��캯��
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		usBufCount		ָ����������
	/// @return 	-
	/// @note 		by li.xl 2013/06/19 
	/// ==============================================
	CBufferPool(USHORT usBufCount);

	/// ==============================================
	/// @par ���� 
	/// Ĭ����������
	/// @return 	-
	/// @note 		by li.xl 2013/06/19 
	/// ==============================================
	~CBufferPool();

public:

	/// ==============================================
	/// @par ���� 
	/// ���ÿ��л�������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		usBufCount		ָ�����������
	/// @return 	-
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	void e_SetBuffFreeCount(USHORT usBufCount);

	/// ==============================================
	/// @par ���� 
	/// ��ȡ���л����б��С
	/// @param 
	/// @return 	USHORT	���ص�����
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	USHORT e_GetFreeListSize();

	/// ==============================================
	/// @par ���� 
	/// ��ȡʹ�û����б��С
	/// @param 
	/// @return 	USHORT	���ص�����
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	USHORT e_GetBusyListSize();

	/// ==============================================
	/// @par ���� 
	/// ���б��л�ȡ���û������ݶ�����Ϣ
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pNetBuffer		�������ָ��
	/// [in]		usIOType		�����������
	/// @return 	bool			��ȡ�ɹ�����ʧ��
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	bool e_AllocateBuffer(CNetBuffer*& pNetBuffer, USHORT usIOType);

	/// ==============================================
	/// @par ���� 
	/// ������绺�����ݰ�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pSrcBuffer		���ݻ���ԭ����
	/// [in]		pDesBuffer		Ŀ�����ݻ���
	/// [in]		unSplitSize		Ҫ��ֵ����ݴ�С
	/// @return 	-
	/// @note 		by li.xl 2013/06/21 
	/// ==============================================
	bool e_SplitNetBuffer(CNetBuffer*& pSrcBuffer, CNetBuffer*& pDesBuffer, UINT unSplitSize);

	/// ==============================================
	/// @par ���� 
	/// �ͷ�ʹ�õĻ������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pNetBuffer		�������ָ��
	/// @return 	bool			�ͷųɹ�����ʧ��
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	bool e_ReleaseBuffer(CNetBuffer*& pNetBuffer);

private:
	/// ==============================================
	/// @par ���� 
	/// ���ӻ�����Ϣ���󵽿����б�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pNetBuffer		�������ָ��
	/// @return 	-
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	bool i_AddBufferToFreeList(CNetBuffer*& pNetBuffer);

	/// ==============================================
	/// @par ���� 
	/// ������ݻ����б�
	/// @param 
	/// @return 	-
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	void i_ClearBufferList();

	/// ==============================================
	/// @par ���� 
	/// ���ɻ���ID
	/// @param 
	/// @return 	�������ɳɹ������ɻ�����Ա��
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	ULONG i_BuildNetBufferID();
private:
	/// ������������(Ϊ NO_NUM_FREE_LIST ��ʾ�޹̶������б�)
	USHORT					m_usFreeBufCount;
	/// �����б���
	CLightCritSec			m_CFreeListCrit;
	/// �����б������������б�
	CBufferPtrArray			m_ArrayFreeBufList;
	/// �����б���
	CLightCritSec			m_CBusyListCrit;
	/// ʹ���б���Hash�����б�
	CBufferMapToPtr			m_ArrayBusyBufList;
	/// ������Ϣ�Ա�ű���
	ULONG					m_ulConstomBufferID;
};

#endif	/// __LEE_CONTEST_BUFFER_POOL_MANAGER_H_H__