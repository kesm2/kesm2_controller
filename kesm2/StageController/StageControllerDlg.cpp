// StageControllerDlg.cpp : implementation file
//
//#define FEATURE_CHECK_CAPTURED_IMAGE

#include "stdafx.h"
#include <math.h>
#include "StageController.h"
#include "StageControllerDlg.h"

#include "..\DataShare.h"
#include ".\stagecontrollerdlg.h"
#include "SessionDescDlg.h"
#include "SettingsDlg.h"
#include <fstream>
#include <direct.h>
#include "shlobj.h"
#include <string>

#include "tiffio.h" // LibTiff;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogSK
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

CAboutDlg::CAboutDlg() : CDialogSK(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogSK)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStageControllerDlg dialog

CStageControllerDlg::CStageControllerDlg(CWnd* pParent /*=NULL*/)
	: CDialogSK(CStageControllerDlg::IDD, pParent)
	, m_strCommand(_T(""))
	, m_fSaveImages(FALSE)
	, m_strXPos(_T(""))
	, m_strYPos(_T(""))
	, m_strZPos(_T(""))
	, m_strXPosFB(_T(""))
	, m_strYPosFB(_T(""))
	, m_strZPosFB(_T(""))
	, m_strSessionDesc(_T(""))
	, m_strSessionTitle(_T(""))
	, m_nColNum(0)
	, m_nSliceNum(0)
	, m_nCurCol(0)
	, m_dCurZ(0)
{
	//{{AFX_DATA_INIT(CStageControllerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_fStart = FALSE;
	m_pMotionController = new CMotionController(this);
	m_fSetDlgOpen = false;

	m_bStairStepStart = true;
	m_bStairStepPause = true;

	m_fFirstClickStepByStep = false;
}

CStageControllerDlg::~CStageControllerDlg()
{
	delete m_pMotionController;
}

void CStageControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStageControllerDlg)
	DDX_Control(pDX, IDC_LST_LOG, m_lstLog);
	DDX_Control(pDX, IDC_BTN_START_STOP, m_btnStartStop);
	//}}AFX_DATA_MAP
	//DDX_Text(pDX, IDC_EDIT_COMMAND, m_strCommand); // temporarily deleted
	DDX_Check(pDX, IDC_CHECK_SAVEIMAGES, m_fSaveImages);
	DDX_Text(pDX, IDC_EDIT_XPOS, m_strXPos);
	DDX_Text(pDX, IDC_EDIT_YPOS, m_strYPos);
	DDX_Text(pDX, IDC_EDIT_ZPOS, m_strZPos);
	DDX_Text(pDX, IDC_EDIT_XPOS2, m_strXPosFB);
	DDX_Text(pDX, IDC_EDIT_YPOS2, m_strYPosFB);
	DDX_Text(pDX, IDC_EDIT_ZPOS2, m_strZPosFB);
	DDX_Control(pDX, IDC_LEFT_SLICES, m_sttLeftSlices);
	DDX_Text(pDX, IDC_STATIC_SESSION_DESC, m_strSessionDesc);
	DDX_Text(pDX, IDC_STATIC_SESSION_TITLE, m_strSessionTitle);
	DDX_Text(pDX, IDC_EDIT_SS_CUR_COL, m_nCurCol);
	DDX_Text(pDX, IDC_EDIT_SS_CUR_Z, m_dCurZ);
	DDX_Control(pDX, IDC_BTN_SS_START_STOP, m_btnStartStopSS);
	DDX_Control(pDX, IDC_BTN_SS_PAUSE_RESUME, m_btnPauseResumeSS);
}

BEGIN_MESSAGE_MAP(CStageControllerDlg, CDialogSK)
	//{{AFX_MSG_MAP(CStageControllerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LAUNCH, OnBtnLaunch)
	//ON_BN_CLICKED(IDC_BTN_START_STOP, OnBtnStartStop)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_APP_BSS, OnAppBSS)
    ON_BN_CLICKED(IDC_BTN_SETTINGS, OnBnClickedBtnSettings)
	ON_BN_CLICKED(IDC_BTN_SEND_COMMAND, OnBnClickedBtnSendCommand)
	ON_BN_CLICKED(IDC_BTN_INIT_STAGE, OnBnClickedBtnInitStage)
	ON_BN_CLICKED(IDC_BTN_RESET_STAGE, OnBnClickedBtnResetStage)
	ON_BN_CLICKED(IDC_BTN_ENABLE_STAGE, OnBnClickedBtnEnableStage)
	ON_BN_CLICKED(IDC_BTN_DISABLE_STAGE, OnBnClickedBtnDisableStage)
	ON_BN_CLICKED(IDC_BTN_CUT_A_SLICE, OnBnClickedBtnCutASlice)
	ON_BN_CLICKED(IDC_BTN_CUT_A_PLANK, OnBnClickedBtnCutAPlank)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_PLANK_SESSION, OnBnClickedBtnPlankSession)
	ON_BN_CLICKED(IDC_BTN_TEST_CAPTURE_SEND, OnBnClickedBtnTestCaptureSend)
	ON_BN_CLICKED(IDC_CHECK_SAVEIMAGES, OnBnClickedCheckSaveimages)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_LAUNCH_IMAGE_CAPTURE, OnBnClickedBtnLaunchImageCapture)
