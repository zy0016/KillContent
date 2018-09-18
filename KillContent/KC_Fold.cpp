// KC_Fold.cpp : implementation file
//

#include "stdafx.h"
#include "KillContent.h"
#include "KC_Fold.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKC_Fold property page

IMPLEMENT_DYNCREATE(CKC_Fold, CPropertyPage)

CKC_Fold::CKC_Fold() : CPropertyPage(CKC_Fold::IDD)
{
	//{{AFX_DATA_INIT(CKC_Fold)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKC_Fold::~CKC_Fold()
{
}

void CKC_Fold::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKC_Fold)
	DDX_Control(pDX, IDC_STATIC_FOLDCLEWBOX, m_Static_FoldClewBox);
	DDX_Control(pDX, IDC_STATIC_CLEWFOLD, m_Static_ClewFold);
	DDX_Control(pDX, IDC_BUTTON_DELETEFOLD, m_Button_Delete);
	DDX_Control(pDX, IDC_LIST_FOLD, m_FoldList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKC_Fold, CPropertyPage)
	//{{AFX_MSG_MAP(CKC_Fold)
	ON_BN_CLICKED(IDC_BUTTON_DELETEFOLD, OnButtonDeletefold)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKC_Fold message handlers

BOOL CKC_Fold::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	SetText();
	// TODO: Add extra initialization here
	m_FoldList.SetKillListType(KILLLIST_FOLD);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKC_Fold::OnButtonDeletefold() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_FoldList.GetFirstSelectedItemPosition();
    int nSelectedItem = m_FoldList.GetNextSelectedItem(pos);//获得列表框中当前选择的项目
    int iCount = m_FoldList.GetItemCount();

    if (iCount < 2)
        return;

    CString cSourItem = "",cDesItem = "";
    for (int i = 0;i < iCount - 1;i++)
    {
        cSourItem = cSourItem + m_FoldList.GetItemText(i,0) + SPACESIGN;
        cDesItem  = cDesItem  + m_FoldList.GetItemText(i,1) + SPACESIGN;
    }
    COperateKillNote cKillNote;

    if (cSourItem.Right(1) == SPACESIGN)
        cSourItem = cSourItem.Left(cSourItem.GetLength() - 1);
    if (cDesItem.Right(1) == SPACESIGN)
        cDesItem = cDesItem.Left(cDesItem.GetLength() - 1);

    cKillNote.SetOperateType(KILLPATHS,cSourItem,cDesItem);
    cKillNote.DoModal();//开始删除
}
void CKC_Fold::SetText(void)//设置文字
{
    CString caption,cClewBox,cFileClew,cBPreview,cBDelete;
    caption.LoadString(IDS_STRING_OPERATE_FOLD);
    SetWindowText(caption);

    cClewBox.LoadString(IDS_STRING_FOLD);
    m_Static_FoldClewBox.SetWindowText(cClewBox);

    cFileClew.LoadString(IDS_STRING_CLEWFOLD);
    m_Static_ClewFold.SetWindowText(cFileClew);

    cBDelete.LoadString(IDS_STRING_BEGINDELETE);
    m_Button_Delete.SetWindowText(cBDelete);
}
