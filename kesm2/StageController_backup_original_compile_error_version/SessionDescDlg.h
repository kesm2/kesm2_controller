#pragma once


// CSessionDescDlg dialog

class CSessionDescDlg : public CDialog
{
	DECLARE_DYNAMIC(CSessionDescDlg)

public:
	CSessionDescDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSessionDescDlg();

// Dialog Data
	enum { IDD = IDD_SESSION_DESC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStageControllerDlg *m_pParentDlg;
	afx_msg void OnBnClickedOk();
	CString m_strTitle;
	CString m_strDesc;
};