//	ON_WM_DESTROY()
ON_BN_CLICKED(IDC_BUTTON_NEW_SESSION, OnBnClickedButtonNewSession)
ON_WM_CLOSE()
ON_BN_CLICKED(IDC_BUTTON_OPEN_SESSION, OnBnClickedButtonOpenSession)
ON_BN_CLICKED(IDC_BTN_SS_START_STOP, OnBnClickedBtnSsStartStop)
ON_BN_CLICKED(IDC_BTN_SS_PAUSE_RESUME, OnBnClickedBtnSsPauseResume)
ON_BN_CLICKED(IDC_BTN_SS_INIT, OnBnClickedBtnSsInit)
ON_BN_CLICKED(IDC_BTN_SS_STEP_BY_STEP, OnBnClickedBtnSsStepByStep)
ON_BN_CLICKED(IDC_BTN_SS_GO, OnBnClickedBtnSsGo)
ON_BN_CLICKED(IDC_BTN_SS_SET_START, OnBnClickedBtnSsSetStart)
ON_BN_CLICKED(IDC_BTN_SS_STOP, OnBnClickedBtnSsStop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStageControllerDlg message handlers

BOOL CStageControllerDlg::OnInitDialog()
{
	CDialogSK::OnInitDialog();

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
	EnableEasyMove();  // enable moving of the dialog 
                       // by clicking anywhere in the dialog
    //SetBitmap (IDB_BKGRND); // set background bitmap
    //SetStyle (LO_RESIZE); // resize dialog to the size of the bitmap
    //SetTransparentColor(RGB(0, 255, 0)); // set green as the 
    //                                     // transparent color

	SetWindowText(BSS_STAGECTLR_TITLE);
    LoadParameters(szFILE4PARAMS);

	// automatic initialization
	//m_pMotionController->Initialize();
	//AddLog("Initializing...");

	m_sttLeftSlices.Format("%04d", 0);


	
	SetTimer(1,100,NULL);


	OnBnClickedBtnLaunchImageCapture();
	EnableAllItems(false);

	// jw, hwang, 02-29-2016
	pDCapture = NULL;

	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CStageControllerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogSK::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStageControllerDlg::OnPaint() 
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
		CDialogSK::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStageControllerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CStageControllerDlg::OnBtnLaunch() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, "open", BSS_PATH_DATA_DISPATCHER, "", "", SW_SHOW);
	AddLog("Launching Data Dispatcher...");
	ShellExecute(this->m_hWnd, "open", BSS_PATH_IMAGE_CAPTURE, "", "", SW_SHOW);
	AddLog("Launching Data Capture...");
}

void CStageControllerDlg::AddLog(CString strLog)
{
	//CTime t = CTime::GetCurrentTime();
	CString s = COleDateTime::GetCurrentTime().Format(_T("%I:%M:%S %p"));
	//CString s = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d, %I:%M:%S %p"));
	CString strItem;
	int nLast;

	strItem.Format("%s> %s", s, strLog);
	m_lstLog.AddString(strItem);
	nLast = m_lstLog.GetCount()-1;
	if(nLast >= 0)
		m_lstLog.SetCurSel(nLast);

	UpdateData(FALSE);
}

void CStageControllerDlg::OnBtnStartStop() 
{
	// TODO: Add your control notification handler code here
	// Get Data Capture Window Handle
	CWnd *pWnd = FindWindow(NULL, BSS_DCAPTURE_TITLE);
	if(pWnd == NULL)
	{
		AddLog("Error!! Cannot find Data Capture App!");
		return;
	}
	m_fStart = !m_fStart;
	if(m_fStart)
	{
		m_btnStartStop.SetWindowText("Stop");
		pWnd->PostMessage(WM_APP_BSS, WP_START_CAPTURE); 
		AddLog("PostMessage to DC : WP_START_CAPTURE");
	}
	else // stop
	{
		m_btnStartStop.SetWindowText("Start");
		pWnd->PostMessage(WM_APP_BSS, WP_STOP_CAPTURE);
		AddLog("PostMessage to DC : WP_STOP_CAPTURE");
	}
}

