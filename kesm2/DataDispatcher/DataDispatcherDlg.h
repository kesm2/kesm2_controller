// DataDispatcherDlg.h : header file
//

#if !defined(AFX_DATADISPATCHERDLG_H__0B71C59B_6DC1_4969_BE1C_F10533F8AA5E__INCLUDED_)
#define AFX_DATADISPATCHERDLG_H__0B71C59B_6DC1_4969_BE1C_F10533F8AA5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "Cluster.h"
#include "XListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDataDispatcherDlg dialog

class CDataDispatcherDlg : public CDialog
{
// Construction
public:
	CDataDispatcherDlg(CWnd* pParent = NULL);	// standard constructor
	~CDataDispatcherDlg();

// Dialog Data
	//{{AFX_DATA(CDataDispatcherDlg)
	enum { IDD = IDD_DATADISPATCHER_DIALOG };
	CButton	m_btnStartStop;
	CListBox	m_lstLog;
	CXListCtrl	m_lstFiles;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataDispatcherDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SendDataFile();
	DWORD DoSendData();
	static DWORD WINAPI SendDataThread(LPVOID lpArg);

	LRESULT OnAppBSS(WPARAM wParam, LPARAM lParam);
	//LRESULT OnUpdateProgress(WPARAM wParam, LPARAM lParam);
	
	bool m_fStart;
	void AddLog(CString strLog);
	HICON m_hIcon;
	CString m_strFileName;
	CString m_strSendingFileName;
	int m_nSendingItemNo;
	HANDLE m_hThread;
	bool m_fSending;
	//bool m_fQuit;

	CCluster m_Cluster;

	void InitListCtrl();
	void AddItem(CString strFileName/*, CTime Time*/);

	// Generated message map functions
	//{{AFX_MSG(CDataDispatcherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnClearMsg();
	afx_msg void OnBnClickedBtnClearList();
	afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATADISPATCHERDLG_H__0B71C59B_6DC1_4969_BE1C_F10533F8AA5E__INCLUDED_)
