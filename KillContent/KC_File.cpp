// KC_File.cpp : implementation file
//

#include "stdafx.h"
#include "KillContent.h"
#include "KC_File.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKC_File property page

IMPLEMENT_DYNCREATE(CKC_File, CPropertyPage)

CKC_File::CKC_File() : CPropertyPage(CKC_File::IDD)
{
	//{{AFX_DATA_INIT(CKC_File)
	//}}AFX_DATA_INIT
}

CKC_File::~CKC_File()
{
}

void CKC_File::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKC_File)
	DDX_Control(pDX, IDC_STATIC_CLEWBOX, m_Static_ClewBox);
	DDX_Control(pDX, IDC_STATIC_FILECLEW, m_Static_FileClew);
	DDX_Control(pDX, IDC_BUTTON_PREVIEW, m_Button_Preview);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_Button_Delete);
	DDX_Control(pDX, IDC_LIST_FILE, m_FileList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKC_File, CPropertyPage)
	//{{AFX_MSG_MAP(CKC_File)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, OnButtonPreview)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKC_File message handlers

void CKC_File::OnButtonPreview() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_FileList.GetFirstSelectedItemPosition();
    int nSelectedItem = m_FileList.GetNextSelectedItem(pos);//获得列表框中当前选择的项目
    int iCount = m_FileList.GetItemCount();

    if ((-1 == nSelectedItem) || (nSelectedItem == iCount - 1))
        return;

    CPreviewCode cpc;

    cpc.SetPreviewFileName(m_FileList.GetItemText(nSelectedItem,0),m_FileList.GetItemText(nSelectedItem,1));
    cpc.DoModal();
}

void CKC_File::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_FileList.GetFirstSelectedItemPosition();
    int nSelectedItem = m_FileList.GetNextSelectedItem(pos);//获得列表框中当前选择的项目
    int iCount = m_FileList.GetItemCount();

    if (iCount < 2)
        return;

    CString cSourItem = "",cDesItem = "";
    for (int i = 0;i < iCount - 1;i++)
    {
        cSourItem = cSourItem + m_FileList.GetItemText(i,0) + SPACESIGN;
        cDesItem = cDesItem + m_FileList.GetItemText(i,1) + SPACESIGN;
    }
    COperateKillNote cKillNote;

    if (cSourItem.Right(1) == SPACESIGN)
        cSourItem = cSourItem.Left(cSourItem.GetLength() - 1);
    if (cDesItem.Right(1) == SPACESIGN)
        cDesItem = cDesItem.Left(cDesItem.GetLength() - 1);

    cKillNote.SetOperateType(KILLFILES,cSourItem,cDesItem);
    cKillNote.DoModal();//开始删除    
}

BOOL CKC_File::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	SetText();
	// TODO: Add extra initialization here
	m_FileList.SetKillListType(KILLLIST_FILE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKC_File::SetText(void)//设置文字
{
    CString caption,cClewBox,cFileClew,cBPreview,cBDelete;
    caption.LoadString(IDS_STRING_OPERATE_FILE);
    SetWindowText(caption);

    cClewBox.LoadString(IDS_STRING_FILE);
    m_Static_ClewBox.SetWindowText(cClewBox);

    cFileClew.LoadString(IDS_STRING_CLEWFILE);
    m_Static_FileClew.SetWindowText(cFileClew);

    cBPreview.LoadString(IDS_STRING_PREVIEWCODE);
    m_Button_Preview.SetWindowText(cBPreview);

    cBDelete.LoadString(IDS_STRING_BEGINDELETE);
    m_Button_Delete.SetWindowText(cBDelete);
}
