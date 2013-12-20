/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// ���λ����
/// @par ����ļ�
/// ���ô��ļ������ļ�ͷ
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         RingBuf.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/01/17
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __COMM_RING_BUFFER_H_H__
#define __COMM_RING_BUFFER_H_H__
#include "CommTypes.h"
#include "AutoLock.h"

template <class T>
class CRingBuf
{
public:
	CRingBuf(UINT unSize = TRANSF_STREAM_BUF_SIZE);
	~CRingBuf();
public:
	/// ��ȡ����ؿɶ����ݿ����
	UINT i_GetBufSize();
	/// ��ȡ����ؿ�д���ݿ����
	UINT i_GetBufSpace();
public:
	/// ��ȡ���ݿ�
	bool e_PutData(T *data, UINT nNum);
	/// ��ȡ���ݿ�
	UINT e_GetData(T *data, UINT nNum, bool bIsTakeAway = true);
	/// �鿴���ݿ�
	UINT e_PeekData(T *data, UINT nNum);
	/// �ƶ��ɶ����ݿ�λ��
	bool e_SeekData(UINT nNum);
private:
	/// ��д�ڴ��ָ��
	T* m_rb;
	/// ָ��ǰӦ�ö�ȡλ��
	UINT m_unRPos;
	/// ָ��ǰӦ��д��λ��
	UINT m_unWPos;
	/// ������ڴ��������
	UINT m_unSize;
	/// ����
	CLightCritSec m_LockCrit;
};

template <class T>
CRingBuf<T>::CRingBuf(UINT unSize)
{
	m_unRPos = 0;
	m_unWPos = 0;
	m_unSize = unSize;
	m_rb = new T[m_unSize];
	memset(m_rb, 0x00, m_unSize * sizeof(T));
}

template <class T>
CRingBuf<T>::~CRingBuf()
{
	delete[] m_rb;
	m_rb = NULL;
}

// ���ݿɶ�����
template <class T>
UINT CRingBuf<T>::i_GetBufSize()
{
	CAutoLock AutoLock(&m_LockCrit);
	if((0 <= m_unRPos && m_unRPos < m_unSize) && (0 <= m_unWPos && m_unWPos < m_unSize))
	{
		if(m_unWPos >= m_unRPos)
		{
			return m_unWPos - m_unRPos;
		}
		else
		{
			return m_unSize - m_unRPos + m_unWPos;
		}
	}
	else
	{
		return 0;
	}
}

// ʣ���д�ռ�
template <class T>
UINT CRingBuf<T>::i_GetBufSpace()
{
	return m_unSize - 1 - i_GetBufSize();
}

// ��������д����
template <class T>
bool CRingBuf<T>::e_PutData(T* data, UINT unNum)
{
	if(NULL == data || 0 >= unNum)
	{
		return false;
	}

	// ��֤���㹻�Ŀռ�
	if(unNum > i_GetBufSpace())
	{
		return false;
	}

	CAutoLock AutoLock(&m_LockCrit);
	if(unNum <= m_unSize - m_unWPos)
	{
		// ������ĩβ���㹻�ռ�
		memcpy(m_rb + m_unWPos, data, unNum * sizeof(T));
		m_unWPos = (m_unWPos + unNum) % m_unSize;
	}
	else
	{
		// ������ĩβ�ռ䲻���������θ���
		memcpy(m_rb + m_unWPos, data, (m_unSize - m_unWPos) * sizeof(T));
		memcpy(m_rb, data + m_unSize - m_unWPos, (unNum - (m_unSize - m_unWPos)) * sizeof(T));
		m_unWPos = unNum - (m_unSize - m_unWPos);
	}
	return true;
}

// �ӻ�����������
template <class T>
UINT CRingBuf<T>::e_GetData(T* data, UINT unNum, bool bIsTakeAway)
{
	/// ���巵��ֵ
	UINT unRet = 0;
	if(NULL == data || 0 >= unNum)
	{
		return unRet;
	}

	CAutoLock AutoLock(&m_LockCrit);
	/// ��ȡ��������
	unRet = i_GetBufSize();
	if(unNum > unRet)
	{
		return 0;
	}

	/// ��ֵ����
	if(m_unWPos >= m_unRPos || unNum <= m_unSize - m_unRPos)
	{
		// ������������������ĩβ���㹻����
		memcpy(data, m_rb + m_unRPos, unNum * sizeof(T));
		if(bIsTakeAway)
		{
			m_unRPos = (m_unRPos + unNum) % m_unSize;
		}
	}
	else
	{
		// ���ݲ���������ĩβû���㹻���ݣ������θ���
		memcpy(data, m_rb + m_unRPos, (m_unSize - m_unRPos) * sizeof(T));
		memcpy(data + m_unSize - m_unRPos, m_rb, (unNum - (m_unSize - m_unRPos)) * sizeof(T));
		if(bIsTakeAway)
		{
			m_unRPos = unNum - (m_unSize - m_unRPos);
		}
	}
	return unRet;
}

// �鿴����������ȡ��
template <class T>
UINT CRingBuf<T>::e_PeekData(T* data, UINT unNum)
{
	return e_GetData(data, unNum, false);
}

// �ƶ���ȡ��λ��
template <class T>
bool CRingBuf<T>::e_SeekData(UINT unNum)
{
	if(0 >= unNum)
	{
		return false;
	}

	CAutoLock AutoLock(&m_LockCrit);
	/// ��ȡ��������
	UINT unTemp = i_GetBufSize();
	if(unNum > unTemp)
	{
		return false;
	}
	/// �ƶ��ɶ����ݿ�λ��
	if(m_unWPos >= m_unRPos || unNum <= m_unSize - m_unRPos)
	{
		m_unRPos = (m_unRPos + unNum) % m_unSize;
	}
	else
	{
		m_unRPos = unNum - (m_unSize - m_unRPos);
	}
	return true;
}

#endif __COMM_RING_BUFFER_H_H__