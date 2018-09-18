#if !defined(AFX_KFIND_H__1EBA57D4_7112_4538_BEE4_56D82D53878F__INCLUDED_)
#define AFX_KFIND_H__1EBA57D4_7112_4538_BEE4_56D82D53878F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KFind.h : header file
//
#include "afxadv.h"
/////////////////////////////////////////////////////////////////////////////
// CKFind dialog

class CKFind : public CDialog
{
// Construction
public:
	CKFind(CWnd* pParent = NULL);   // standard constructor

    CKFind(CString cInitString,CWnd* pParent = NULL);
    void SetInitString(CString cInitString);//设置初始字符串
    CString GetFindString(void);//获得需要查找的字符串
// Dialog Data
	//{{AFX_DATA(CKFind)
	enum { IDD = IDD_DIALOG_FIND };
	CButton	m_BUTTON_Find;
	CButton	m_Static_Direct;
	CStatic	m_Static_FindContent;
	CButton	m_Check_LU;
	CButton	m_Check_All;
	CButton	m_Cancel;
	CComboBox	m_Combo_Find;
	//}}AFX_DATA

private:
    CString cInitFindString;//显示的初始字符串
    CString cFindString;//需要查找的字符串
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKFind)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKFind)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KFIND_H__1EBA57D4_7112_4538_BEE4_56D82D53878F__INCLUDED_)
