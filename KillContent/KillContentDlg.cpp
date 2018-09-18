// KillContentDlg.cpp : implementation file
//
/*
注释杀手alpha2.0版
注释杀手可以删除C/C++代码中的注释,并且支持保留其中某些部分的注释.
支持保留文件头注释,用于保留文件版本等信息,
支持保留函数头注释,用于保留函数的有关信息,
注释杀手支持UNICODE编码.
目前版本存在的主要问题是:
1.在代码预览界面对于两个预览控件的尺寸拖动,也就是类似于窗口分割的功能还处理的不好.
2.在代码预览界面里的查询功能还很不完善.
*/
#include "stdafx.h"
#include "KillContent.h"
#include "KillContentDlg.h"
#include "winuser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKillContentDlg dialog

CKillContentDlg::CKillContentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKillContentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKillContentDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKillContentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKillContentDlg)
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_Button_Close);
	DDX_Control(pDX, IDC_BUTTON_OPTION, m_Button_Option);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKillContentDlg, CDialog)
	//{{AFX_MSG_MAP(CKillContentDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, OnButtonOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKillContentDlg message handlers

BOOL CKillContentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here    
	CK_Sheet.AddPage(&C_File);
    CK_Sheet.AddPage(&C_Fold);
    
    //用Create来创建一个属性页
	CK_Sheet.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);

	//调整属性页的大小和位置
	CK_Sheet.SetWindowPos(NULL, 0,0,0,0 , SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

    TCHAR szBuf[256];
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //hins=AfxGetInstanceHandle();
    GetCurrentDirectory(sizeof(szBuf),szBuf);
    CurrentAppPath = szBuf;
    //GetModuleFileName(AfxGetInstanceHandle(),szBuf,sizeof(szBuf));//获得当前应用的具体名称,包括路径
    //GetModuleFileName(NULL,szBuf,sizeof(szBuf));//也可以    

    SetText();//设置文字

    KillOption = KILLALL;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKillContentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKillContentDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKillContentDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

CString GetCurrentAppPath(void)//返回当前应用所在路径
{
    return CurrentAppPath;
}

void CKillContentDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
    CDialog::OnCancel();
}

void CKillContentDlg::OnButtonOption() 
{
	// TODO: Add your control notification handler code here
	CKillOption cko;

    cko.SetKillOption(KillOption);
    if (cko.DoModal() == IDOK )
    {
        KillOption = GetKillOption();
    }
}

void CKillContentDlg::SetText(void)//设置文字
{
    CString caption;
    caption.LoadString(IDS_STRING_KILLCONTENT);
    SetWindowText(caption);//设置标题
    CString cOpetion;
    cOpetion.LoadString(IDS_STRING_OPTION);
    m_Button_Option.SetWindowText(cOpetion);
    CString cClose;
    cClose.LoadString(IDS_STRING_CLOSE);
    m_Button_Close.SetWindowText(cClose);
}
