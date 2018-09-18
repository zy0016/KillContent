#if !defined(AFX_KILLRICHEDITCTRL_H__CBCF3A7B_F19F_4F62_AB29_88C1E52DCAEC__INCLUDED_)
#define AFX_KILLRICHEDITCTRL_H__CBCF3A7B_F19F_4F62_AB29_88C1E52DCAEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KillRichEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKillRichEditCtrl window

class CKillRichEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CKillRichEditCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKillRichEditCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKillRichEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKillRichEditCtrl)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KILLRICHEDITCTRL_H__CBCF3A7B_F19F_4F62_AB29_88C1E52DCAEC__INCLUDED_)