LRESULT CStageControllerDlg::OnAppBSS(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case WP_DD_LAUNCH_DONE:
		AddLog("Get Message from DD : Launched Data Dispacther App.");
		break;
	case WP_DC_LAUNCH_DONE:
		AddLog("Get Message from DC : Launched Data Capture App.");
		break;
	case WP_START_CAPTURE:
		AddLog("Get Message from DC : Started Data Capture.");
		break;
	case WP_STOP_CAPTURE:
		AddLog("Get Message from DC : Stopped Data Capture.");
		break;
	case WP_DC_SAVE_ERROR:
		AddLog("Get Message from DC : Error in saving a file.");
		// jrkwon 2007-09-25
		OnBnClickedBtnSsStop();
		break;
	case WP_DC_SAVE_DONE:
		AddLog("Get Message from DC : Success to save a file.");
		m_pMotionController->m_bGo = true; // rudimentary sync

#ifdef FEATURE_CHECK_CAPTURED_IMAGE
		// jrkwon 2007-09-25 -----------------------
		// check the captured image
		if(CheckCapturedImage() == false)
		{
			int nResult = AfxMessageBox("The camera or light source has a problem.\nAfer fix the problem, click OK to continue\nIf you want to pause the stair-step cutting, click CANCEL.", MB_OKCANCEL|MB_ICONEXCLAMATION);
			if(nResult == IDCANCEL)
			{
				// if stair-step cutting is running, pause!!
				if(m_pMotionController->m_bStepByStep)
					OnBnClickedBtnSsGo();
			}
		}
		// ------------------------------------------
#endif
		break;
	}

	return 0;
}

bool CStageControllerDlg::CheckCapturedImage()
{
	uint32 nImageLength;
	uint32 nImageWidth;
	uint32 nRow;
	uint32 nWhitePixel;
	uint32 nBlackPixel;
	unsigned char *pLine;
	tdata_t pBuf;
	bool bRet;

	bRet = true;

	TIFF* pTif = TIFFOpen(m_pMotionController->m_strImageFileName, "r");
    if (pTif) 
	{
		nWhitePixel = nBlackPixel = 0;

		TIFFGetField(pTif, TIFFTAG_IMAGELENGTH, &nImageLength);
		nImageWidth = TIFFScanlineSize(pTif);
		pBuf = _TIFFmalloc(nImageWidth);
		for (nRow = 0; nRow < nImageLength; nRow++)
		{
			TIFFReadScanline(pTif, pBuf, nRow);
			pLine = (unsigned char*)pBuf;

			for(uint32 i = 0; i < nImageWidth; i++)
			{
				if(pLine[i] == 0xFF)
					nWhitePixel++;
				else if(pLine[i] == 0x00)
					nBlackPixel++;
			}
		}
		if((nWhitePixel > (nImageWidth*nImageLength/2))
			|| (nBlackPixel > (nImageWidth*nImageLength/2)))
			bRet = false;

		CString strMsg;
		strMsg.Format("White: %d, Black: %d", nWhitePixel, nBlackPixel);
		AddLog(strMsg);

		_TIFFfree(pBuf);
		TIFFClose(pTif);
    }
	return bRet;
}

void CStageControllerDlg::UpdatePosition()
{
	UpdateData(0);
}

void CStageControllerDlg::OnBnClickedBtnSettings()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //LoadParameters(szFILE4PARAMS);

	//CSettingsDlg dlgSettings(this);
    //dlgSettings.DoModal();
	
	CSettingsDlg *pSettingsDlg;

	if(!m_fSetDlgOpen)
	{
		// Modaless dialog
		pSettingsDlg = new CSettingsDlg(this);
		pSettingsDlg->Create(IDD_SETTINGS_DIALOG);
		pSettingsDlg->ShowWindow(SW_SHOW);
		m_fSetDlgOpen = true;
	}

    //SaveParameters(szFILE4PARAMS);
}

