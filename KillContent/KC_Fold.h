#if !defined(AFX_KC_FOLD_H__973C14F2_F2F9_4ED2_BCE9_91937F7FE446__INCLUDED_)
#define AFX_KC_FOLD_H__973C14F2_F2F9_4ED2_BCE9_91937F7FE446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KC_Fold.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKC_Fold dialog
#include "KillListCtrl.h"
#include "OperateKillNote.h"

class CKC_Fold : public CPropertyPage
{
	DECLARE_DYNCREATE(CKC_Fold)

// Construction
public:
	CKC_Fold();
	~CKC_Fold();

// Dialog Data
	//{{AFX_DATA(CKC_Fold)
	enum { IDD = IDD_DIALOG_FOLD };
	CButton	m_Static_FoldClewBox;
	CStatic	m_Static_ClewFold;
	CButton	m_Button_Delete;
	CKillListCtrl	m_FoldList;
	//}}AFX_DATA

private:
    void SetText(void);//…Ë÷√Œƒ◊÷
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKC_Fold)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKC_Fold)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDeletefold();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KC_FOLD_H__973C14F2_F2F9_4ED2_BCE9_91937F7FE446__INCLUDED_)
