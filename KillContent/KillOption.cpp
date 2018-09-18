// KillOption.cpp : implementation file
//

#include "stdafx.h"
#include "killcontent.h"
#include "KillOption.h"
#include "GenNotesList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static DWORD KillOption;
/////////////////////////////////////////////////////////////////////////////
// CKillOption dialog


CKillOption::CKillOption(CWnd* pParent /*=NULL*/)
	: CDialog(CKillOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKillOption)
	//}}AFX_DATA_INIT
}


void CKillOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKillOption)
	DDX_Control(pDX, IDC_STATIC_NOTE, m_State_Note);
	DDX_Control(pDX, IDCANCEL, m_Button_Cancel);
	DDX_Control(pDX, IDOK, m_Button_Ok);
	DDX_Control(pDX, IDC_STATIC_OPTION, m_Static_Option);
	DDX_Control(pDX, IDC_CHECK_FILEHEADER, m_FileHeader);
	DDX_Control(pDX, IDC_CHECK_FUNCHEADER, m_FuncHeader);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKillOption, CDialog)
	//{{AFX_MSG_MAP(CKillOption)
	ON_BN_CLICKED(IDC_RADIO_KILLALL, OnRadioKillall)
	ON_BN_CLICKED(IDC_RADIO_KILLPART, OnRadioKillpart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKillOption message handlers

BOOL CKillOption::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CButton	* pKillAll = ((CButton *)(GetDlgItem(IDC_RADIO_KILLALL)));
    CButton	* pKillSave = ((CButton *)(GetDlgItem(IDC_RADIO_KILLPART)));

    if (KillOption == KILLALL)
    {
        pKillAll->SetCheck(1);
        m_FuncHeader.EnableWindow(FALSE);
        m_FileHeader.EnableWindow(FALSE);
        m_State_Note.EnableWindow(FALSE);
    }
	if (KillOption & KILL_HOLDFILEHEADER)
    {
        pKillSave->SetCheck(1);
        m_FuncHeader.EnableWindow(TRUE);
        m_FileHeader.EnableWindow(TRUE);

        m_FileHeader.SetCheck(1);
    }
    if (KillOption & KILL_HOLDFUNCHEADER)
    {
        pKillSave->SetCheck(1);
        m_FuncHeader.EnableWindow(TRUE);
        m_FileHeader.EnableWindow(TRUE);

        m_FuncHeader.SetCheck(1);
    }
    SetText();
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKillOption::OnRadioKillall() 
{
	// TODO: Add your control notification handler code here
	m_FuncHeader.EnableWindow(FALSE);
    m_FileHeader.EnableWindow(FALSE);
    m_State_Note.EnableWindow(FALSE);
}

void CKillOption::OnRadioKillpart() 
{
	// TODO: Add your control notification handler code here
	m_FuncHeader.EnableWindow(TRUE);
    m_FileHeader.EnableWindow(TRUE);
    m_State_Note.EnableWindow(TRUE);
}

void CKillOption::OnOK() 
{
	// TODO: Add extra validation here
	CButton	* pKillAll = ((CButton *)(GetDlgItem(IDC_RADIO_KILLALL)));
    if (pKillAll->GetCheck() == 1)
    {
        KillOption = KILLALL;
    }
    else
    {
        int iFileHeader = m_FileHeader.GetCheck();
        int iFuncHeader = m_FuncHeader.GetCheck();

        if ((iFileHeader == 0) && (iFuncHeader == 0))
        {
            CString cNote,cClew;
            cNote.LoadString(IDS_STRING_OPTION_CLEW);
            cClew.LoadString(IDS_STRING_CLEW);

            MessageBox(cNote,cClew,MB_ICONASTERISK|MB_OK);
            m_FileHeader.SetFocus();
            return;
        }
        KillOption = 0;
        if (iFileHeader == 1)
            KillOption |= KILL_HOLDFILEHEADER;

        if (iFuncHeader == 1)
            KillOption |= KILL_HOLDFUNCHEADER;        
    }
	CDialog::OnOK();
}

DWORD GetKillOption(void)//获得删除类型
{
    return KillOption;
}

void CKillOption::SetKillOption(DWORD dKillOption)//设置删除类型
{
    KillOption = dKillOption;
}

void CKillOption::SetText(void)//设置文字
{
    CString cnote,caption,cStaticClew,cFileHeader,cFuncHeader,cRadioAll,cRadioPart,cOk,cCancel;

    caption.LoadString(IDS_STRING_DIALOG_OPTION);
    SetWindowText(caption);

    cStaticClew.LoadString(IDS_STRING_OPTION_SAVECLEW);
    m_Static_Option.SetWindowText(cStaticClew);

    cFileHeader.LoadString(IDS_STRING_OPTION_SAVEFILEHEADER);
    m_FileHeader.SetWindowText(cFileHeader);

    cFuncHeader.LoadString(IDS_STRING_OPTION_SAVEFUNCHEADER);
    m_FuncHeader.SetWindowText(cFuncHeader);

    cnote.LoadString(IDS_STRING_KILLNOTE);
    m_State_Note.SetWindowText(cnote);

    cRadioAll.LoadString(IDS_STRING_OPTION_KILLALL);
    cRadioPart.LoadString(IDS_STRING_OPTION_KILLPART);

    CButton	* pKillAll = ((CButton *)(GetDlgItem(IDC_RADIO_KILLALL)));
    CButton	* pKillPart = ((CButton *)(GetDlgItem(IDC_RADIO_KILLPART)));

    pKillAll->SetWindowText(cRadioAll);
    pKillPart->SetWindowText(cRadioPart);

    cOk.LoadString(IDS_STRING_OK);
    cCancel.LoadString(IDS_STRING_CANCEL);

    m_Button_Ok.SetWindowText(cOk);
    m_Button_Cancel.SetWindowText(cCancel);
}
