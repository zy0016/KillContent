// KillContent.h : main header file for the KILLCONTENT application
//

#if !defined(AFX_KILLCONTENT_H__2FAD9536_5ED4_4962_B216_B0BE97B66588__INCLUDED_)
#define AFX_KILLCONTENT_H__2FAD9536_5ED4_4962_B216_B0BE97B66588__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKillContentApp:
// See KillContent.cpp for the implementation of this class
//

class CKillContentApp : public CWinApp
{
public:
	CKillContentApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKillContentApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKillContentApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KILLCONTENT_H__2FAD9536_5ED4_4962_B216_B0BE97B66588__INCLUDED_)
