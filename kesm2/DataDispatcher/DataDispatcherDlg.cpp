// DataDispatcherDlg.cpp : implementation file
//

#include "stdafx.h"
#include <process.h>
#include <io.h>
#include "DataDispatcher.h"
#include "DataDispatcherDlg.h"

#include "..\DataShare.h"

#include "..\SocketLib\SocketJr.h"
#include "..\SocketLib\ClientSocket.h"
#include "..\SocketLib\SocketException.h"
#include "..\Protocol.h"
#include ".\datadispatcherdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define sUSE_LOCAL_HOST

//#define USE_CRITICAL_SECTION

// Global variable
#ifdef USE_CRITICAL_SECTION
CRITICAL_SECTION CriticalSection; 
#endif

#define PERCENT_COL		3

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
// CDataDispatcherDlg dialog

CDataDispatcherDlg::CDataDispatcherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataDispatcherDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataDispatcherDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_fStart = FALSE;
	m_hThread = NULL;
//	m_fQuit = false;
}

CDataDispatcherDlg::~CDataDispatcherDlg()
{
	if(m_hThread != NULL)
	{
		WaitForSingleObject( m_hThread, INFINITE );
		// Destroy the thread object.
		CloseHandle( m_hThread );
	}

	// Release resources used by the critical section object.
#ifdef USE_CRITICAL_SECTION
    DeleteCriticalSection(&CriticalSection);
#endif
}

void CDataDispatcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataDispatcherDlg)
	DDX_Control(pDX, IDC_LST_LOG, m_lstLog);
	DDX_Control(pDX, IDC_LST_FILES, m_lstFiles);
	//}}AFX_DATA_MAP
}

//#define WM_APP_UPDATE_PROGRESS	WM_APP_BSS+1

BEGIN_MESSAGE_MAP(CDataDispatcherDlg, CDialog)
	//{{AFX_MSG_MAP(CDataDispatcherDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_COPYDATA()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_APP_BSS, OnAppBSS)
	ON_BN_CLICKED(IDC_BTN_CLEAR_MSG, OnBnClickedBtnClearMsg)
	ON_BN_CLICKED(IDC_BTN_CLEAR_LIST, OnBnClickedBtnClearList)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

#define TIMER_CHK_NEW   1

/////////////////////////////////////////////////////////////////////////////
// CDataDispatcherDlg message handlers

BOOL CDataDispatcherDlg::OnInitDialog()
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
	// XListCtrl must have LVS_EX_FULLROWSELECT if combo boxes are used
	m_lstFiles.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_TRACKSELECT);

	InitListCtrl();

	// test -------------
	//AddItem("2005-09-08 090909.TIF"/* , TIME*/);
	//m_lstFiles.SetProgress(0, 3);
	//m_lstFiles.UpdateProgress(0, 3, 70);
	//m_lstFiles.DeleteProgress(0, 3);

	//AddItem("2005-09-08 090909.TIF"/* , TIME*/);
	// -------------- test

	SetWindowText(BSS_DDISPATCHER_TITLE);
    CWnd *pWnd = FindWindow(NULL, BSS_STAGECTLR_TITLE);
	if(pWnd)
	{
		pWnd->PostMessage(WM_APP_BSS, WP_DD_LAUNCH_DONE);
	}

	// Start timer for monitoring items
	SetTimer(TIMER_CHK_NEW, 1000, 0);

	// Set clusters
#ifdef sUSE_LOCAL_HOST
	m_Cluster.Add(CCluster::CLUSTER_0_E, "127.0.0.1",  30000);
#else
	m_Cluster.Add(CCluster::CLUSTER_0_E, "128.194.146.74",  30001);
	m_Cluster.Add(CCluster::CLUSTER_1_E, "128.194.141.133", 30001);
#endif

    // Initialize the critical section one time only.
#ifdef USE_CRITICAL_SECTION
    if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x80000400) ) 
        return FALSE;
#endif
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDataDispatcherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDataDispatcherDlg::OnPaint() 
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
HCURSOR CDataDispatcherDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

#define sITEM_NUM	4;
//
void CDataDispatcherDlg::InitListCtrl()
{
	// set column width according to window rect
	CRect Rect;
	m_lstFiles.GetWindowRect(&Rect);

	int nWidth = Rect.Width() - 2;
	int nColWidths[] = { 10, 20, 20, 10 };

	TCHAR *	lpszHeaders[] = { _T("#"),
							  _T("File Name"),
							  _T("Date/Time"),
							  _T("Pct"),
							  NULL };
	int nTotalCx = 0;
	LV_COLUMN lvColumn;
	memset(&lvColumn, 0, sizeof(lvColumn));

	// add columns
	for (int i = 0; lpszHeaders[i]; i++)
	{
		lvColumn.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.pszText = lpszHeaders[i];
		lvColumn.iSubItem = i;
		lvColumn.cx = (lpszHeaders[i+1] == NULL) ? nWidth - nTotalCx - 2 : (nWidth * nColWidths[i]) / 64;
		nTotalCx += lvColumn.cx;
		m_lstFiles.InsertColumn(i, &lvColumn);
	}
}

