#if !defined(AFX_PREVIEWCODE_H__F6EB8BA8_31DF_4950_BE50_68CA0ED446D5__INCLUDED_)
#define AFX_PREVIEWCODE_H__F6EB8BA8_31DF_4950_BE50_68CA0ED446D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewCode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreviewCode dialog
#include "GenNotesList.h"
#include "KillRichEditCtrl.h"
#include "KFind.h"

#define PREVIEWCODE_TIMER	1003
#define SNDUPDATEMSG()				//CMainFrame* lpFrame = (CMainFrame*)::AfxGetMainWnd();\
									//if(lpFrame->m_hWnd)\
									//lpFrame->SendMessage(WM_UPDATE_STATUSBAR_INFO, 0, 0)

class CPreviewCode : public CDialog
{
// Construction
public:
	CPreviewCode(CWnd* pParent = NULL);   // standard constructor
    void SetPreviewFileName(CString sourfilename,CString desfilename);
// Dialog Data
	//{{AFX_DATA(CPreviewCode)
	enum { IDD = IDD_DIALOG_PREVIEW };
	CEdit	m_File_Result;
	CEdit	m_File_Preview;
	CButton	m_Static_Splitter;
	CRichEditCtrl	m_Result;
	CRichEditCtrl	m_Preview;
	//}}AFX_DATA

private:
    long m_nSearchCharPos;
    CToolBar cPreToolBar;
    CString cSourFileName;
    CString cDesFileName;
    DWORD dKillOption;
    CHARFORMAT	chFormat;	//用于设置RichEdit中的字体格式
    CString cTempFile ;//临时文件名
    BOOL SetCommentColor(CString cfilename,CRichEditCtrl & richedit);
    void SetText(void);
    CString CreateTempFile(CString cSourFileName,CString cContent);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewCode)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreviewCode)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnButtonPreview();
    afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonFind();
	afx_msg void OnButtonPreviewAll();
	afx_msg void OnButtonPreviewHead();
	afx_msg void OnButtonPreviewFunc();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonPreviewSave();
	//}}AFX_MSG
    afx_msg LRESULT OnFindReplace(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWCODE_H__F6EB8BA8_31DF_4950_BE50_68CA0ED446D5__INCLUDED_)
