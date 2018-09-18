// KillListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "KillContent.h"
#include "KillListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int iItemSourWidch = 300;//��һ��(Դ�ļ�����)�Ŀ�
static const int iItemDesWidth  = 300;//�ڶ���(���λ��)�Ŀ�
/////////////////////////////////////////////////////////////////////////////
// CKillListCtrl

CKillListCtrl::CKillListCtrl()
{
    KillListType = KILLLIST_FILE;
}

CKillListCtrl::~CKillListCtrl()
{
}


BEGIN_MESSAGE_MAP(CKillListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CKillListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKillListCtrl message handlers

void CKillListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
    //��������listctrl�ؼ����
	DWORD oldStyle = GetExtendedStyle();
	SetExtendedStyle(oldStyle |LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_GRIDLINES);
    
    //������Ϣ,��ʼ���б�,��ӵ�һ��
    PostMessage(WM_COMMAND,LISTCTRL_INIT_WPARAM,LISTCTRL_INIT_LPARAM);
    ////////////////////////////////////////////////////
	CListCtrl::PreSubclassWindow();

    RECT rListCtrl;
    GetClientRect(&rListCtrl);

    CString cClewToolTip;
    cClewToolTip.LoadString(IDS_STRING_MOUSECLEW);
    m_ToolTip.Create(this);
    m_ToolTip.AddTool(this,cClewToolTip, &rListCtrl, IDC_TOOLTIP_ID);
}

BOOL CKillListCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((wParam == LISTCTRL_INIT_WPARAM) && (lParam == LISTCTRL_INIT_LPARAM))
	{   
#if 0//Ч������
		CFont cListFont;
		//cListFont.CreateFont(20,20,
		//CSize cs(100,100),ss;
		//ss = ApproximateViewRect(cs,-1);

		LOGFONT logfont;	//������������
        CString cFont;

        cFont.LoadString(IDS_STRING_FONT_SONG);
		memset(&logfont,0x00,sizeof(LOGFONT));

		logfont.lfHeight	= 18;//64,96
		logfont.lfWidth		= 0;
		logfont.lfWeight	= FW_DONTCARE;
		logfont.lfItalic	= FALSE;
		logfont.lfUnderline = FALSE;
		logfont.lfStrikeOut = FALSE;
		logfont.lfCharSet	= GB2312_CHARSET;
		//logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		//logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		//logfont.lfQuality = DEFAULT_QUALITY;

		strcpy(logfont.lfFaceName,cFont);//����
		cListFont.CreateFontIndirect(&logfont);
		SetFont(&cListFont,TRUE);
#endif
        return TRUE;
	}
    POSITION pos = GetFirstSelectedItemPosition();
	int nSelectedItem = GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ
    CString cFileSour,cFileDes;
    CFileAddEdit FAddEdit;
    
    if (nSelectedItem == -1)//û��ѡ����Ŀ
        nSelectedItem = 0;
    switch(wParam) 
    {
    case IDC_MENU_ADDFILES://�����Ŀ  
        switch (KillListType)
        {
        case KILLLIST_FILE:
            FAddEdit.CFileOperateType(OPERATETYPE_ADD,DIALOGTYPE_FILE,"","");
            break;
        case KILLLIST_FOLD:
            FAddEdit.CFileOperateType(OPERATETYPE_ADD,DIALOGTYPE_FOLD,"","");
            break;
        }
        if (FAddEdit.DoModal() == IDOK)
        {
            FAddEdit.GetFileName(cFileSour,cFileDes);
            InsertItem(nSelectedItem,cFileSour);
            SetItemText(nSelectedItem,1,cFileDes);
        }
    	break;
    case IDC_MENU_DELETEFILE://ɾ����Ŀ
        if ((nSelectedItem != GetItemCount() - 1) && (nSelectedItem != -1))//ɾ��һ��
        {
            CString cFileSour,cDelClew,cClew,cue;
            cFileSour = GetItemText(nSelectedItem,0);

            cDelClew.LoadString(IDS_STRING_KILLLIST_DELETEFROMLIST);//"���б���ɾ��\"" + cFileSour + "\"�Ƿ����?"
            cue.Format(cDelClew,cFileSour);
            cClew.LoadString(IDS_STRING_CLEW);

            if (MessageBox(cue,cClew,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDYES)
                DeleteItem(nSelectedItem);
        }
    	break;
    case IDC_MENU_MODIFY://�޸���Ŀ
        switch (KillListType)
        {
        case KILLLIST_FILE:
            FAddEdit.CFileOperateType(OPERATETYPE_EDIT,DIALOGTYPE_FILE,
                GetItemText(nSelectedItem,0),GetItemText(nSelectedItem,1));
            break;
        case KILLLIST_FOLD:
            FAddEdit.CFileOperateType(OPERATETYPE_EDIT,DIALOGTYPE_FOLD,
                GetItemText(nSelectedItem,0),GetItemText(nSelectedItem,1));
            break;
        }        

        if (FAddEdit.DoModal() == IDOK)
        {
            FAddEdit.GetFileName(cFileSour,cFileDes);
            SetItemText(nSelectedItem,0,cFileSour);
            SetItemText(nSelectedItem,1,cFileDes);
        }
        break;

    case IDC_MENU_PREVIEW://Ԥ������
        POSITION pos = GetFirstSelectedItemPosition();
        int nSelectedItem = GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ
        int iCount = GetItemCount();
        
        if ((-1 == nSelectedItem) || (nSelectedItem == iCount - 1))
            return CListCtrl::OnCommand(wParam, lParam);
        
        CPreviewCode cpc;
        
        cpc.SetPreviewFileName(GetItemText(nSelectedItem,0),GetItemText(nSelectedItem,1));
        cpc.DoModal();
        break;
    }
    return CListCtrl::OnCommand(wParam, lParam);
}

