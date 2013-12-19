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

#ifdef	NETCLIENT_EXPORTS
#define NETCLIENT_API __declspec(dllexport)
#else
#define NETCLIENT_API __declspec(dllimport)
#endif /// LOTTERYNETCLIENT_API_EXPORTS


class NETCLIENT_API CNetClient
{
public:
	CNetClient();

	~CNetClient();
public:

	/// ==============================================
	/// @par ���� 
	/// ���ӷ�����(֧���Զ�����)
	/// @param 
	/// [in,out]	<������>			<����˵��>
	/// [in]		usConnectType		���ӷ�������(0 = ɨ������IP�� 1 = �״�����ָ��IP��2 = ֻ����ָ��IP)
	/// [in]		pszServerIP			�����IP
	/// [in]		usServerPort		����˿ں�
	/// [in]		pfnRecvDataCallBack	�������ݻص��¼�
	/// @return 	int						1 = �ɹ���С�ڵ���0��ʾʧ��
	/// @note 		Creat By li.xl 2013/12/19 
	/// ==============================================
	int e_ConnectionServer(USHORT usConnectType, char* pszServerIP, USHORT usServerPort, 
		OnRecvDataCallBack pfnRecvDataCallBack);

	/// ==============================================
	/// @par ���� 
	/// �Ͽ��ͷ���������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	int				1= �ɹ���С�ڵ���0��ʾʧ��
	/// @note 		Creat By li.xl 2013/12/19 
	/// ==============================================
	int  e_DisconnectServer();

	/// ==============================================
	/// @par ���� 
	/// ���Ͷ���������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		unSSDType		����/�ṹ�Ķ�������
	/// [in]		usOBJSize		����/�ṹ�Ĵ�С
	/// [in]		usOBJCount		����/�ṹ�ĸ���
	/// [in]		pSendData		Ҫ���͵���������
	/// @return 	int				1 = �ɹ���С�ڵ���0��ʾʧ��
	/// @note 		Creat By li.xl 2013/12/19 
	/// ==============================================
	int e_SendBinaryData(UINT unSSDType, USHORT usOBJSize, USHORT usOBJCount, BYTE* pSendData);

	/// ==============================================
	/// @par ���� 
	/// �����ַ�������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		unSSDType		����/�ṹ�Ķ�������
	/// [in]		pszSendData		�ַ�������
	/// [in]		usDatalen		�ַ�������	
	/// @return 	int				1 = �ɹ���С�ڵ���0��ʾʧ��
	/// @note 		Creat By li.xl 2013/12/19 
	/// ==============================================
	int e_SendStringData(UINT unSSDType, char* pszSendData, USHORT usDatalen);

};

#endif	/// __LEE_NETCLIENT_OUTINCLUDE_H__INCLUDE__