// SimImageCaptureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimImageCapture.h"
#include "SimImageCaptureDlg.h"

#include "../DataShare.h"
#include ".\simimagecapturedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimImageCaptureDlg dialog

CSimImageCaptureDlg::CSimImageCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimImageCaptureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimImageCaptureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimImageCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimImageCaptureDlg)
	DDX_Control(pDX, IDC_BTN_START_STOP, m_btnStartStop);
	DDX_Control(pDX, IDC_LST_LOG, m_lstLog);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimImageCaptureDlg, CDialog)
	//{{AFX_MSG_MAP(CSimImageCaptureDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_STOP, OnBtnStartStop)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CLEAR_MSG, OnBnClickedBtnClearMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimImageCaptureDlg message handlers

BOOL CSimImageCaptureDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSimImageCaptureDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSimImageCaptureDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSimImageCaptureDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

#define MAKE_TEST_DATA	0
#define COPY_TEST_DATA	1

void CSimImageCaptureDlg::OnBtnStartStop() 
{
	// TODO: Add your control notification handler code here
	CString strLog;
	struct tm *pTime;
	time_t aclock;
	TCHAR szFileName[_MAX_PATH];
	TCHAR szPathName[_MAX_PATH];

	time( &aclock );   // Get time in seconds
	pTime = localtime( &aclock );   // Convert time to struct tm form 

	sprintf(szFileName, "%d-%02d-%02d %02d %02d %02d.tif", 
		1900+pTime->tm_year, pTime->tm_mon+1, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);

	sprintf(szPathName, "..\\Data\\%s", szFileName);

#if MAKE_TEST_DATA
	FILE *fp;
	fp = fopen(szPathName, "wb");

	if(fp != NULL)
	{
		char szData[1024];

		for(int i = 0; i < 15000; i++)
		{
			memset(szData, i, 1024);
			fwrite(szData, sizeof(char), 1024, fp);
			//fwrite(szData, sizeof(char), 5000, fp);
		}
		fclose(fp);

		strLog.Format("Success to create a test data file: %s", szFileName);
		AddLog(strLog);
	}
	else
	{
		strLog.Format("Fail to create a test data file: %s", szFileName);
		AddLog(strLog);
	}
#elif COPY_TEST_DATA
	strLog.Format("Start to create a test data file: %s", szFileName);
	AddLog(strLog);

	if(CopyFile("spiderman.tif", szPathName, true))
	{
		strLog.Format("Success to create a test data file: %s", szFileName);
		AddLog(strLog);
	}
	else
	{
		strLog.Format("Fail to create a test data file: %s", szFileName);
		AddLog(strLog);
	}
#endif

	CWnd* pWnd = FindWindow(NULL, BSS_DDISPATCHER_TITLE);
	if(pWnd)
	{
		// copy data
		COPYDATASTRUCT cpd;
		cpd.dwData = 0;
		cpd.cbData = strlen(szFileName)+1; //strDataToSend.GetLength();
		cpd.lpData = (void*)szFileName; //strDataToSend.GetBuffer(cpd.cbData);
		pWnd->SendMessage(WM_COPYDATA, (WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(), (LPARAM)&cpd);

		//pWnd->PostMessage(WM_APP_BSS, WP_DC_SAVE_DONE);
		pWnd->SendMessage(WM_APP_BSS, WP_DC_SAVE_DONE);

		AddLog("Send Msg to Data Dispatcher");
	}
}

void CSimImageCaptureDlg::AddLog(CString strLog)
{
	CString s = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d, %I:%M:%S %p"));
	CString strItem;
	int nLast;

	strItem.Format("%s> %s", s, strLog);
	m_lstLog.AddString(strItem);
	nLast = m_lstLog.GetCount()-1;
	if(nLast < 0)
		return;
	m_lstLog.SetCurSel(nLast);
}

void CSimImageCaptureDlg::OnBnClickedBtnClearMsg()
{
	// TODO: Add your control notification handler code here
	m_lstLog.ResetContent();
}
