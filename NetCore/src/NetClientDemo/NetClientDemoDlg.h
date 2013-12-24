
// NetClientDemoDlg.h : ͷ�ļ�
//

#pragma once

#include "NetClient/NetClient.h"


// CNetClientDemoDlg �Ի���
class CNetClientDemoDlg : public CDialogEx
{
// ����
public:
	CNetClientDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NETCLIENTDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnconn();
	afx_msg void OnBnClickedBtndisconn();
	afx_msg void OnBnClickedBtnudpcreat();
	afx_msg void OnBnClickedBtnudpdestory();
	afx_msg void OnBnClickedBtnSendTCPString();
	afx_msg void OnBnClickedBtnSendTCPBary();
	afx_msg void OnBnClickedBtnSendUDPString();
	afx_msg void OnBnClickedBtnSendUDPBary();

private:
	/// ==============================================
	/// @par ���� 
	/// ������յ�������
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
	static void i_OnRecvDataCallBack(USHORT usNetType, const char* pszClientIP, 
		USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount, 
		SHORT sSNum, SHORT sENum, int nDatalen, void* pData);

private:
	/// ��ȡ�ͻ�������ģ��
	CNetClient m_CNetClient;
};
