// DataServerDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "..\SocketLib\SocketJr.h"
#include "..\SocketLib\ServerSocket.h"
#include "..\SocketLib\SocketException.h"

// CDataServerDlg dialog
class CDataServerDlg : public CDialog
{
// Construction
public:
	CDataServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DATASERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	UINT m_nPortNumber;
	CListBox m_lstLog;
	BOOL m_fStart;
	CServerSocket *m_pServer;
	UINT m_nFileNo;

	afx_msg void OnBnClickedBtnStartStop();
	CButton m_btnStartStop;
	void AddLog(CString strLog);
	afx_msg void OnBnClickedBtnClearLog();
	LRESULT OnAppBSS(WPARAM wParam, LPARAM lParam);
	void DoReceiveData();
	afx_msg void OnClose();
};
