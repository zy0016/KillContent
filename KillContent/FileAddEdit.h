#if !defined(AFX_FILEADDEDIT_H__DE8452D1_B022_462D_9E57_752A9A3A3FB3__INCLUDED_)
#define AFX_FILEADDEDIT_H__DE8452D1_B022_462D_9E57_752A9A3A3FB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileAddEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
#include "FileDialogEx.h"

extern CString GetCurrentAppPath(void);//返回当前应用所在路径
// CFileAddEdit dialog
typedef enum
{
    OPERATETYPE_ADD,//添加项目
    OPERATETYPE_EDIT//编辑项目
}OPERATETYPE;       //操作类型

typedef enum
{
    DIALOGTYPE_FILE,       //添加文件
    DIALOGTYPE_FOLD        //添加目录
}DIALOGTYPE;    //对话框类型

class CFileAddEdit : public CDialog
{
// Construction
public:
	CFileAddEdit(CWnd* pParent = NULL);   // standard constructor

    BOOL CFileOperateType(OPERATETYPE Operate_Type,//设置对话框风格
        DIALOGTYPE Dialog_Type,//对话框类型
        CString cFileSour ,CString cFileDes );
    
    void GetFileName(CString & cFileNameSour,CString & cFileNameDes);//获得源文件，目的文件
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
    OPERATETYPE OperateType;//当前操作类型
    DIALOGTYPE  DialogType;//对话框类型

    CString Item_Sour;//调用方获得的源文件(目录)
    CString Item_Des;//调用方获得的目的文件(目录)

    CString initSourFile_Fold;//编辑项目的时候，需要现实的源文件(目录)
    CString initDesFile_Fold;//编辑项目的时候，需要现实的目的文件(目录)
    void AddEditFile(CComboBox & combox);
    BOOL SaveFoldList(CComboBox & combox,CString sItem);//保存项目
    void ReadFoldList(CComboBox & combox);//读取项目列表
    void SetComboWidth(CComboBox & combox);//设置下拉列表部分的宽度

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
