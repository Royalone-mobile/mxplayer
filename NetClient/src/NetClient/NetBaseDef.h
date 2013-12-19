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
	JDT_None = 0,						///< �޹�������
	JDT_TextData,						///< �ı�����
	JDT_StructData,						///< �ṹ������
	JDT_ArrStructData,					///< �ṹ����������
};

/// �������ݽṹ��
typedef struct _tBufferStruct
{
	int					nJobType;		///< ��������
	int					nSST;			///< ������������
	int					nSize;			///< �������ݴ�С
	char*				pBuf;			///< ����������
}T_BufferStruct, *LP_BufferStruct;

/// = ���ڽ��պͷ��͵Ľṹ��Ϣ
/// = ͨ�� nJobDataType��nClientType��nSRStructType���Ȳ�����֤
/// = ͨ�� szData ȡ������
typedef struct _tRSStructData
{
	UINT				nJobDataType;	///< ��������
	UINT				nClientType;	///< �ͻ�������
	UINT				nSRStructType;	///< �������ṹ�崦������
	UINT				nDataSize;		///< ���ݴ�СָszDataָ������������С
	UINT				nDataCount;		///< ������������������szData����Ч�Ľṹ��ĸ���
	UINT				nErrorCode;		///< ������Ϣ����
	char				szData[1];		///< ��������Ч�����׵�ַ������Ϊ���һ���ֶ�
}T_RSStructData, *LP_RSStructData;

#endif	/// __FWAYNET_NETCLIENTD_NETBASEDEF_H_H__