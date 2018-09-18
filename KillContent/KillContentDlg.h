// KillContentDlg.h : header file
//

#if !defined(AFX_KILLCONTENTDLG_H__C9458871_9D82_49CC_AB79_21BEFD9CDB6E__INCLUDED_)
#define AFX_KILLCONTENTDLG_H__C9458871_9D82_49CC_AB79_21BEFD9CDB6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKillContentDlg dialog
#include "KC_File.h"
#include "KC_Fold.h"
#include "KillOption.h"

CString GetCurrentAppPath(void);//返回当前应用所在路径
static CString CurrentAppPath;//当前应用所在路径

class CKillContentDlg : public CDialog
{
// Construction
public:
	CKillContentDlg(CWnd* pParent = NULL);	// standard constructor
    
// Dialog Data
	//{{AFX_DATA(CKillContentDlg)
	enum { IDD = IDD_KILLCONTENT_DIALOG };
	CButton	m_Button_Close;
	CButton	m_Button_Option;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKillContentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
    CKC_File C_File;
    CKC_Fold C_Fold;

    CPropertySheet CK_Sheet;

    DWORD KillOption;
    void SetText(void);//设置文字
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKillContentDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonOption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KILLCONTENTDLG_H__C9458871_9D82_49CC_AB79_21BEFD9CDB6E__INCLUDED_)
