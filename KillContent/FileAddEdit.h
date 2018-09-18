#if !defined(AFX_FILEADDEDIT_H__DE8452D1_B022_462D_9E57_752A9A3A3FB3__INCLUDED_)
#define AFX_FILEADDEDIT_H__DE8452D1_B022_462D_9E57_752A9A3A3FB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileAddEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
#include "FileDialogEx.h"

extern CString GetCurrentAppPath(void);//���ص�ǰӦ������·��
// CFileAddEdit dialog
typedef enum
{
    OPERATETYPE_ADD,//�����Ŀ
    OPERATETYPE_EDIT//�༭��Ŀ
}OPERATETYPE;       //��������

typedef enum
{
    DIALOGTYPE_FILE,       //����ļ�
    DIALOGTYPE_FOLD        //���Ŀ¼
}DIALOGTYPE;    //�Ի�������

class CFileAddEdit : public CDialog
{
// Construction
public:
	CFileAddEdit(CWnd* pParent = NULL);   // standard constructor

    BOOL CFileOperateType(OPERATETYPE Operate_Type,//���öԻ�����
        DIALOGTYPE Dialog_Type,//�Ի�������
        CString cFileSour ,CString cFileDes );
    
    void GetFileName(CString & cFileNameSour,CString & cFileNameDes);//���Դ�ļ���Ŀ���ļ�
// Dialog Data
	//{{AFX_DATA(CFileAddEdit)
	enum { IDD = IDD_DIALOG_ADD_EDITFILE };
	CButton	m_Button_Cancel;
	CButton	m_Button_OK;
	CStatic	m_Static_Des;
	CStatic	m_Static_Sour;
	CComboBox	m_ComDes;
	CComboBox	m_ComSour;
	//}}AFX_DATA

private:
    OPERATETYPE OperateType;//��ǰ��������
    DIALOGTYPE  DialogType;//�Ի�������

    CString Item_Sour;//���÷���õ�Դ�ļ�(Ŀ¼)
    CString Item_Des;//���÷���õ�Ŀ���ļ�(Ŀ¼)

    CString initSourFile_Fold;//�༭��Ŀ��ʱ����Ҫ��ʵ��Դ�ļ�(Ŀ¼)
    CString initDesFile_Fold;//�༭��Ŀ��ʱ����Ҫ��ʵ��Ŀ���ļ�(Ŀ¼)
    void AddEditFile(CComboBox & combox);
    BOOL SaveFoldList(CComboBox & combox,CString sItem);//������Ŀ
    void ReadFoldList(CComboBox & combox);//��ȡ��Ŀ�б�
    void SetComboWidth(CComboBox & combox);//���������б��ֵĿ��

    static int CALLBACK SetSelProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileAddEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileAddEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSourfile();
	afx_msg void OnButtonDesfile();
	virtual void OnOK();
	afx_msg void OnDropdownComboDes();
	afx_msg void OnDropdownComboSour();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEADDEDIT_H__DE8452D1_B022_462D_9E57_752A9A3A3FB3__INCLUDED_)
