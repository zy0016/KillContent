#if !defined(AFX_KILLOPTION_H__876B6DF4_D3E3_4E44_A181_51BF9D149D4B__INCLUDED_)
#define AFX_KILLOPTION_H__876B6DF4_D3E3_4E44_A181_51BF9D149D4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KillOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKillOption dialog
//typedef enum
//{
//    KILLALL                 = 0x00, //ɾ������ע��
//    KILL_HOLDFILEHEADER     = 0x01, //�����ļ�ͷע��
//    KILL_HOLDFUNCHEADER     = 0x02, //��������ͷע��
//}KILLOPTION;
DWORD GetKillOption(void);
class CKillOption : public CDialog
{
// Construction
public:
	CKillOption(CWnd* pParent = NULL);   // standard constructor
    void SetKillOption(DWORD dKillOption);//����ɾ������
// Dialog Data
	//{{AFX_DATA(CKillOption)
	enum { IDD = IDD_DIALOG_OPTION };
	CStatic	m_State_Note;
	CButton	m_Button_Cancel;
	CButton	m_Button_Ok;
	CButton	m_Static_Option;
	CButton	m_FileHeader;
	CButton	m_FuncHeader;
	//}}AFX_DATA

private:
    void SetText(void);//��������
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKillOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKillOption)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioKillall();
	afx_msg void OnRadioKillpart();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KILLOPTION_H__876B6DF4_D3E3_4E44_A181_51BF9D149D4B__INCLUDED_)
