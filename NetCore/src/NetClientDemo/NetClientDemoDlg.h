
// NetClientDemoDlg.h : ͷ�ļ�
//

#pragma once


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
	afx_msg void OnBnClickedBtnconn2();
	afx_msg void OnBnClickedBtnconn4();
	afx_msg void OnBnClickedBtnconn3();
	afx_msg void OnBnClickedBtnconn5();
};
