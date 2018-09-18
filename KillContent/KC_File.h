#if !defined(AFX_KC_FILE_H__E6354829_392D_4F25_9EAD_A3C50A420278__INCLUDED_)
#define AFX_KC_FILE_H__E6354829_392D_4F25_9EAD_A3C50A420278__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KC_File.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKC_File dialog
#include "KillListCtrl.h"
#include "PreviewCode.h"
#include "OperateKillNote.h"

class CKC_File : public CPropertyPage
{
	DECLARE_DYNCREATE(CKC_File)

// Construction
public:
	CKC_File();
	~CKC_File();

// Dialog Data
	//{{AFX_DATA(CKC_File)
	enum { IDD = IDD_DIALOG_FILE };
	CButton	m_Static_ClewBox;
	CStatic	m_Static_FileClew;
	CButton	m_Button_Preview;
	CButton	m_Button_Delete;
	CKillListCtrl	m_FileList;
	//}}AFX_DATA
private:
    void SetText(void);//…Ë÷√Œƒ◊÷

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKC_File)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKC_File)
	afx_msg void OnButtonPreview();
	afx_msg void OnButtonDelete();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KC_FILE_H__E6354829_392D_4F25_9EAD_A3C50A420278__INCLUDED_)