void CStageControllerDlg::LoadParameters(char *pszFileName)
{
    std::ifstream grab(pszFileName);

    //check file exists
    if (!grab)
    {
        return;
    }

    //load in from the file
    char szParamDescription[256];

#define SKIP_PARAM_DESCRIPTION      grab >> szParamDescription

    int nMode = m_pMotionController->m_CuttingMode;

    SKIP_PARAM_DESCRIPTION;
    grab >> nMode;
	m_pMotionController->m_CuttingMode = (CMotionController::eCuttingMode)nMode;
    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_dSliceSizeY;
    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_dSliceSizeZ;
    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_dSliceSizeX;
    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_dLiftHeight;
    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_dReturnSpeed;
    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_dAverageCuttingSpeed;
    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_dDeviation;
    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_nWaitBetweenSlice;

    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_nSlicesPerPlank;
    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_nStairPlankDepth;
    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_nStairPlankWidth;
    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_dFireStep;
    SKIP_PARAM_DESCRIPTION;
    grab >> m_pMotionController->m_dFirePulseWidth;

//    SKIP_PARAM_DESCRIPTION;
//    grab >> m_pMotionController->m_nNumCols;
//    SKIP_PARAM_DESCRIPTION;
//    grab >> m_pMotionController->m_nNumSlices;
	int nEdge = m_pMotionController->m_pStairStep->m_CuttingEdge;
    SKIP_PARAM_DESCRIPTION;
    grab >> nEdge;
	m_pMotionController->m_pStairStep->m_CuttingEdge = (CStairStep::eCuttingEdge)nEdge;
    SKIP_PARAM_DESCRIPTION;
	grab >> m_pMotionController->m_pStairStep->m_dBlockWidth;
    SKIP_PARAM_DESCRIPTION;
	grab >> m_pMotionController->m_pStairStep->m_dColumnWidth;
    SKIP_PARAM_DESCRIPTION;
	grab >> m_pMotionController->m_pStairStep->m_dThickness;
    SKIP_PARAM_DESCRIPTION;
	grab >> m_pMotionController->m_pStairStep->m_nPlankDepth;

	// should calculate nNumCols 'cause it is not saved
	m_pMotionController->m_pStairStep->m_nNumCols 
		= (int)ceil(m_pMotionController->m_pStairStep->m_dBlockWidth 
					/ m_pMotionController->m_pStairStep->m_dColumnWidth);

	CString strMsg;
	strMsg.Format("Read params from %s", pszFileName);
	AddLog(strMsg);
}

void CStageControllerDlg::SaveParameters(char *pszFileName)
{
    std::ofstream grab(pszFileName);

    //check file exists
    if (!grab)
    {
        return;
    }

    grab << "CuttingMode: ";
    grab << (int)m_pMotionController->m_CuttingMode << std::endl;
    grab << "SliceWidth: ";
    grab << m_pMotionController->m_dSliceSizeY << std::endl;
    grab << "SliceThickness: ";
    grab << m_pMotionController->m_dSliceSizeZ << std::endl;
    grab << "SliceLength: ";
    grab << m_pMotionController->m_dSliceSizeX << std::endl;
    grab << "LiftHeight: ";
    grab << m_pMotionController->m_dLiftHeight << std::endl;
    grab << "ReturnSpeed: ";
    grab << m_pMotionController->m_dReturnSpeed << std::endl;
    grab << "AverageCuttingSpeed: ";
    grab << m_pMotionController->m_dAverageCuttingSpeed << std::endl;
    grab << "Deviation: ";
    grab << m_pMotionController->m_dDeviation << std::endl;
    grab << "Wait: ";
    grab << m_pMotionController->m_nWaitBetweenSlice << std::endl;
    grab << "SlicePerPlank: ";
    grab << m_pMotionController->m_nSlicesPerPlank << std::endl;
    grab << "PlankDepth: ";
    grab << m_pMotionController->m_nStairPlankDepth << std::endl;
    grab << "PlankWidth: ";
    grab << m_pMotionController->m_nStairPlankWidth << std::endl;
    grab << "FireStep: ";
    grab << m_pMotionController->m_dFireStep << std::endl;
    grab << "FirePulseWidth: ";
    grab << m_pMotionController->m_dFirePulseWidth << std::endl;

//	grab << "NumCols: ";
//    grab << m_pMotionController->m_nNumCols << std::endl;
//    grab << "NumSlices: ";
//    grab << m_pMotionController->m_nNumSlices << std::endl;

    grab << "CuttingEdge: ";
	grab << (int)m_pMotionController->m_pStairStep->m_CuttingEdge << std::endl;
	grab << "BlockWidth: ";
	grab << m_pMotionController->m_pStairStep->m_dBlockWidth << std::endl;
    grab << "ColumnWidth: ";
	grab << m_pMotionController->m_pStairStep->m_dColumnWidth << std::endl;
	grab << "Thickness: ";
	grab << m_pMotionController->m_pStairStep->m_dThickness << std::endl;
    grab << "PlankDepth: ";
	grab << m_pMotionController->m_pStairStep->m_nPlankDepth << std::endl;

	CString strMsg;
	strMsg.Format("Write params to %s", pszFileName);
	AddLog(strMsg);
}

void CStageControllerDlg::OnBnClickedBtnSendCommand()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_pMotionController->SendCommand(m_strCommand.GetBuffer());
}

void CStageControllerDlg::OnBnClickedBtnInitStage()
{
	// TODO: Add your control notification handler code here
	m_pMotionController->Initialize();
	AddLog("Initializing...");
}

void CStageControllerDlg::OnBnClickedBtnResetStage()
{
	// TODO: Add your control notification handler code here
	m_pMotionController->Reset();
	AddLog("Resetting...");
}

#define sCMD_ENABLE_XYZ			"EN X Y Z"
#define sCMD_DISABLE_XYZ		"DI X Y Z"

