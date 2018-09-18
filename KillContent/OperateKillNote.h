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

#define SPACESIGN                       _T(";")//��Ŀ�ָ��
#define SCANING_TIMER		            1001
#define FILESIZE                        _T("%ld")
typedef enum
{
    KILLFILES,  //�г��ļ�
    KILLPATHS   //�г�Ŀ¼
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
    DWORD dKillOption;//ɾ��ѡ��

    typedef enum
	{
		KILLNOTEING,//����ɾ��ע��
		STOPKILLNOTE//ֹͣɾ��ע��
	}KILLNOTESTATUS;
    KILLNOTESTATUS KillNoteStatus;//����״̬

    typedef enum
    {
        DETAIL, //��ʾϸ��
        NODETAIL//����ʾϸ��
    }WINDOWSTATUS;//����״̬
    WINDOWSTATUS WindowStatus;//����״̬
    RECT rDetail,rNoDetail;

    OPERATEKILLNOTE_TYPE OperateKillNote_Type;
    CString PauseClew;
    CString cSourceItem;//Դ�ļ���Ŀ¼���ĳ�ʼ����
    CString cIntentionItem;//Ŀ���ļ���Ŀ¼���ĳ�ʼ����
    CStringList ListFiles;//�ļ��б�
    //Դ�ļ�����Ŀ���ļ�,��ô��������һ����ʱ�ļ�,�ٽ�Դ�ļ�ɾ��,Ȼ����ʱ�ļ�����,
    //��ΪԴ�ļ���.��������һ������:���ɵ�Ŀ���ļ�����һ��������ʱ�򽫻ᱻ��������
    //����Ȼ�ǲ����ʵ�,������ɨ��ÿһ���ļ�֮��,��¼����ļ���,���Ƿ���������,
    //�ٴ�������ʱ��,��������ļ��Ƿ��Ѿ�ɨ���,ɨ����Ļ����������ļ�

    BOOL bOpenSourFile;//�Ƿ��Ѿ���Դ�ļ�
    BOOL bOpenDesFile;//�Ƿ��Ѿ���Ŀ���ļ�

    DWORD dSourFileNumber;//ɨ��Դ�ļ�����
    DWORD dDesFileNumber;//���ɵ�Ŀ���ļ�����
    DWORD dSanByte;//��¼ɨ����ֽ�
    DWORD dScanSingleFileTime;//ɨ��һ���ļ���ʱ
    int iUsedSecond;
	int iUsedMinute;
	int iUsedHour;
    int m_nTimer;
    
    RTL_CRITICAL_SECTION lpcritical_section;
    //DWORD dThread;//�̺߳�
    HANDLE hThread;
    static UINT KillNoteThread(LPVOID lpvData);
    BOOL KillNoteFile(CString cFileName,CString cDesFileName,DWORD dKillType);
    void SetText(void);//��������
    BOOL IsInListFiles(CString FileName);//���ļ��Ƿ�����������
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
