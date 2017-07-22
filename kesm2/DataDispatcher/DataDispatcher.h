// DataDispatcher.h : main header file for the DATADISPATCHER application
//

#if !defined(AFX_DATADISPATCHER_H__306268AA_96F7_4AAB_B5F5_AFED1A7ED698__INCLUDED_)
#define AFX_DATADISPATCHER_H__306268AA_96F7_4AAB_B5F5_AFED1A7ED698__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDataDispatcherApp:
// See DataDispatcher.cpp for the implementation of this class
//

class CDataDispatcherApp : public CWinApp
{
public:
	CDataDispatcherApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataDispatcherApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDataDispatcherApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATADISPATCHER_H__306268AA_96F7_4AAB_B5F5_AFED1A7ED698__INCLUDED_)