void CStageControllerDlg::OnBnClickedBtnEnableStage()
{
	// TODO: Add your control notification handler code here
	m_pMotionController->SendCommand(sCMD_ENABLE_XYZ);
	AddLog(sCMD_ENABLE_XYZ);
	GetDlgItem(IDC_BTN_ENABLE_STAGE)->SetWindowText("Disable XYZ");
}

void CStageControllerDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogSK::OnOK();
}

void CStageControllerDlg::OnBnClickedBtnDisableStage()
{
	// TODO: Add your control notification handler code here
	m_pMotionController->SendCommand(sCMD_DISABLE_XYZ);
	AddLog(sCMD_DISABLE_XYZ);
	GetDlgItem(IDC_BTN_ENABLE_STAGE)->SetWindowText("Enable XYZ");
}

void CStageControllerDlg::OnBnClickedBtnCutASlice()
{
	// TODO: Add your control notification handler code here
	//CWnd *pWnd = FindWindow(NULL, BSS_DCAPTURE_TITLE);
	//if(pWnd == NULL)
	//{
	//	AddLog("Error!! Cannot find Data Capture App!");
	//	return;
	//}
	//pWnd->PostMessage(WM_APP_BSS, WP_START_CAPTURE); 
	//AddLog("PostMessage to DC : WP_START_CAPTURE");

	//m_pMotionController->CutSlice(m_pMotionController->GetSpeed());

	//wczhang 2014-04-18
	SetTimer(2,4000,NULL);
	AddLog("Launching Image Error Detection...");
	CString err_detection_program = "E:\\ChangeDetection\\code\\wingui.py";
	CString curr_folder = "\""+m_strFolderName+"\"";
	ShellExecute(this->m_hWnd,"open",err_detection_program,
		curr_folder,"",SW_SHOW);
	
	//AddLog("Starting to cut a slice");
}

void CStageControllerDlg::OnBnClickedBtnCutAPlank()
{
	//CWnd *pWnd = FindWindow(NULL, BSS_DCAPTURE_TITLE);
	//if(pWnd == NULL)
	//{
	//	AddLog("Error!! Cannot find Data Capture App!");
	//	return;
	//}
	//pWnd->PostMessage(WM_APP_BSS, WP_START_CAPTURE); 
	//AddLog("PostMessage to DC : WP_START_CAPTURE");

	// TODO: Add your control notification handler code here
	if(m_pMotionController->CutPlank())
		AddLog("Starting to cut a plank");
	else
		AddLog("Fail to start thread or error while cutting a plank");
}

void CStageControllerDlg::OnBnClickedBtnStop()
{
	// TODO: Add your control notification handler code here
	m_pMotionController->Stop();
	CWnd *pWnd = FindWindow(NULL, BSS_DCAPTURE_TITLE);
	if(pWnd == NULL)
	{
		AddLog("Error!! Cannot find Data Capture App!");
		//return;
	}
	else
	{
		pWnd->PostMessage(WM_APP_BSS, WP_STOP_CAPTURE); 
		AddLog("PostMessage to DC : WP_STOP_CAPTURE");
	}

	Sleep(1000);
	m_pMotionController->Initialize();
	AddLog("Initializing...");
	m_sttLeftSlices.Format("%04d", 0);
}

void CStageControllerDlg::OnBnClickedBtnPlankSession()
{
	// TODO: Add your control notification handler code here
	if(m_pMotionController->StairSteppingSession())
		AddLog("Starting to cut a StairSteppingSession");
	else
		AddLog("Fail to start thread or error in the middle of Stair Stepping Session");
}

void CStageControllerDlg::OnBnClickedBtnTestCaptureSend()
{
	// TODO: Add your control notification handler code here
}

void CStageControllerDlg::OnBnClickedCheckSaveimages()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}


void CStageControllerDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//wczhang 2014-04-18
	if(nIDEvent==1){
		UpdatePosition();
	}
	if(nIDEvent==2){
		//AddLog("TimeEvent!");
		CString m_strErrorChangeFile = m_strFolderName + "\\CuttingPause.txt";
		FILE *f_error_detect = fopen(m_strErrorChangeFile,"r");
		if(f_error_detect != NULL){
			int value=-1;
			fscanf(f_error_detect,"%d\n",&value);
			if(value==1) {
				OnBnClickedBtnSsGo();
				FILE *f_error_detect = fopen(m_strErrorChangeFile,"w");
				//f_error_detect.write('0');
				fputs("0",f_error_detect);
				fclose(f_error_detect);
			}
		}
	}

	//UpdatePosition();
	//m_sttLeftSlices.Format("%03d", m_pMotionController->m_nSlicesPerPlank-m_pMotionController->m_nCurPlankNo-1);
	CDialogSK::OnTimer(nIDEvent);
}

