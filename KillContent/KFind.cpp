// KFind.cpp : implementation file
//

#include "stdafx.h"
#include "killcontent.h"
#include "KFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKFind dialog


CKFind::CKFind(CWnd* pParent /*=NULL*/)
	: CDialog(CKFind::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKFind)
	//}}AFX_DATA_INIT
}


void CKFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKFind)
	DDX_Control(pDX, IDC_BUTTON_FIND, m_BUTTON_Find);
	DDX_Control(pDX, IDC_STATIC_DIRECT, m_Static_Direct);
	DDX_Control(pDX, IDC_STATIC_FINDCONTENT, m_Static_FindContent);
	DDX_Control(pDX, IDC_CHECK_LU, m_Check_LU);
	DDX_Control(pDX, IDC_CHECK_ALL, m_Check_All);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_COMBO_FIND, m_Combo_Find);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKFind, CDialog)
	//{{AFX_MSG_MAP(CKFind)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnButtonFind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKFind message handlers

BOOL CKFind::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    CButton	* pButtonDown = ((CButton *)(GetDlgItem(IDC_RADIO_DOWN)));
    CButton	* pButtonup   = ((CButton *)(GetDlgItem(IDC_RADIO_UP  )));
    pButtonDown->SetCheck(1);
    m_Combo_Find.SetWindowText(cInitFindString);
    //////////////////////////////////////////////////////////////////////////
    CString caption,close,cfindcontent,cfinddirect,clu,call,cup,cdown;
    caption.LoadString(IDS_STRING_PREVIEW_FIND);
    SetWindowText(caption);
    m_BUTTON_Find.SetWindowText(caption);

    close.LoadString(IDS_STRING_CLOSE);
    m_Cancel.SetWindowText(close);

    cfindcontent.LoadString(IDS_STRING_FINDCONTENT);
    m_Static_FindContent.SetWindowText(cfindcontent);

    cfinddirect.LoadString(IDS_STRING_FINDDIRECT);
    m_Static_Direct.SetWindowText(cfinddirect);

    clu.LoadString(IDS_STRING_FIND_LU);
    m_Check_LU.SetWindowText(clu);

    call.LoadString(IDS_STRING_ALL);
    m_Check_All.SetWindowText(call);

    cup.LoadString(IDS_STRING_FIND_UP);
    pButtonup->SetWindowText(cup);

    cdown.LoadString(IDS_STRING_FIND_DOWN);
    pButtonDown->SetWindowText(cdown);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKFind::SetInitString(CString cInitString)//设置初始字符串
{
    cInitFindString = cInitString;
}

void CKFind::OnButtonFind() 
{
	// TODO: Add your control notification handler code here
    m_Combo_Find.GetWindowText(cFindString);
    CDialog::OnOK();
}

CString CKFind::GetFindString(void)//获得需要查找的字符串
{
    return cFindString;
}

CKFind::CKFind(CString cInitString,CWnd* pParent /*=NULL*/)
	: CDialog(CKFind::IDD, pParent)
{
	cInitFindString = cInitString;
}

BOOL CKFind::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
    return CDialog::DestroyWindow();
}
