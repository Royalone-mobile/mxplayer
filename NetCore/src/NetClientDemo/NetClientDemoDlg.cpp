
// NetClientDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NetClientDemo.h"
#include "NetClientDemoDlg.h"
#include "afxdialogex.h"
#include "common/CommTypes.h"
#include "common/NetCoreDef.h"

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

	/// ��ʼ�� Windows ����ģ��
	WSADATA wsData;
	if(0 != WSAStartup(MAKEWORD(2, 2), &wsData))
	{
		printf("��ʼ��sockte�����.", GetLastError());
	}

	
	T_InitNetClient sttNetClient;
	strncpy_s(sttNetClient.szTCPServerIP, "127.0.0.1",
		sizeof(sttNetClient.szTCPServerIP) - 1);
	strncpy_s(sttNetClient.szUDPGroupIP, "239.255.0.2", 
		sizeof(sttNetClient.szUDPGroupIP) - 1);
	sttNetClient.usTCPServerPort = 6880;
	sttNetClient.usLocalUDPPort = 15008;
	sttNetClient.usUDPJoinGroup = 1;
	m_CNetClient.e_IInitNetClient(&sttNetClient, i_OnRecvDataCallBack);
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
	ON_BN_CLICKED(IDBTNCONN2, &CNetClientDemoDlg::OnBnClickedBtnSendTCPString)
	ON_BN_CLICKED(IDBTNCONN4, &CNetClientDemoDlg::OnBnClickedBtnSendTCPBary)
	ON_BN_CLICKED(IDBTNCONN3, &CNetClientDemoDlg::OnBnClickedBtnSendUDPString)
	ON_BN_CLICKED(IDBTNCONN5, &CNetClientDemoDlg::OnBnClickedBtnSendUDPBary)
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

void CNetClientDemoDlg::i_OnRecvDataCallBack(USHORT usNetType, const char* pszClientIP, 
	USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount, 
	SHORT sSNum, SHORT sENum, int nDatalen, void* pData)
{
	/// ��֤���ݺϷ���
	if(NULL == pszClientIP || 0 >= usClientPort || 0 >= nDatalen)
	{
		return;
	}
	/// ��ӡ��־
	TRACE("NetType = %d, ClientIP = %s, ClientPort = %d, DataType = %d, OBJType = %d, OBJCount = %d, "\
		"SNum = %d, ENum = %d, Datalen = %d", usNetType, pszClientIP, usClientPort, sDataType, nOBJType,
		sOBJCount, sSNum, sENum, nDatalen);

	if(JDT_StringData == sDataType)
	{
		TRACE("�յ�, String Data ---> %s\r\n", (char* )pData);
	}
	else
	{
		if(COBJT_Test == nOBJType)
		{
			T_TestStruct* pTestStruct = (T_TestStruct* )pData;
			TRACE("COBJT_Test ---> Time = %s, Addr = %s, Name = %s\r\n", pTestStruct->szTime,
				pTestStruct->szAddr, pTestStruct->szName);
		}
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
	m_CNetClient.e_IConnectionServer();
}


void CNetClientDemoDlg::OnBnClickedBtndisconn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_CNetClient.e_IDisconnectServer();
}


void CNetClientDemoDlg::OnBnClickedBtnudpcreat()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_CNetClient.e_ICreatUDPClient();
}


void CNetClientDemoDlg::OnBnClickedBtnudpdestory()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_CNetClient.e_IDestroyUDPClient();
}


void CNetClientDemoDlg::OnBnClickedBtnSendTCPString()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string strSend = "Hello This Is Fway Client Sending TCP String Data.";

	m_CNetClient.e_ISendTCPStringData((char* )strSend.c_str(), strSend.length());
}


void CNetClientDemoDlg::OnBnClickedBtnSendTCPBary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	T_TestStruct sttTestStruct;
	strncpy_s(sttTestStruct.szTime, "2013-12-12 00:00:00", sizeof(sttTestStruct.szTime) - 1);
	strncpy_s(sttTestStruct.szName, "FWAY TCP", sizeof(sttTestStruct.szName) - 1);
	strncpy_s(sttTestStruct.szAddr, "At Home", sizeof(sttTestStruct.szAddr) - 1);

	m_CNetClient.e_ISendTCPBinaryData(COBJT_Test, sizeof(T_TestStruct), 1, (BYTE* )&sttTestStruct);
}


void CNetClientDemoDlg::OnBnClickedBtnSendUDPString()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string strSend = "Hello This Is Fway Client Sending UDP String Data.";
	/// UDP��ַ���͵����鲥��ַ
	m_CNetClient.e_ISendUDPStringData("127.0.0.1", 6880, (char* )strSend.c_str(), strSend.length());
}


void CNetClientDemoDlg::OnBnClickedBtnSendUDPBary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	T_TestStruct sttTestStruct;
	strncpy_s(sttTestStruct.szTime, "2013-12-12 00:00:00", sizeof(sttTestStruct.szTime) - 1);
	strncpy_s(sttTestStruct.szName, "FWAY UDP", sizeof(sttTestStruct.szName) - 1);
	strncpy_s(sttTestStruct.szAddr, "At Home", sizeof(sttTestStruct.szAddr) - 1);

	m_CNetClient.e_ISendUDPBinaryData("239.255.0.1", 6880, COBJT_Test, sizeof(T_TestStruct), 1, (BYTE* )&sttTestStruct);
}