void CDataDispatcherDlg::AddItem(CString strFileName/*, CTime Time*/)
{
	LVITEM lvi;
	CString strItem;
    int nItemNo;

	nItemNo = m_lstFiles.GetItemCount();
	
	// Insert the first item
	lvi.mask =  /*LVIF_IMAGE | */LVIF_TEXT;
	strItem.Format(_T("%d"), nItemNo);
	lvi.iItem = nItemNo;
	lvi.iSubItem = 0;
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_lstFiles.InsertItem(&lvi);

	// Set subitem 1
	strItem = strFileName;
	lvi.iSubItem =1;
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_lstFiles.SetItem(&lvi);

	// Set subitem 2
	strItem.Format(_T("%s"),
				   COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d, %I:%M:%S %p")));
	lvi.iSubItem =2;
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_lstFiles.SetItem(&lvi);

	m_lstFiles.SetProgress(nItemNo, PERCENT_COL);
	m_lstFiles.UpdateProgress(nItemNo, PERCENT_COL, 0);
	m_lstFiles.SetItemData(nItemNo, 0); // progress 
}

void CDataDispatcherDlg::AddLog(CString strLog)
{
	//CTime t = CTime::GetCurrentTime();
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

//void CDataDispatcherDlg::OnBtnStartStop() 
//{
//	// TODO: Add your control notification handler code here
//	m_fStart = !m_fStart;
//	if(m_fStart)
//	{
//		m_btnStartStop.SetWindowText("Stop");
//		//pWnd->PostMessage(WM_APP_BSS, WP_START_CAPTURE); 
//		//AddLog("PostMessage to DC : WP_START_CAPTURE");
//	}
//	else // stop
//	{
//		m_btnStartStop.SetWindowText("Start");
//		//pWnd->PostMessage(WM_APP_BSS, WP_STOP_CAPTURE);
//		//AddLog("PostMessage to DC : WP_STOP_CAPTURE");
//	}
//	
//}

LRESULT CDataDispatcherDlg::OnAppBSS(WPARAM wParam, LPARAM lParam)
{
	CString strLog;

	switch(wParam)
	{
	case WP_DC_SAVE_DONE:
		strLog = "Msg from DC : Saved " + m_strFileName;
		AddLog(strLog);
		
		AddItem(m_strFileName);

		//Sleep(1000);

		//strLog = "Start to send " + m_strFileName;
		//AddLog(strLog);

		break;
	}

	return 0;
}

//LRESULT CDataDispatcherDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
//{
//	CString strLog;
//	//static int nOldPercent = -1; // init disp
//	int nPercent = (int)((double)wParam/lParam*100.0);
//
//	m_lstFiles.SetItemData(m_nSendingItemNo, nPercent);
//	//if(nPercent != nOldPercent)
//		m_lstFiles.UpdateProgress(m_nSendingItemNo, PERCENT_COL, nPercent);
//	//nOldPercent = nPercent;
//	
//	return 0;
//}

BOOL CDataDispatcherDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	m_strFileName = (LPCSTR) (pCopyDataStruct->lpData);
	
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CDataDispatcherDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int nItem;

	if(m_fSending == true)
		return;

	nItem = m_lstFiles.GetItemCount();
	for(int i = 0; i < nItem; i++)
	{
		if(m_lstFiles.GetItemData(i) == 0) /* added but not started yet */
		{
			m_strSendingFileName = m_lstFiles.GetItemText(i, 1);
			m_nSendingItemNo = i;

			SendDataFile();
			
			m_lstFiles.SetItemData(i, 100);
			//UpdateData();
			break;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CDataDispatcherDlg::SendDataFile()
{
    DWORD dwThreadId; 
    //HANDLE hThread; 

	// copy file name to send
	//m_strSendingFileName = strFileName;

    m_hThread = (HANDLE)CreateThread( 
        NULL,                        // default security attributes 
        0,                           // use default stack size  
        SendDataThread,                  // thread function 
        this,		                 // argument to thread function 
        0,                           // use default creation flags 
        &dwThreadId);                // returns the thread identifier 
 
   // Check the return value for success. 
 
   if (m_hThread == NULL) 
   {
      AddLog("CreateThread failed");
	  return;
   }
   m_fSending = true;
}

// UpdateData --> can't be used
DWORD CDataDispatcherDlg::DoSendData()
{
	CString strFileName = m_strSendingFileName;
	CDataChunk DataChunk;
	DataChunk.m_pData = new char [MAX_DATA_CHUNK_SIZE];
	CString strLog;

	try
	{
		CClusterID ClusterID;
		
		m_Cluster.Get(CCluster::CLUSTER_0_E, ClusterID);

		// Brainscan : 128.194.146.74
		CClientSocket ClientSocket ( ClusterID.m_strIP.c_str(), ClusterID.m_nPort);
		try
		{
			// send file name info
			strcpy(DataChunk.m_pData, strFileName);
			DataChunk.m_nSize = strFileName.GetLength()+1;

			ClientSocket << DataChunk;
			//ClientSocket >> reply;

			FILE* fp;
			CString strPathName;

			strPathName = "..\\Data\\" + strFileName;

			fp = fopen(strPathName, "rb");
			if(fp == NULL)
			{
				CString strLog;
				strLog.Format("Read error from file:  %s", strFileName);
				//AddLog(strLog);
				throw strLog;
			}
			long lFileLen = _filelength(_fileno(fp));
			long lSum = 0;
			static int nOldPercent = -1;
			int nPercent;

			while(!feof(fp))// && !m_fQuit)
			{
				DataChunk.m_nSize = fread(DataChunk.m_pData, sizeof(char), MAX_DATA_CHUNK_SIZE, fp);
				// Request ownership of the critical section.
#ifdef USE_CRITICAL_SECTION
				EnterCriticalSection(&CriticalSection); 
#endif
				//if(DataChunk.m_nSize != MAX_DATA_CHUNK_SIZE)
				//{
				//	MessageBox("Wooooop!!!");
				//}
				lSum += DataChunk.m_nSize;
				
				nPercent = (int)((double)lSum/lFileLen*100.0);
				if(nOldPercent != nPercent)// && !m_fQuit)
					//PostMessage(WM_APP_UPDATE_PROGRESS, lSum, lFileLen);
					m_lstFiles.UpdateProgress(m_nSendingItemNo, PERCENT_COL, nPercent);
				nOldPercent = nPercent;

				//m_lstFiles.SetItemData(m_nSendingItemNo, nPercent);
				//UpdateData();

				// Release ownership of the critical section.
#ifdef USE_CRITICAL_SECTION
				LeaveCriticalSection(&CriticalSection);
#endif
				ClientSocket << DataChunk;
				//Sleep(1);
				//ClientSocket >> Reply;
				//if(!strcmp(Reply.m_pData, "ACK"))
				//	continue;
				//else
				//	AddLog("No ACK");
			}
			Sleep(500);

			// send EOF
			strcpy(DataChunk.m_pData, BSS_EOF);
			DataChunk.m_nSize = (strlen(BSS_EOF)+1);
			ClientSocket << DataChunk;
		}
		catch ( CSocketException& e ) 
		{
			CString strLog;
			strLog.Format("Exception was caught while sending data: %s", e.description());
			MessageBox(strLog);
			//AddLog(strLog);
		}
		catch ( char *str)
		{
			MessageBox(str);
		}
		//std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;
	}
	catch ( CSocketException& e )
	{
		CString strLog;
		strLog.Format("Exception was caught: %s", e.description());
		MessageBox(strLog);
		//AddLog(strLog);
	}
	delete DataChunk.m_pData;

	m_fSending = false;
	return 0;
}

DWORD WINAPI CDataDispatcherDlg::SendDataThread(LPVOID lpArg)
{
	// for local server test
    CWnd *pWnd = FindWindow(NULL, BSS_DSERVER_TITLE);
	if(pWnd)
	{
		pWnd->PostMessage(WM_APP_BSS, WA_DD_SEND_START);
	}

	CDataDispatcherDlg *pDlg = (CDataDispatcherDlg *)lpArg;
	return pDlg->DoSendData();
}

void CDataDispatcherDlg::OnBnClickedBtnClearMsg()
{
	// TODO: Add your control notification handler code here
	m_lstLog.ResetContent();
}

void CDataDispatcherDlg::OnBnClickedBtnClearList()
{
	// TODO: Add your control notification handler code here
	m_lstFiles.DeleteAllItems();
}

void CDataDispatcherDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(TIMER_CHK_NEW);
	TerminateThread(m_hThread, 0);
//	m_fQuit = true;
	
	CDialog::OnClose();
}
