////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �ͻ�������ͨѶҵ����
/// @par ����ļ�
/// NetTreatment.cpp
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetTreatMent.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/12/20
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FWAYNET_NETCLIENT_NETTREATMENT_H__
#define __FWAYNET_NETCLIENT_NETTREATMENT_H__

#include "NetBase.h"

class CNetTreatment: public CNetBase
{
public:
	CNetTreatment();
	~CNetTreatment();

protected:
	/// ==============================================
	/// @par ���� 
	/// �����̴߳���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	virtual void e_SendLoop();

	/// ==============================================
	/// @par ���� 
	/// �����̴߳���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	virtual void e_ReceiveLoop();

private:
	/// ==============================================
	/// @par ���� 
	/// 
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	void i_ProcessTCPRecData();

	/// ==============================================
	/// @par ���� 
	/// 
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	void i_ProcessUDPRecData();

	/// ==============================================
	/// @par ���� 
	/// ������յ��Ľṹ������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		usNetType		���ͺͽ������ݵ��������� �μ�_eNetTransfType
	/// [in]		pszClientIP		�ͻ���IP
	/// [in]		usClientPort	�ͻ��˶˿ں�
	/// [in]		sDataType		��������
	/// [in]		nOBJType		����/�ṹ����
	/// [in]		sOBJCount		����/�ṹ����
	/// [in]		sSNum			����/�ṹ���鿪ʼ����(�����б���ʱ��sOBJCount���ʹ��)
	/// [in]		sENum			����/�ṹ�����������(�����б�������sOBJCount���ʹ��)
	/// [in]		nDatalen		���ݳ���
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	void i_ProcessReceivePackage(USHORT usNetType, const char* pszClientIP, 
		USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount, 
		SHORT sSNum, SHORT sENum, int nDatalen, void* pData);

private:
	/// �������ݻ���
	char  m_szRecvBuffer[MAX_NET_BUFFER_SIZE];
};

#endif /// __FWAYNET_NETCLIENT_NETTREATMENT_H__