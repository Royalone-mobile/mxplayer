/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �������ģ��
/// @par ����ļ�
/// ���ò��ļ�ͷ�������ļ�
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetCore.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/06/13
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __LEE_NET_CORE_H_H__
#define __LEE_NET_CORE_H_H__

#include "NetServerDef.h"

#ifdef NETSERVER_EXPORTS
	#define NETSERVER_API __declspec(dllexport)
#else
	#define NETSERVER_API __declspec(dllimport)
#endif /// NETSERVER_EXPORTS

class NETSERVER_API CNetServer
{
public:
	CNetServer();
	virtual ~CNetServer();

	/// ==============================================
	/// @par ���� 
	/// ��������
	/// @param 
	/// [in,out]	<������>			<����˵��>
	///	[in]		psttNetInitStruct	�����ʼ������
	/// [in]		pFunRecvData		�������ݻص�����
	/// [in]		pFunSendData		�������ݻص�����
	/// [in]		pFunConection		�ͻ���TCP�ɹ����ӻص�����
	/// [in]		pFunDisConection	�ͻ���TCP�Ͽ����ӻص�����
	/// @return 	-
	/// @note 		by li.xl 2013/06/22 
	/// ==============================================
	bool e_IStartServer(T_NetInitStruct* psttNetInitStruct,
		OnRecvDataCallBack pFunRecvData = NULL,
		OnSendDataCallBack pFunSendData = NULL, 
		OnConectionCallBack pFunConection = NULL, 
		OnDisConectionCallBack pFunDisConection = NULL,
		OnPrintLog pFunPrintLog = NULL);

	/// ==============================================
	/// @par ���� 
	/// ֹͣ����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// @return 	bool			����ֹͣ�ɹ�����ʧ��
	/// @note 		by li.xl 2013/06/22 
	/// ==============================================
	bool e_IStopdServer();

	/// ==============================================
	/// @par ���� 
	/// TCP��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		ulContextID		������ϢID
	/// [in]		pSendData		���͵�����
	/// [in]		nDataLen		���ݳ���
	/// @return 	-
	/// @note 		by li.xl 2013/06/22 
	/// ==============================================
	bool e_ITCPSendData(ULONG ulContextID, BYTE* pSendData, int nDataLen);

	/// ==============================================
	/// @par ���� 
	/// UDP��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pszClientIP		�ͻ���IP
	///	[in]		usClientPort	�ͻ��˶˿ں�
	/// [in]		pSendData		���͵�����
	/// [in]		nDataLen		���ݳ���
	/// @return 	-
	/// @note 		by li.xl 2013/06/22 
	/// ==============================================
	bool e_IUDPSendData(const char* pszClientIP, USHORT usClientPort, BYTE* pSendData, int nDataLen);

	/// ==============================================
	/// @par ���� 
	/// �ر�TCP����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		ulContextID		������ϢID
	/// @return 	-
	/// @note 		by li.xl 2013/06/28 
	/// ==============================================
	bool e_ICloseTCPContext(ULONG ulContextID);

private:
	/// ��������������ָ��
	void* m_pCNetProcess;
};

#endif	/// __LEE_NET_CORE_H_H__