void CStageControllerDlg::OnBnClickedBtnLaunchImageCapture()
{
	// TODO: Add your control notification handler code here
	char szDir[_MAX_PATH];

	CWnd *pWnd = FindWindow(NULL, BSS_DCAPTURE_TITLE);
	if(pWnd)
		return;

	GetCurrentDirectory(_MAX_PATH, szDir);
	chdir("..\\ImageCapture");	
	ShellExecute(this->m_hWnd, "open", BSS_PATH_IMAGE_CAPTURE, "", "", SW_SHOW);
	//chdir("..\\StageController");
	SetCurrentDirectory(szDir);
	AddLog("Launching Data Capture...");
}

//void CStageControllerDlg::OnDestroy()
//{
//	CDialogSK::OnDestroy();
//
//	// TODO: Add your message handler code here
//}

//jrkwon 2008-01-30
#define DATA_FOLDER_NAME		"H:\\KESM\\"

void CStageControllerDlg::OnBnClickedButtonNewSession()
{
	// TODO: Add your control notification handler code here
	CSessionDescDlg dlgSession(this);
	dlgSession.DoModal();
	//UpdateData(false);
	
	CTime CurTime = CTime::GetCurrentTime();

	//jrkwon 2008-01-30
	m_strFolderName.Format("..\\Data\\%04d-%02d-%02d %s", 
						CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), m_strSessionTitle);
	//m_strFolderName.Format("%sData\\%04d-%02d-%02d %s", DATA_FOLDER_NAME,
	//					CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), m_strSessionTitle);
	m_pMotionController->SetLogFilenames();
	InitMotionController();
	m_pMotionController->m_bFirstStepByStep = true;

	if(CreateDirectory(m_strFolderName, NULL))
	{
		CString strFileName;

		strFileName.Format("%s\\%s.txt", m_strFolderName, m_strSessionTitle);
		std::ofstream SessionDescTxt(strFileName);
		SessionDescTxt << m_strSessionDesc;
		SessionDescTxt.close();

		EnableAllItems(true);

		strFileName = "";
		strFileName.Format("%s\\%s", m_strFolderName, szFILE4PARAMS);
		SaveParameters(strFileName.GetBuffer());
	}
	else
	{
		CString strMsg;

		strMsg.Format("%s: %s", m_strFolderName, "Same folder name exists or disk full");
		AfxMessageBox(strMsg);
		m_strFolderName = "";
	}
}

void CStageControllerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CWnd *pWnd = FindWindow(NULL, BSS_DCAPTURE_TITLE);
	if(pWnd)
	{
		pWnd->SendMessage(WM_CLOSE);
	}

	CDialogSK::OnClose();
}
void CStageControllerDlg::EnableAllItems(bool bEnable)
{
	GetDlgItem(IDC_BTN_SETTINGS)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_SAVEIMAGES)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_CUT_A_SLICE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_CUT_A_PLANK)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_PLANK_SESSION)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_ENABLE_STAGE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_DISABLE_STAGE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_RESET_STAGE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_LAUNCH_IMAGE_CAPTURE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SS_START_STOP)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SS_PAUSE_RESUME)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SS_INIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SS_STEP_BY_STEP)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SS_GO)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SS_STOP)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SS_SET_START)->EnableWindow(bEnable);
}

bool CStageControllerDlg::GetFolder(std::string& folderpath, LPCTSTR szCaption, LPCWSTR szStartPath, HWND hOwner)
{
	bool retVal = false;

	// get a ITEMIDLIST for the starting path
	LPITEMIDLIST  pIDLRoot  = NULL;

	if (szStartPath != NULL)
	{
		LPSHELLFOLDER pShellFolder = NULL;
		HRESULT       hResult      = NULL;
		ULONG         chUsed       = 0L;
		BSTR          bstrPath     = NULL;

		bstrPath = ::SysAllocString ( szStartPath );

		if ( NULL != bstrPath )
		{
			// Get desktop IShellFolder interface
			if (::SHGetDesktopFolder (&pShellFolder) == NOERROR)
			{
				// convert the path to an ITEMIDLIST
				hResult = 
					pShellFolder->ParseDisplayName 
						(
							NULL,			// owner window
							NULL,			// reserved (must be NULL)
							bstrPath,       	// folder name
							&chUsed,		// number of chars parsed
							&pIDLRoot,      // ITEMIDLIST
							NULL            // attributes (can be NULL)
						);

				if (FAILED(hResult))
				{
					pIDLRoot = NULL;
				}

				pShellFolder->Release();
			}
		}

		::SysFreeString(bstrPath);
	}

	// The BROWSEINFO struct tells the shell 
	// how it should display the dialog.
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.pidlRoot  = pIDLRoot;
	bi.ulFlags   = BIF_USENEWUI;
	bi.hwndOwner = hOwner;
	bi.lpszTitle = szCaption;

	// must call this if using BIF_USENEWUI
	::OleInitialize(NULL);

	// Show the dialog and get the itemIDList for the selected folder.
	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);

	if(pIDL != NULL)
	{
		// Create a buffer to store the path, then get the path.
		char buffer[_MAX_PATH] = {'\0'};
		if(::SHGetPathFromIDList(pIDL, buffer) != 0)
		{
			// Set the string value.
			folderpath = buffer;
			retVal = true;
		}		

		// free the item id list
		::CoTaskMemFree(pIDL);
	}

	::OleUninitialize();

	return retVal;
}

