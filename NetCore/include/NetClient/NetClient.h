////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// ����ӿں�������
/// @par ����ļ�
/// NetClient.cpp
/// @par ������ϸ����
/// <�����Ǳ����ܵ���ϸ���������ݳ���120������ַ���Ҫ�ӷ�б��N���н���һ������>
/// @par ���̰߳�ȫ��
/// <��/��>[��˵��]
/// @par �쳣ʱ��ȫ��
/// <��/��>[��˵��]
/// @note         -
/// 
/// @file         NetClient.h
/// @brief        <ģ�鹦�ܼ���>
/// @author       Li.xl
/// @version      1.0
/// @date         2011/06/08
/// @todo         <����Ҫ��������>
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __LEE_NETCLIENT_OUTINCLUDE_H__INCLUDE__
#define __LEE_NETCLIENT_OUTINCLUDE_H__INCLUDE__

#include "NetClient/NetClientDef.h"
#include "NetBusinessDef.h"

#ifdef	NETCLIENT_EXPORTS
#define NETCLIENT_API __declspec(dllexport)
#else
#define NETCLIENT_API __declspec(dllimport)
#endif /// NETCLIENT_EXPORTS

class NETCLIENT_API CNetClient
{
public:
	CNetClient();
	~CNetClient();
public:
	/// ==============================================
	/// @par ���� 
	/// ��ʼ���ͻ�������ģ��
	/// @param 
	/// [in,out]	<������>			<����˵��>
	///	[in]		psttInitNetClient	��ʼ���ṹ�����ָ��
	/// [in]		pfnRecvDataCallBack	�������ݻص�����
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	int e_IInitNetClient(T_InitNetClient* psttInitNetClient,
		OnRecvDataCallBack pfnRecvDataCallBack);

	/// ==============================================
	/// @par ���� 
	/// ���ӷ�����(֧���Զ�����)
	/// @param 
	/// [in,out]	<������>			<����˵��>
	/// -
	/// @return 	int		1 = �ɹ���С�ڵ���0��ʾʧ��
	/// @note 		Creat By li.xl 2013/12/19 
	/// ==============================================
	int e_IConnectionServer();

	/// ==============================================
	/// @par ���� 
	/// �Ͽ��ͷ���������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	int		1= �ɹ���С�ڵ���0��ʾʧ��
	/// @note 		Creat By li.xl 2013/12/19 
	/// ==============================================
	int e_IDisconnectServer();

	/// ==============================================
	/// @par ���� 
	/// ����UDP�ͻ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	int		1= �ɹ���С�ڵ���0��ʾʧ��
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	int e_ICreatUDPClient();

	/// ==============================================
	/// @par ���� 
	/// ����UDP�ͻ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	int		1= �ɹ���С�ڵ���0��ʾʧ��
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	int e_IDestroyUDPClient();

	/// ==============================================
	/// @par ���� 
	/// ����TCP�ַ�������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pszSendData		�ַ�������
	/// [in]		unDatalen		�ַ�������	
	/// @return 	int		1 = �ɹ���С�ڵ���0��ʾʧ��
	/// @note 		Creat By li.xl 2013/12/19 
	/// ==============================================
	int e_ISendTCPStringData(char* pszSendData, UINT unDatalen);

	/// ==============================================
	/// @par ���� 
	/// ���Ͷ��������ݶ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		unSSDType		����/�ṹ�Ķ�������
	/// [in]		unOBJSize		����/�ṹ�Ĵ�С
	/// [in]		usOBJCount		����/�ṹ�ĸ���
	/// [in]		pSendData		Ҫ���͵���������
	/// @return 	int		1 = �ɹ���С�ڵ���0��ʾʧ��
	/// @note 		Creat By li.xl 2013/12/19 
	/// ==============================================
	int e_ISendTCPBinaryData(UINT unSSDType, UINT unOBJSize,
		USHORT usOBJCount, BYTE* pSendData);

	/// ==============================================
	/// @par ���� 
	/// ����UDP�ַ�������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pszSendData		�ַ�������
	/// [in]		unDatalen		�ַ�������	
	/// @return 	int		1 = �ɹ���С�ڵ���0��ʾʧ��
	/// @note 		Creat By li.xl 2013/12/19 
	/// ==============================================
	int e_ISendUDPStringData(const char* pRemoteIP, USHORT usRemotePort, 
		char* pszSendData, UINT unDatalen);

	/// ==============================================
	/// @par ���� 
	/// ����UDP���������ݶ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		unSSDType		����/�ṹ�Ķ�������
	/// [in]		unOBJSize		����/�ṹ�Ĵ�С
	/// [in]		usOBJCount		����/�ṹ�ĸ���
	/// [in]		pSendData		Ҫ���͵���������
	/// @return 	int		1 = �ɹ���С�ڵ���0��ʾʧ��
	/// @note 		Creat By li.xl 2013/12/19 
	/// ==============================================
	int e_ISendUDPBinaryData(const char* pRemoteIP, USHORT usRemotePort, UINT unSSDType,
		UINT unOBJSize, USHORT usOBJCount, BYTE* pSendData);

private:
	/// �ͻ������紦�����
	void*  m_pProNetClient;
};

#endif	/// __LEE_NETCLIENT_OUTINCLUDE_H__INCLUDE__