BOOL CKillListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);
    if (pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN)
        {
            POSITION pos = GetFirstSelectedItemPosition();
            int nSelectedItem = GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ
            CString cFileSour,cFileDes;
            CFileAddEdit FAddEdit;

            if (-1 != nSelectedItem)//û��ѡ����Ŀ
            {
                if (nSelectedItem == GetItemCount() - 1)//ѡ�����һ����Ŀ
                {
                    switch (KillListType)
                    {
                    case KILLLIST_FILE:
                        FAddEdit.CFileOperateType(OPERATETYPE_ADD,DIALOGTYPE_FILE,"","");
                        break;
                    case KILLLIST_FOLD:
                        FAddEdit.CFileOperateType(OPERATETYPE_ADD,DIALOGTYPE_FOLD,"","");
                        break;
                    }                    
                    if (FAddEdit.DoModal() == IDOK)
                    {
                        FAddEdit.GetFileName(cFileSour,cFileDes);
                        InsertItem(nSelectedItem,cFileSour);
                        SetItemText(nSelectedItem,1,cFileDes);
                    }
                }
                else//ѡ��������Ŀ
                {
                    switch (KillListType)
                    {
                    case KILLLIST_FILE:
                        FAddEdit.CFileOperateType(OPERATETYPE_EDIT,DIALOGTYPE_FILE,
                            GetItemText(nSelectedItem,0),GetItemText(nSelectedItem,1));
                        break;
                    case KILLLIST_FOLD:
                        FAddEdit.CFileOperateType(OPERATETYPE_EDIT,DIALOGTYPE_FOLD,
                            GetItemText(nSelectedItem,0),GetItemText(nSelectedItem,1));
                        break;
                    }

                    if (FAddEdit.DoModal() == IDOK)
                    {
                        FAddEdit.GetFileName(cFileSour,cFileDes);
                        SetItemText(nSelectedItem,0,cFileSour);
                        SetItemText(nSelectedItem,1,cFileDes);
                    }
                }
                return TRUE;
            }
        }
    }
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CKillListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    POSITION pos = GetFirstSelectedItemPosition();
	int nSelectedItem = GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ
    CString cFileSour,cFileDes;
    CFileAddEdit FAddEdit;

    if (nSelectedItem != -1)
    {
        if (nSelectedItem == GetItemCount() - 1)//˫�����һ��,��Ҫ�����Ŀ
        {
            switch (KillListType)
            {
            case KILLLIST_FILE:
                FAddEdit.CFileOperateType(OPERATETYPE_ADD,DIALOGTYPE_FILE,"","");
                break;
            case KILLLIST_FOLD:
                FAddEdit.CFileOperateType(OPERATETYPE_ADD,DIALOGTYPE_FOLD,"","");
                break;
            }            
            if (FAddEdit.DoModal() == IDOK)
            {
                FAddEdit.GetFileName(cFileSour,cFileDes);
                InsertItem(nSelectedItem,cFileSour);
			    SetItemText(nSelectedItem,1,cFileDes);
            }
        }
        else//��Ҫ�༭��Ŀ
        {
            switch (KillListType)
            {
            case KILLLIST_FILE:
                FAddEdit.CFileOperateType(OPERATETYPE_EDIT,DIALOGTYPE_FILE,
                    GetItemText(nSelectedItem,0),GetItemText(nSelectedItem,1));
                break;
            case KILLLIST_FOLD:
                FAddEdit.CFileOperateType(OPERATETYPE_EDIT,DIALOGTYPE_FOLD,
                    GetItemText(nSelectedItem,0),GetItemText(nSelectedItem,1));
                break;
            }
            if (FAddEdit.DoModal() == IDOK)
            {
                FAddEdit.GetFileName(cFileSour,cFileDes);
                SetItemText(nSelectedItem,0,cFileSour);
			    SetItemText(nSelectedItem,1,cFileDes);
            }
        }
    }
    CListCtrl::OnLButtonDblClk(nFlags, point);
}

