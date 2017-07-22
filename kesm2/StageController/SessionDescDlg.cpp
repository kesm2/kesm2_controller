// SessionDescDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StageController.h"
#include "StageControllerDlg.h"
#include "SessionDescDlg.h"
#include ".\sessiondescdlg.h"


// CSessionDescDlg dialog

IMPLEMENT_DYNAMIC(CSessionDescDlg, CDialog)
CSessionDescDlg::CSessionDescDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSessionDescDlg::IDD, pParent)
	, m_strTitle(_T(""))
	, m_strDesc(_T(""))
{
	m_pParentDlg = (CStageControllerDlg *)pParent;
}

CSessionDescDlg::~CSessionDescDlg()
{
}

void CSessionDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SESSION_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_EDIT_SESSION_DESCRIPTION, m_strDesc);
}


BEGIN_MESSAGE_MAP(CSessionDescDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CSessionDescDlg message handlers

void CSessionDescDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_pParentDlg->m_strSessionTitle = m_strTitle;
	m_pParentDlg->m_strSessionDesc = m_strDesc;
	OnOK();
}
