
// NetClientDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NetClientDemo.h"
#include "NetClientDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNetClientDemoDlg �Ի���




CNetClientDemoDlg::CNetClientDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetClientDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetClientDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNetClientDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBTNCONN, &CNetClientDemoDlg::OnBnClickedBtnconn)
	ON_BN_CLICKED(IDBTNDISCONN, &CNetClientDemoDlg::OnBnClickedBtndisconn)
	ON_BN_CLICKED(IDBTNUDPCREAT, &CNetClientDemoDlg::OnBnClickedBtnudpcreat)
	ON_BN_CLICKED(IDBTNUDPDESTORY, &CNetClientDemoDlg::OnBnClickedBtnudpdestory)
	ON_BN_CLICKED(IDBTNCONN2, &CNetClientDemoDlg::OnBnClickedBtnconn2)
	ON_BN_CLICKED(IDBTNCONN4, &CNetClientDemoDlg::OnBnClickedBtnconn4)
	ON_BN_CLICKED(IDBTNCONN3, &CNetClientDemoDlg::OnBnClickedBtnconn3)
	ON_BN_CLICKED(IDBTNCONN5, &CNetClientDemoDlg::OnBnClickedBtnconn5)
END_MESSAGE_MAP()


// CNetClientDemoDlg ��Ϣ�������

BOOL CNetClientDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CNetClientDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNetClientDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CNetClientDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNetClientDemoDlg::OnBnClickedBtnconn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


}


void CNetClientDemoDlg::OnBnClickedBtndisconn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}


void CNetClientDemoDlg::OnBnClickedBtnudpcreat()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CNetClientDemoDlg::OnBnClickedBtnudpdestory()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CNetClientDemoDlg::OnBnClickedBtnconn2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CNetClientDemoDlg::OnBnClickedBtnconn4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CNetClientDemoDlg::OnBnClickedBtnconn3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CNetClientDemoDlg::OnBnClickedBtnconn5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
