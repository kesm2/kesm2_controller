// SimImageCaptureDlg.h : header file
//

#if !defined(AFX_SIMIMAGECAPTUREDLG_H__B164B138_3EE9_490A_809E_BCABC443DB92__INCLUDED_)
#define AFX_SIMIMAGECAPTUREDLG_H__B164B138_3EE9_490A_809E_BCABC443DB92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSimImageCaptureDlg dialog

class CSimImageCaptureDlg : public CDialog
{
// Construction
public:
	CSimImageCaptureDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSimImageCaptureDlg)
	enum { IDD = IDD_SIMIMAGECAPTURE_DIALOG };
	CButton	m_btnStartStop;
	CListBox	m_lstLog;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimImageCaptureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AddLog(CString strLog);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSimImageCaptureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnStartStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnClearMsg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMIMAGECAPTUREDLG_H__B164B138_3EE9_490A_809E_BCABC443DB92__INCLUDED_)