//���쵱ǰϵͳ�Ƿ�֧���µ�ͨ�öԻ���
BOOL CKillListCtrl::IfSupportFileDialogEx(void)
{
    OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure,
   // which is supported on Windows 2000.
   //
   // If that fails, try using the OSVERSIONINFO structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
   {
      // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
         return FALSE;
   }

   switch (osvi.dwPlatformId)
   {
      case VER_PLATFORM_WIN32_NT:
         if ( osvi.dwMajorVersion >= 5 )
            return TRUE;

         break;
   }
   return FALSE; 
}

void CKillListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	POSITION pos = GetFirstSelectedItemPosition();
	int nSelectedItem = GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ
    int iCount = GetItemCount();

    if (nChar == VK_DELETE)
    {
        if ((nSelectedItem != iCount - 1) && (nSelectedItem != -1))//ɾ��һ��
        {
            CString cFileSour,cDelClew,cClew,cue;
            cFileSour = GetItemText(nSelectedItem,0);

            cDelClew.LoadString(IDS_STRING_KILLLIST_DELETEFROMLIST);//"���б���ɾ��\"" + cFileSour + "\"�Ƿ����?"
            cue.Format(cDelClew,cFileSour);
            cClew.LoadString(IDS_STRING_CLEW);

            if (MessageBox(cue,cClew,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
            {
                return;
            }
            DeleteItem(nSelectedItem);
        }
    }
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CKillListCtrl::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This block was added by the Pop-up Menu component
	{
        CRect rect;
	    GetClientRect(rect);
        ClientToScreen(rect);
		if (point.x == -1 && point.y == -1)
		{
			//keystroke invocation						
			point = rect.TopLeft();
			point.Offset(5, 5);
		}
        
		CPoint cPoint = point;

		{
            CMenu m_Menu;
            CBitmap	cAddFiles,cDelete;
            CString cAdd,cModify,csDelete,cPreview;

            cAdd.LoadString(IDS_STRING_MENUADD);//�����Ŀ(&A)
            cModify.LoadString(IDS_STRING_MENUMODIFY);//�޸���Ŀ(&M)
            csDelete.LoadString(IDS_STRING_MENUDELETE);//ɾ����Ŀ(&D)
            cPreview.LoadString(IDS_STRING_MENUPREVIEW);//Ԥ������(&P)

            cAddFiles.LoadBitmap(IDB_BITMAP_NEW);
            cDelete.LoadBitmap(IDB_BITMAP_DELETE);
            
            m_Menu.CreatePopupMenu();
            m_Menu.AppendMenu( MF_STRING, IDC_MENU_ADDFILES, cAdd);
            m_Menu.AppendMenu( MF_STRING, IDC_MENU_MODIFY,cModify);
            m_Menu.AppendMenu( MF_STRING, IDC_MENU_DELETEFILE,csDelete);
            
            if (KillListType == KILLLIST_FILE)
            {
                m_Menu.AppendMenu(MF_SEPARATOR,0);
                m_Menu.AppendMenu( MF_STRING, IDC_MENU_PREVIEW,cPreview);
            }
            m_Menu.SetMenuItemBitmaps(IDC_MENU_ADDFILES,MF_BYCOMMAND,&cAddFiles,&cAddFiles);
            m_Menu.SetMenuItemBitmaps(IDC_MENU_DELETEFILE,MF_BYCOMMAND,&cDelete,&cDelete);

            POSITION pos = GetFirstSelectedItemPosition();
	        int nSelectedItem = GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ
            int iCount = GetItemCount();

            if ((nSelectedItem == -1) || //û��ѡ����Ŀ
                (nSelectedItem == GetItemCount() - 1))//ѡ�����һ����Ŀ
            {
                EnableMenuItem(m_Menu,IDC_MENU_DELETEFILE,MF_GRAYED);
                EnableMenuItem(m_Menu,IDC_MENU_MODIFY,MF_GRAYED);
                EnableMenuItem(m_Menu,IDC_MENU_PREVIEW,MF_GRAYED);
            }
            m_Menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, cPoint.x, cPoint.y, this );
		}
	}
}