void CStageControllerDlg::OnBnClickedButtonOpenSession()
{
	// TODO: Add your control notification handler code here
	//char *pszString = "E:\\Project\\kesm\\StageController";
	//CStringW wString(pszString);
	std::string strSessionName;
	CString strPath;
	char szPath[MAX_PATH];

	//jrkwon 2008-01-30
	GetCurrentDirectory(MAX_PATH, szPath);
	//strcpy(szPath, DATA_FOLDER_NAME);

	char *pszPath;
	pszPath = strrchr(szPath, '\\');
	if(pszPath != NULL)
		*pszPath = '\0';
	strcat(szPath, "\\Data");
	strPath = szPath;
	CStringW wString(strPath);

	if(GetFolder(strSessionName, "Select a session", wString))
	{
		// remove drive and directory name before the session name

		//2014-4-17 wczhang
		char * cstr = new char [strSessionName.length()+1];
		std::strcpy (cstr, strSessionName.c_str());
		pszPath = strrchr(cstr, '\\');
		//pszPath = strrchr(strSessionName.c_str(), '\\');


		if(pszPath != NULL)
		{
			//jrkwon 2008-01-30
			m_strFolderName.Format("..\\Data\\%s", pszPath+1);
			//m_strFolderName.Format("%s\\Data\\%s", DATA_FOLDER_NAME, pszPath+1);
			
			m_pMotionController->SetLogFilenames();
			InitMotionController();
		}
		CString strFileParam;
		strFileParam.Format("%s\\%s", m_strFolderName, szFILE4PARAMS);
		//AfxMessageBox(strSessionName.c_str());
		// 1. load the params
		LoadParameters(strFileParam.GetBuffer());
		pszPath = strchr(pszPath+1, ' ');
		if(pszPath)
			m_strSessionTitle = pszPath+1;
		EnableAllItems(true);

		if(m_fSetDlgOpen)
		{
			CWnd *pWnd = FindWindow(NULL, "Settings");
			if(pWnd != NULL)
			{
				pWnd->PostMessage(WM_CLOSE);
			}
		}
	}
}

void CStageControllerDlg::OnBnClickedBtnStairStep()
{
	// TODO: Add your control notification handler code here
	if(m_pMotionController->StairSteppingSession())
		AddLog("Starting to cut a StairSteppingSession");
	else
		AddLog("Fail to start thread or error in the middle of Stair Stepping Session");
}

void CStageControllerDlg::OnBnClickedBtnSsStartStop()
{
	// TODO: Add your control notification handler code here

	if(m_bStairStepStart)
	{
		int nResult = AfxMessageBox("Are you SURE to start Stair Step Cutting?", MB_YESNO|MB_ICONQUESTION);
		if(nResult == IDNO)
		{
		   return;
		}

		GetDlgItem(IDC_BTN_SS_INIT)->EnableWindow(false);

		if(m_pMotionController->StairSteppingSession())
			AddLog("Starting to cut a StairSteppingSession");
		else
			AddLog("Fail to start thread or error in the middle of Stair Stepping Session");

		m_btnStartStopSS.SetWindowText("Stop");
		m_bStairStepStart = false;
	}
	else
	{
		m_pMotionController->Stop();
		CWnd *pWnd = FindWindow(NULL, BSS_DCAPTURE_TITLE);
		if(pWnd == NULL)
		{
			AddLog("Error!! Cannot find Data Capture App!");
			//return;
		}
		else
		{
			pWnd->PostMessage(WM_APP_BSS, WP_STOP_CAPTURE); 
			AddLog("PostMessage to DC : WP_STOP_CAPTURE");
		}

		Sleep(1000);
		m_pMotionController->Initialize();
		AddLog("Initializing...");
		m_sttLeftSlices.Format("%04d", 0);
		
		//m_pMotionController->SaveCurState(m_pMotionController);
		m_btnStartStopSS.SetWindowText("Start");

		// clear PauseResume button
		if(!m_bStairStepPause)
			OnBnClickedBtnSsPauseResume();
		m_bStairStepStart = true;

		GetDlgItem(IDC_BTN_SS_INIT)->EnableWindow(true);
	}
}

