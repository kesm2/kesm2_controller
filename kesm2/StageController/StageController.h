// StageController.h : main header file for the STAGECONTROLLER application
//

#if !defined(AFX_STAGECONTROLLER_H__F817DBBB_027A_4801_802C_C0A060DD4FA4__INCLUDED_)
#define AFX_STAGECONTROLLER_H__F817DBBB_027A_4801_802C_C0A060DD4FA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CStageControllerApp:
// See StageController.cpp for the implementation of this class
//

class CStageControllerApp : public CWinApp
{
public:
	CStageControllerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStageControllerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CStageControllerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STAGECONTROLLER_H__F817DBBB_027A_4801_802C_C0A060DD4FA4__INCLUDED_)
