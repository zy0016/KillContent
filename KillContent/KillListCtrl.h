#if !defined(AFX_KILLLISTCTRL_H__831015A3_032F_496B_A0BD_A245F231EFAA__INCLUDED_)
#define AFX_KILLLISTCTRL_H__831015A3_032F_496B_A0BD_A245F231EFAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KillListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKillListCtrl window
#include "FileAddEdit.h"
#include "PreviewCode.h"

#define IDC_TOOLTIP_ID			1

#define LISTCTRL_INIT_WPARAM    WM_USER + 100//��ʼ���ؼ���wparam
#define LISTCTRL_INIT_LPARAM    WM_USER + 100//��ʼ���ؼ���lparam

///////////////menu////////////////////////
#define IDC_MENU_ADDFILES		200//�����Ŀ
#define IDC_MENU_DELETEFILE		210//ɾ����Ŀ
#define IDC_MENU_MODIFY         220//�޸���Ŀ
#define IDC_MENU_PREVIEW        230//Ԥ������
//#include "FileDialogEx.h"

typedef enum
{
    KILLLIST_FILE,//�г��ļ�
    KILLLIST_FOLD //�г�Ŀ¼
}KILLLISTTYPE;//�б�����

class CKillListCtrl : public CListCtrl
{
// Construction
public:
	CKillListCtrl();

// Attributes
public:

    BOOL IfSupportFileDialogEx(void);
    void SetKillListType(KILLLISTTYPE killlisttype);
// Operations
private:
    KILLLISTTYPE KillListType;
    CToolTipCtrl m_ToolTip;
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKillListCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKillListCtrl();

	// Generated message map functions
protected:
    //{{AFX_MSG(CKillListCtrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KILLLISTCTRL_H__831015A3_032F_496B_A0BD_A245F231EFAA__INCLUDED_)
