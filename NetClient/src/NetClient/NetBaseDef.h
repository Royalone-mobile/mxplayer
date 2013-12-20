////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �ͻ�������ģ��������ݶ���
/// @par ����ļ�
/// ���ô��ļ�ͷ�������ļ�
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetBaseDef.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/12/19
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FWAYNET_NETCLIENTD_NETBASEDEF_H_H__
#define __FWAYNET_NETCLIENTD_NETBASEDEF_H_H__

#include "NetClient/NetClientDef.h"

#define MAXDATALEN

/// ������෢�͵Ļ������鳤��
#define MAXSENDBUFARR		1000			
/// ������������ʱ�������100�ֽ����ݣ�����ͨ��˫���Ľṹ��һ��������ڴ����Խ�磬���Զ������ڴ��ʼ��
#define	EXTRA_BUFFER_SIZE	100
/// ���ӷ�����ʱɨ���IP��ʼ��ַ
#define NETSERVERSTARIP		1
/// ���ӷ�����ʱɨ���IP������ַ
#define NETSERVERSTOPIP		20

/// ���乤������
enum _eJobDataType
{
	JDT_None = 0,					///< �޹�������
	JDT_StringData,					///< �ַ�������
	JDT_StructData,					///< ����/�ṹ������
};

/// ��������ͷ�ṹ��
typedef struct _tBufferHead
{
	int				nTotalLen;		///< �����ܳ���(��������ͷ���������ݳ���)
	short			sDataType;		///< ��������			�μ���_eJobDataType
	int				nOBJType;		///< ������������		�μ���_eCmdOBJType
	int				nOBJSize;		///< �������ݴ�С		
	short			sOBJCount;		///< �������ݸ���
	short			sSNum;			///< ��ʼ����
	short			sENum;			///< ��������
	int				nFlag;			///< ��ʶ��(����/�ظ�)
	int				nReserved;		///< Ԥ����չ�ֶ�
	char			cBuf;			///< ����������(������ݵ�һ���ֽ�)
}T_BufferHead, *LP_BufferHead;

#endif	/// __FWAYNET_NETCLIENTD_NETBASEDEF_H_H__