// SimImageCapture.h : main header file for the SIMIMAGECAPTURE application
//

#if !defined(AFX_SIMIMAGECAPTURE_H__B8E676D1_0705_46F6_99BA_9D8A7D048CAD__INCLUDED_)
#define AFX_SIMIMAGECAPTURE_H__B8E676D1_0705_46F6_99BA_9D8A7D048CAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSimImageCaptureApp:
// See SimImageCapture.cpp for the implementation of this class
//

class CSimImageCaptureApp : public CWinApp
{
public:
	CSimImageCaptureApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimImageCaptureApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSimImageCaptureApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMIMAGECAPTURE_H__B8E676D1_0705_46F6_99BA_9D8A7D048CAD__INCLUDED_)
