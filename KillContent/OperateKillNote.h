#if !defined(AFX_OPERATEKILLNOTE_H__C22B525D_7ABC_4643_BAB5_5C1C334D3E80__INCLUDED_)
#define AFX_OPERATEKILLNOTE_H__C22B525D_7ABC_4643_BAB5_5C1C334D3E80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OperateKillNote.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COperateKillNote dialog
#include "GenNotesList.h"
#include "KillOption.h"

#define SPACESIGN                       _T(";")//项目分割符
#define SCANING_TIMER		            1001
#define FILESIZE                        _T("%ld")
typedef enum
{
    KILLFILES,  //列出文件
    KILLPATHS   //列出目录
}OPERATEKILLNOTE_TYPE;
class COperateKillNote : public CDialog
{
// Construction
public:
	COperateKillNote(CWnd* pParent = NULL);   // standard constructor

    BOOL SetOperateType(OPERATEKILLNOTE_TYPE operatekillnote_type,
        CString cSourItems,CString cDesItems);

    //BOOL CreateDirectoryFromString(CString csPath);
// Dialog Data
	//{{AFX_DATA(COperateKillNote)
	enum { IDD = IDD_DIALOG_KILLNOTE };
	CStatic	m_Static_DesFile;
	CStatic	m_Static_SourFile;
	CStatic	m_ScanTime;
	CListCtrl	m_DetailList;
	CButton	m_Cancel;
	CButton	m_Detail;
	CEdit	m_Edit_DesFile;
	CEdit	m_Edit_SourFile;
	//}}AFX_DATA

private:
    DWORD dKillOption;//删除选项

    typedef enum
	{
		KILLNOTEING,//正在删除注释
		STOPKILLNOTE//停止删除注释
	}KILLNOTESTATUS;
    KILLNOTESTATUS KillNoteStatus;//操作状态

    typedef enum
    {
        DETAIL, //显示细节
        NODETAIL//不显示细节
    }WINDOWSTATUS;//窗口状态
    WINDOWSTATUS WindowStatus;//窗口状态
    RECT rDetail,rNoDetail;

    OPERATEKILLNOTE_TYPE OperateKillNote_Type;
    CString PauseClew;
    CString cSourceItem;//源文件（目录）的初始内容
    CString cIntentionItem;//目的文件（目录）的初始内容
    CStringList ListFiles;//文件列表
    //源文件就是目的文件,那么首先生成一个临时文件,再将源文件删除,然后将临时文件改名,
    //改为源文件名.这样做有一个问题:生成的目的文件在下一次搜索的时候将会被搜索出来
    //这显然是不合适的,所以在扫描每一个文件之后,记录这个文件名,就是放在链表里,
    //再次搜索的时候,考察这个文件是否已经扫描过,扫描过的话就跳过该文件

    BOOL bOpenSourFile;//是否已经打开源文件
    BOOL bOpenDesFile;//是否已经打开目的文件

    DWORD dSourFileNumber;//扫描源文件个数
    DWORD dDesFileNumber;//生成的目的文件个数
    DWORD dSanByte;//纪录扫描的字节
    DWORD dScanSingleFileTime;//扫描一个文件耗时
    int iUsedSecond;
	int iUsedMinute;
	int iUsedHour;
    int m_nTimer;
    
    RTL_CRITICAL_SECTION lpcritical_section;
    //DWORD dThread;//线程号
    HANDLE hThread;
    static UINT KillNoteThread(LPVOID lpvData);
    BOOL KillNoteFile(CString cFileName,CString cDesFileName,DWORD dKillType);
    void SetText(void);//设置文字
    BOOL IsInListFiles(CString FileName);//该文件是否在例外项中
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COperateKillNote)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COperateKillNote)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnButtonDetail();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDblclkListDetail(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPERATEKILLNOTE_H__C22B525D_7ABC_4643_BAB5_5C1C334D3E80__INCLUDED_)
