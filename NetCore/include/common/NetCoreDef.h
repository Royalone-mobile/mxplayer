///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �������ģ�鶨���ļ�
/// @par ����ļ�
/// ���ô��ļ�ͷ�������ļ�
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetCoreDef.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/12/20
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FWAYNET_NETCORE_NETCORE_DEF_H_H__
#define __FWAYNET_NETCORE_NETCORE_DEF_H_H__

/// ��С�������ݻ������С Sizeof(UINT)
#define MIN_NET_BUFFER_SIZE		4
/// ����������ݻ������С 1024 * 32 (32K)
#define MAX_NET_BUFFER_SIZE		32768
/// ����������ݰ���Ч���ݴ�С MAX_NET_BUFFER_SIZE - MIN_NET_BUFFER_SIZE
#define MAX_PAL_BUFFER_SIZE		32764

/// ���乤������
enum _eJobDataType
{
	JDT_None = 0,					///< �޹�������
	JDT_StringData,					///< �ַ�������
	JDT_StructData,					///< ����/�ṹ������
};

/// TCP��������ͷ�ṹ��
typedef struct _tTCPBufferHead
{
	int				nTotalLen;		///< �����ܳ���(��������ͷ���������ݳ��ȣ������������ֶα���)
	short			sDataType;		///< ��������			�μ���_eJobDataType
	int				nOBJType;		///< ������������		�μ���_eCmdOBJType
	int				nOBJSize;		///< �������ݴ�С
	short			sOBJCount;		///< �������ݸ���
	short			sSNum;			///< ��ʼ����
	short			sENum;			///< ��������
	int				nReserved;		///< Ԥ����չ�ֶ�
}T_TCPBufferHead, *LP_TCPBufferHead;

/// UDP��������ͷ�ṹ��
typedef struct _tUDPBufferHead
{
	short			sDataType;		///< ��������			�μ���_eJobDataType
	int				nOBJType;		///< ������������		�μ���_eCmdOBJType
	int				nOBJSize;		///< �������ݴ�С
	short			sOBJCount;		///< �������ݸ���
	short			sSNum;			///< ��ʼ����
	short			sENum;			///< ��������
	int				nFlag;			///< ��ʶ��(����/�ظ�)
	int				nReserved;		///< Ԥ����չ�ֶ�
}T_UDPBufferHead, *LP_UDPBufferHead;

#endif	/// __FWAYNET_NETCORE_NETCORE_DEF_H_H__