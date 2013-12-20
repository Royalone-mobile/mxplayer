////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �ͻ�������ͨѶҵ����
/// @par ����ļ�
/// NetTreatment.cpp
/// @par ������ϸ����
/// <����̨Ӧ�ó������ڵ�>
/// @par ���̰߳�ȫ��
/// <��/��>[��˵��]
/// @par �쳣ʱ��ȫ��
/// <��/��>[��˵��]
/// @note         -
/// 
/// @file         NetTreatment.h
/// @brief        <ģ�鹦�ܼ���>
/// @author       Li.xl
/// @version      1.0
/// @date         2011/05/25
/// @todo         <����Ҫ��������>
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __FWAYNET_LOTTERY_NETTREATMENT_H__
#define __FWAYNET_LOTTERY_NETTREATMENT_H__

#include "NetBase.h"

namespace FWAYNET
{
	////////////////////////////////////////////////////////////////////////////////
	/// 
	/// @par 	�쳣��
	/// <�쳣��˵��>
	/// @par 	��ע
	/// <����ͻ���ҵ��ͨѶ��>
	/// @par 	�����÷�
	/// @code
	/// <�����ǵ����÷���ʾ������>
	/// @endcode
	/// 
	/// @brief
	/// @author	Li.xl
	/// 
	////////////////////////////////////////////////////////////////////////////////
	class CNetTreatment: public CNetBase
	{
	public:
		/// 
		/// @par ���� 
		/// ���캯��
		/// @param 
		///	[in,out]	<������>		<����˵��>
		/// 
		/// @return 		-
		/// @note 
		/// <����˵��>
		/// @par ʾ����
		/// @code 
		/// <���ñ�������ʾ������>
		/// @endcode 
		/// @see		<�μ�����> 
		/// @deprecated	<��ʱ˵��> 
		/// 
		CNetTreatment();

		/// 
		/// @par ���� 
		/// ��������
		/// @param 
		///	[in,out]	<������>		<����˵��>
		/// 
		/// @return 		-
		/// @note 
		/// <����˵��>
		/// @par ʾ����
		/// @code 
		/// <���ñ�������ʾ������>
		/// @endcode 
		/// @see		<�μ�����> 
		/// @deprecated	<��ʱ˵��> 
		/// 
		~CNetTreatment();

		/// 
		/// @par ���� 
		/// �����̴߳���
		/// @param 
		///	[in,out]	<������>		<����˵��>
		/// 
		/// @return 		-
		/// @note 
		/// <����˵��>
		/// @par ʾ����
		/// @code 
		/// <���ñ�������ʾ������>
		/// @endcode 
		/// @see		<�μ�����> 
		/// @deprecated	<��ʱ˵��> 
		/// 
		void e_SendLoop();

		/// 
		/// @par ���� 
		/// �����̴߳���
		/// @param 
		///	[in,out]	<������>		<����˵��>
		/// 
		/// @return 		-
		/// @note 
		/// <����˵��>
		/// @par ʾ����
		/// @code 
		/// <���ñ�������ʾ������>
		/// @endcode 
		/// @see		<�μ�����> 
		/// @deprecated	<��ʱ˵��> 
		/// 
		void e_ReceiveLoop();

		/// 
		/// @par ���� 
		/// ������յ�������
		/// @param 
		///	[in,out]	<������>		<����˵��>
		/// [in]		BtDataType		������������
		/// [in]		pData			���͵�����
		/// @return 		-
		/// @note 
		/// <����˵��>
		/// @par ʾ����
		/// @code 
		/// <���ñ�������ʾ������>
		/// @endcode 
		/// @see		<�μ�����> 
		/// @deprecated	<��ʱ˵��> 
		/// 
		virtual void e_ProcessReceiveData(BYTE BtDataType, void* pData);

		/// 
		/// @par ���� 
		/// ������յ��Ľṹ������
		/// @param 
		///	[in,out]		<������>		<����˵��>
		/// [in]			nSendStructType	���͹����Ľṹ������
		/// [in]			pData			���͵�����
		/// @return 		-
		/// @note 
		/// <����˵��>
		/// @par ʾ����
		/// @code 
		/// <���ñ�������ʾ������>
		/// @endcode 
		/// @see		<�μ�����> 
		/// @deprecated	<��ʱ˵��> 
		/// 
		void e_PackageReceiveStruct(UINT nSendStructType, void* pData);

		/// 
		/// @par ���� 
		/// ��ȡ��ǰ����Ŀ¼·��
		/// @note 		-
		/// @par ʾ����
		/// @code 		-
		/// @endcode 
		/// @see		-
		/// @deprecated	- 
		/// 
		string e_GetFilePath();

		/// 
		/// @par ���� 
		/// ��ȡ�����ļ���ָ���������ط����IP
		/// @param[in/out]	pszServiceIp;	���ز���,Ҫ��ȡ�ķ�����IP
		/// @param[in]		nSizeIp;		���ز����ַ�������
		/// @return 	bool	��ȡ�ɹ�����ʧ��
		/// @note 		-
		/// @par ʾ����
		/// @code 		-
		/// @endcode 
		/// @see		-
		/// @deprecated	- 
		/// 
		bool e_GetNetServiceIp(char* pszServiceIp, int nSizeIp);
	public:
		/// ���ݷ���ʱ��
		time_t		m_tSendTime;
	};
}

#endif /// __FWAYNET_LOTTERY_NETTREATMENT_H__