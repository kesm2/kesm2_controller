// StageControllerDlg.h : header file
//

#if !defined(AFX_STAGECONTROLLERDLG_H__5186E583_9CDE_4C27_B0D6_55B2E13A3FB9__INCLUDED_)
#define AFX_STAGECONTROLLERDLG_H__5186E583_9CDE_4C27_B0D6_55B2E13A3FB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "CDialogSK.h"
#include "motioncontroller.h"
#include "afxwin.h"
#include "Digistatic.h"
#include <string>

#define szFILE4PARAMS   "Params.txt"


/////////////////////////////////////////////////////////////////////////////
// CStageControllerDlg dialog

class CStageControllerDlg : public CDialogSK
{
// Construction
public:
	CStageControllerDlg(CWnd* pParent = NULL);	// standard constructor
	~CStageControllerDlg();	// standard constructor

// Dialog Data
	//{{AFX_DATA(CStageControllerDlg)
	enum { IDD = IDD_STAGECONTROLLER_DIALOG };
	CListBox	m_lstLog;
	CButton	m_btnStartStop;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStageControllerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnAppBSS(WPARAM wParam, LPARAM lParam);

	bool m_fStart;
	HICON m_hIcon;

	bool m_bStairStepStart;
	bool m_bStairStepPause;

	// Generated message map functions
	//{{AFX_MSG(CStageControllerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnLaunch();
	afx_msg void OnBtnStartStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void AddLog(CString strLog);
    afx_msg void OnBnClickedBtnSettings();
    CMotionController *m_pMotionController;
    void LoadParameters(char *pszFileName);
    void SaveParameters(char *pszFileName);
	CString m_strCommand;
	afx_msg void OnBnClickedBtnSendCommand();
	afx_msg void OnBnClickedBtnInitStage();
	afx_msg void OnBnClickedBtnResetStage();
	afx_msg void OnBnClickedBtnEnableStage();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedBtnDisableStage();
	afx_msg void OnBnClickedBtnCutASlice();
	afx_msg void OnBnClickedBtnCutAPlank();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnPlankSession();
	afx_msg void OnBnClickedBtnTestCaptureSend();
	BOOL m_fSaveImages;
	afx_msg void OnBnClickedCheckSaveimages();
	void UpdatePosition();
	CString m_strXPos;
	CString m_strYPos;
	CString m_strZPos;
	afx_msg void OnTimer(UINT nIDEvent);
	CString m_strXPosFB;
	CString m_strYPosFB;
	CString m_strZPosFB;
	CDigiStatic m_sttLeftSlices;
	afx_msg void OnBnClickedBtnLaunchImageCapture();
	bool m_fSetDlgOpen;
	bool m_fFirstClickStepByStep;

//	afx_msg void OnDestroy();
	CString m_strSessionDesc;
	afx_msg void OnBnClickedButtonNewSession();
	CString m_strFolderName;
	afx_msg void OnClose();
	CString m_strSessionTitle;
	void EnableAllItems(bool bEnable);
	afx_msg void OnBnClickedButtonOpenSession();
	bool GetFolder(std::string& folderpath, LPCTSTR szCaption = NULL, LPCWSTR szStartPath = NULL, HWND hOwner = NULL);
	UINT m_nColNum;
	UINT m_nSliceNum;
	afx_msg void OnBnClickedBtnStairStep();
	UINT m_nCurCol;
	double m_dCurZ;
	afx_msg void OnBnClickedBtnSsStartStop();
	afx_msg void OnBnClickedBtnSsPauseResume();
	CButton m_btnStartStopSS;
	CButton m_btnPauseResumeSS;
	afx_msg void OnBnClickedBtnSsInit();
	afx_msg void OnBnClickedBtnSsStepByStep();
	afx_msg void OnBnClickedBtnSsGo();
	void InitMotionController();
	afx_msg void OnBnClickedBtnSsSetStart();
	afx_msg void OnBnClickedBtnSsStop();

	bool CheckCapturedImage();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STAGECONTROLLERDLG_H__5186E583_9CDE_4C27_B0D6_55B2E13A3FB9__INCLUDED_)