void CStageControllerDlg::OnBnClickedBtnSsPauseResume()
{
	// TODO: Add your control notification handler code here
	if(m_bStairStepStart == false) // start stair step cutting
	{
		if(m_bStairStepPause)
		{
			m_pMotionController->m_bStairStepPause = true;
			m_btnPauseResumeSS.SetWindowText("Resume");
			m_bStairStepPause = false;
		}
		else
		{
			m_pMotionController->m_bStairStepPause = false;
			m_btnPauseResumeSS.SetWindowText("Pause");
			m_bStairStepPause = true;
		}
	}
}

void CStageControllerDlg::OnBnClickedBtnSsInit()
{
	m_fFirstClickStepByStep = false;
	// TODO: Add your control notification handler code here
	//m_pMotionController->m_bStepByStep = true;

	//OnBnClickedBtnSsStartStop();
	if(m_pMotionController->InitStairStep())
		GetDlgItem(IDC_BTN_SS_INIT)->EnableWindow(false);
}

void CStageControllerDlg::OnBnClickedBtnSsStepByStep()
{
	if(m_fFirstClickStepByStep == false)
	{
		m_fFirstClickStepByStep = true;

		if(m_pMotionController->StairSteppingSession())
			AddLog("Starting to cut a StairSteppingSession");
		else
			AddLog("Fail to start thread or error in the middle of Stair Stepping Session");
	}
	else
	{
		// TODO: Add your control notification handler code here
		SetEvent(m_pMotionController->m_hEventStepByStep);
		// you can click the step by step button while you are in pause
		m_pMotionController->m_bStepByStep = true;
		//m_pMotionController->SaveCurState(m_pMotionController);
	}
}

void CStageControllerDlg::OnBnClickedBtnSsGo()
{
	// TODO: Add your control notification handler code here
	if(m_pMotionController->m_bStepByStep)
	{
		GetDlgItem(IDC_BTN_SS_GO)->SetWindowText("Return2SS");
		m_pMotionController->m_bStepByStep = false;
	}
	else
	{
		GetDlgItem(IDC_BTN_SS_GO)->SetWindowText("Go!");
		m_pMotionController->m_bStepByStep = true;
	}


	SetEvent(m_pMotionController->m_hEventStepByStep);
	//m_pMotionController->m_bStepByStep = false;
}

void CStageControllerDlg::InitMotionController()
{
	m_pMotionController->Initialize();
	AddLog("Initializing...");
	m_sttLeftSlices.Format("%04d", 0);
}

void CStageControllerDlg::OnBnClickedBtnSsSetStart()
{
	// TODO: Add your control notification handler code here
	AddLog("Reset Starting Position");
	m_pMotionController->ResetStartPos();
}

void CStageControllerDlg::OnBnClickedBtnSsStop()
{
	m_pMotionController->Stop();
	CWnd *pWnd = FindWindow(NULL, BSS_DCAPTURE_TITLE);
	if(pWnd == NULL)
	{
		AddLog("Error!! Cannot find Data Capture App!");
		//return;
	}
	else
	{
		pWnd->PostMessage(WM_APP_BSS, WP_STOP_CAPTURE); 
		AddLog("PostMessage to DC : WP_STOP_CAPTURE");
	}

	Sleep(1000);
	m_pMotionController->Initialize();
	AddLog("Initializing...");
	m_sttLeftSlices.Format("%04d", 0);
	
	//m_pMotionController->SaveCurState(m_pMotionController);
	//m_btnStartStopSS.SetWindowText("Start");
	GetDlgItem(IDC_BTN_SS_GO)->SetWindowText("Go");

	// clear PauseResume button
	if(!m_bStairStepPause)
		OnBnClickedBtnSsPauseResume();
	m_bStairStepStart = true;

	GetDlgItem(IDC_BTN_SS_INIT)->EnableWindow(true);
}

bool CStageControllerDlg::GetnSetpDCapture() {
	pDCapture = FindWindow(NULL, BSS_DCAPTURE_TITLE);
	if(pDCapture == NULL)
	{
		AddLog("Error!! Cannot find Data Capture App!");
		MessageBox("Error!! Cannot find Data Capture App!");
		return false;
	}
	return true;
}

LRESULT CStageControllerDlg::SendMessage2DCapture(UINT msg, WPARAM wParam, LPARAM lParam) {
	LRESULT rtl;
	if(pDCapture == NULL)
	{
		AddLog("Error!! Cannot find Data Capture App!");
		MessageBox("Error!! Cannot find Data Capture App!");
		return -1;
	}
	rtl = pDCapture->SendMessage(msg, wParam, lParam);
	return rtl;
}

LRESULT CStageControllerDlg::PostMessage2DCapture(UINT msg, WPARAM wParam, LPARAM lParam) {
	LRESULT rtl;
	if(pDCapture == NULL)
	{
		AddLog("Error!! Cannot find Data Capture App!");
		MessageBox("Error!! Cannot find Data Capture App!");
		return -1;
	}
	rtl = pDCapture->PostMessage(msg, wParam, lParam);
	return rtl;
}