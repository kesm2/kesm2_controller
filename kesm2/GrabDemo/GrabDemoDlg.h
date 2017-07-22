// GrabDemoDlg.h : header file
//

#if !defined(AFX_GRABDEMODLG_H__82BFE149_F01E_11D1_AF74_00A0C91AC0FB__INCLUDED_)
#define AFX_GRABDEMODLG_H__82BFE149_F01E_11D1_AF74_00A0C91AC0FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SapClassBasic.h"
#include "SapClassGui.h"

/////////////////////////////////////////////////////////////////////////////
// CGrabDemoDlg dialog

class CGrabDemoDlg : public CDialog
{
// Construction
public:
	CGrabDemoDlg(CWnd* pParent = NULL);	// standard constructor

	BOOL CreateObjects();
	BOOL DestroyObjects();
	void UpdateMenu();
   void UpdateTitleBar();
	static void XferCallback(SapXferCallbackInfo *pInfo);
	static void SignalCallback(SapAcqCallbackInfo *pInfo);
   void GetSignalStatus();
   void GetSignalStatus(SapAcquisition::SignalStatus signalStatus);

// Dialog Data
	//{{AFX_DATA(CGrabDemoDlg)
	enum { IDD = IDD_GRABDEMO_DIALOG };
	CStatic	m_statusWnd;
	CScrollBar	m_verticalScr;
	CScrollBar	m_horizontalScr;
	CStatic	m_viewWnd;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrabDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// jwyoo, hwang, 02-29-2016
	char g_szFileName[_MAX_PATH];

	HICON		m_hIcon;
	CString  m_appTitle;

	CImageWnd		*m_ImageWnd;
	SapAcquisition	*m_Acq;
	SapBuffer		*m_Buffers;
	SapTransfer		*m_Xfer;
	SapView        *m_View;

    BOOL m_IsSignalDetected;   // TRUE if camera signal is detected

    LRESULT On_WM_APP_BSS_Receive(WPARAM wParam, LPARAM lParam);
	LRESULT On_WM_COPYDATA_Receive(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	//{{AFX_MSG(CGrabDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSnap();
	afx_msg void OnGrab();
	afx_msg void OnFreeze();
	afx_msg void OnGeneralOptions();
	afx_msg void OnAreaScanOptions();
	afx_msg void OnLineScanOptions();
	afx_msg void OnCompositeOptions();
	afx_msg void OnLoadAcqConfig();
	afx_msg void OnBufferOptions();
	afx_msg void OnViewOptions();
	afx_msg void OnFileLoad();
	afx_msg void OnFileNew();
	afx_msg void OnFileSave();
	afx_msg void OnExit();
    afx_msg void OnEndSession(BOOL bEnding);
    afx_msg BOOL OnQueryEndSession();
    
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRABDEMODLG_H__82BFE149_F01E_11D1_AF74_00A0C91AC0FB__INCLUDED_)
