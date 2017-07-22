// DataServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "DataServer.h"
#include "DataServerDlg.h"
#include ".\dataserverdlg.h"

#include "..\DataShare.h"
#include "..\Protocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDataServerDlg dialog

#define sDEFAULT_PORT_NUMBER	30000

CDataServerDlg::CDataServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataServerDlg::IDD, pParent)
	, m_nPortNumber(0)
{
	m_nPortNumber = sDEFAULT_PORT_NUMBER;
	m_fStart = false;
	m_pServer = NULL;
	m_nFileNo = 0;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_PORT_NUMBER, m_nPortNumber);
	DDV_MinMaxUInt(pDX, m_nPortNumber, 0, 99999);
	DDX_Control(pDX, IDC_LST_LOG, m_lstLog);
	DDX_Control(pDX, IDC_BTN_START_STOP, m_btnStartStop);
}

BEGIN_MESSAGE_MAP(CDataServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_APP_BSS, OnAppBSS)
	ON_BN_CLICKED(IDC_BTN_START_STOP, OnBnClickedBtnStartStop)
	ON_BN_CLICKED(IDC_BTN_CLEAR_LOG, OnBnClickedBtnClearLog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDataServerDlg message handlers

BOOL CDataServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetWindowText(BSS_DSERVER_TITLE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDataServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDataServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDataServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDataServerDlg::OnBnClickedBtnStartStop()
{
	// TODO: Add your control notification handler code here
	CString strLog;

	m_fStart = !m_fStart;
	if(m_fStart)
	{
		m_btnStartStop.SetWindowText("Stop Data Server");
		m_pServer = new CServerSocket(m_nPortNumber);

		strLog.Format("Start Data Server at Port %d", m_nPortNumber);
		AddLog(strLog);
	}
	else
	{
		m_btnStartStop.SetWindowText("Start Data Server");
		delete m_pServer;
		m_pServer = NULL;

		AddLog("Stop Data Server");
	}
}

void CDataServerDlg::AddLog(CString strLog)
{
	CString s = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d, %I:%M:%S %p"));//t.Format( "%#c" );
	CString strItem;
	int nLast;

	strItem.Format("%s> %s", s, strLog);
	m_lstLog.AddString(strItem);
	nLast = m_lstLog.GetCount()-1;
	if(nLast >= 0)
		m_lstLog.SetCurSel(nLast);

	UpdateData(FALSE);
}

void CDataServerDlg::OnBnClickedBtnClearLog()
{
	// TODO: Add your control notification handler code here
	m_lstLog.ResetContent();
}

LRESULT CDataServerDlg::OnAppBSS(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case WA_DD_SEND_START:
		DoReceiveData();
		break;
	}

	return 0;
}

void CDataServerDlg::DoReceiveData()
{
	CString strLog;

	CServerSocket NewSocket;
	m_pServer->Accept(NewSocket);

	try
	{
		using namespace std;
		string strPathName;
		string strLogName;
		FILE *fp;
		FILE *fpLog;

		CDataChunk data;
		NewSocket >> data;
		data.m_pData;
		string strData("..\\RecvData\\");

		std::cout<<"\n>>> (" << m_nFileNo++ << ") File: "<<data.m_pData<<std::endl;
		strLog.Format(">>> (%d) File: %s", m_nFileNo-1, data.m_pData);
		AddLog(strLog);

		strPathName = strData + data.m_pData;
		strLogName = strPathName + ".log";
		fpLog = fopen(strLogName.c_str(), "wt");
		if(fpLog == NULL)
		{
			std::cout << "Cannot create log file: " << strLogName << std::endl;
			strLog.Format("Cannot create log file:: %s", strLogName);
			AddLog(strLog);
			return;
		}    
		fprintf(fpLog, ">>> (%d) File: %s\n", m_nFileNo-1, data.m_pData);

		fp = fopen(strPathName.c_str(), "wb");
		if(fp == NULL)
		{
			std::cout << "Cannot create file: " << strPathName << std::endl;
			strLog.Format("Cannot create log file:: %s", strPathName);
			AddLog(strLog);
			return;
		}
		std::cout<<"Start to receive the file. " << std::endl;
		AddLog("Start to receive the file");

		fprintf(fpLog, "Start to receive the file.\n");

		int nCnt = 0;
		long lSum = 0;

		while ( true )
		{
			NewSocket >> data;

			lSum += data.m_nSize;

			std::cout<<"Get Data: " << data.m_nSize << ", Total : "<<lSum<<" bytes\r";
			//strLog.Format("Get Data: %d, Total: %d bytes", data.m_nSize, lSum);
			//AddLog(strLog);
			fprintf(fpLog, "Get Data: %d, Total: %d\n", data.m_nSize, lSum);

			if((data.m_nSize == (strlen(BSS_EOF)+1))
			&& !strncmp(data.m_pData, BSS_EOF, data.m_nSize-1))
			{
				std::cout<<"\n<<< Get EOF from client\n";
				AddLog("<<< Get EOF from client");
				fprintf(fpLog, "<<< Get EOF from clinet\n");
				//fflush(stdout);
				break;
			}
			fwrite(data.m_pData, sizeof(char), data.m_nSize, fp);
			fflush(stdout);
		}
		fflush(stdout);
		fclose(fp);
		fclose(fpLog);
	}
	catch ( CSocketException& ) {}
}

void CDataServerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(m_pServer)
		delete m_pServer;

	CDialog::OnClose();
}