//�����б�ؼ���ʾ������,��ʾ�ļ�������ʾĿ¼
void CKillListCtrl::SetKillListType(KILLLISTTYPE killlisttype)
{
    CString column1,column2,cClewText1,cClewText2;
    KillListType = killlisttype;

    int iListCount = GetItemCount();
    switch (KillListType)
    {
    case KILLLIST_FILE:
        column1.LoadString(IDS_STRING_KILLLIST_FILECOLUMN1);//��Ҫɾ��ע�͵��ļ�
        column2.LoadString(IDS_STRING_KILLLIST_FILECOLUMN2);//ɾ��ע��֮����ļ�
        cClewText1.LoadString(IDS_STRING_KILLLIST_INPUTSOURFILE);//˫������ѡ���������Դ�ļ���
        cClewText2.LoadString(IDS_STRING_KILLLIST_INPUTDESFILE);//˫������ѡ���������Ŀ���ļ���
        break;
    case KILLLIST_FOLD:
        column1.LoadString(IDS_STRING_KILLLIST_FOLDCOLUMN1);//��Ҫɾ��ע�͵�Ŀ¼
        column2.LoadString(IDS_STRING_KILLLIST_FOLDCOLUMN2);//ɾ��ע��֮���Ŀ¼
        cClewText1.LoadString(IDS_STRING_KILLLIST_INPUTSOURFOLD);//˫������ѡ���������ԴĿ¼
        cClewText2.LoadString(IDS_STRING_KILLLIST_INPUTDESFOLD);//˫������ѡ���������Ŀ��Ŀ¼
        break;
    }
    InsertColumn(0,column1,LVCFMT_LEFT,iItemSourWidch,0);
    InsertColumn(1,column2,LVCFMT_LEFT,iItemDesWidth,0);

    InsertItem(iListCount,cClewText1);
    SetItemText(iListCount,1,cClewText2);
}
