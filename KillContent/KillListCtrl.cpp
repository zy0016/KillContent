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

static const int iItemSourWidch = 300;//第一项(源文件部分)的宽
static const int iItemDesWidth  = 300;//第二项(输出位置)的宽
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
    //重新设置listctrl控件风格
	DWORD oldStyle = GetExtendedStyle();
	SetExtendedStyle(oldStyle |LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_GRIDLINES);
    
    //发送消息,初始化列表,添加第一项
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
#if 0//效果不好
		CFont cListFont;
		//cListFont.CreateFont(20,20,
		//CSize cs(100,100),ss;
		//ss = ApproximateViewRect(cs,-1);

		LOGFONT logfont;	//用于设置字体
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

		strcpy(logfont.lfFaceName,cFont);//宋体
		cListFont.CreateFontIndirect(&logfont);
		SetFont(&cListFont,TRUE);
#endif
        return TRUE;
	}
    POSITION pos = GetFirstSelectedItemPosition();
	int nSelectedItem = GetNextSelectedItem(pos);//获得列表框中当前选择的项目
    CString cFileSour,cFileDes;
    CFileAddEdit FAddEdit;
    
    if (nSelectedItem == -1)//没有选择项目
        nSelectedItem = 0;
    switch(wParam) 
    {
    case IDC_MENU_ADDFILES://添加项目  
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
    case IDC_MENU_DELETEFILE://删除项目
        if ((nSelectedItem != GetItemCount() - 1) && (nSelectedItem != -1))//删除一项
        {
            CString cFileSour,cDelClew,cClew,cue;
            cFileSour = GetItemText(nSelectedItem,0);

            cDelClew.LoadString(IDS_STRING_KILLLIST_DELETEFROMLIST);//"从列表中删除\"" + cFileSour + "\"是否继续?"
            cue.Format(cDelClew,cFileSour);
            cClew.LoadString(IDS_STRING_CLEW);

            if (MessageBox(cue,cClew,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDYES)
                DeleteItem(nSelectedItem);
        }
    	break;
    case IDC_MENU_MODIFY://修改项目
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

    case IDC_MENU_PREVIEW://预览代码
        POSITION pos = GetFirstSelectedItemPosition();
        int nSelectedItem = GetNextSelectedItem(pos);//获得列表框中当前选择的项目
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
            int nSelectedItem = GetNextSelectedItem(pos);//获得列表框中当前选择的项目
            CString cFileSour,cFileDes;
            CFileAddEdit FAddEdit;

            if (-1 != nSelectedItem)//没有选择项目
            {
                if (nSelectedItem == GetItemCount() - 1)//选择最后一个项目
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
                else//选择其他项目
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
	int nSelectedItem = GetNextSelectedItem(pos);//获得列表框中当前选择的项目
    CString cFileSour,cFileDes;
    CFileAddEdit FAddEdit;

    if (nSelectedItem != -1)
    {
        if (nSelectedItem == GetItemCount() - 1)//双击最后一项,需要添加项目
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
        else//需要编辑项目
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

//考察当前系统是否支持新的通用对话框
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
	int nSelectedItem = GetNextSelectedItem(pos);//获得列表框中当前选择的项目
    int iCount = GetItemCount();

    if (nChar == VK_DELETE)
    {
        if ((nSelectedItem != iCount - 1) && (nSelectedItem != -1))//删除一项
        {
            CString cFileSour,cDelClew,cClew,cue;
            cFileSour = GetItemText(nSelectedItem,0);

            cDelClew.LoadString(IDS_STRING_KILLLIST_DELETEFROMLIST);//"从列表中删除\"" + cFileSour + "\"是否继续?"
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

            cAdd.LoadString(IDS_STRING_MENUADD);//添加项目(&A)
            cModify.LoadString(IDS_STRING_MENUMODIFY);//修改项目(&M)
            csDelete.LoadString(IDS_STRING_MENUDELETE);//删除项目(&D)
            cPreview.LoadString(IDS_STRING_MENUPREVIEW);//预览代码(&P)

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
	        int nSelectedItem = GetNextSelectedItem(pos);//获得列表框中当前选择的项目
            int iCount = GetItemCount();

            if ((nSelectedItem == -1) || //没有选择项目
                (nSelectedItem == GetItemCount() - 1))//选择最后一个项目
            {
                EnableMenuItem(m_Menu,IDC_MENU_DELETEFILE,MF_GRAYED);
                EnableMenuItem(m_Menu,IDC_MENU_MODIFY,MF_GRAYED);
                EnableMenuItem(m_Menu,IDC_MENU_PREVIEW,MF_GRAYED);
            }
            m_Menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, cPoint.x, cPoint.y, this );
		}
	}
}

//设置列表控件显示的类型,显示文件还是显示目录
void CKillListCtrl::SetKillListType(KILLLISTTYPE killlisttype)
{
    CString column1,column2,cClewText1,cClewText2;
    KillListType = killlisttype;

    int iListCount = GetItemCount();
    switch (KillListType)
    {
    case KILLLIST_FILE:
        column1.LoadString(IDS_STRING_KILLLIST_FILECOLUMN1);//需要删除注释的文件
        column2.LoadString(IDS_STRING_KILLLIST_FILECOLUMN2);//删除注释之后的文件
        cClewText1.LoadString(IDS_STRING_KILLLIST_INPUTSOURFILE);//双击这里选择或者输入源文件名
        cClewText2.LoadString(IDS_STRING_KILLLIST_INPUTDESFILE);//双击这里选择或者输入目的文件名
        break;
    case KILLLIST_FOLD:
        column1.LoadString(IDS_STRING_KILLLIST_FOLDCOLUMN1);//需要删除注释的目录
        column2.LoadString(IDS_STRING_KILLLIST_FOLDCOLUMN2);//删除注释之后的目录
        cClewText1.LoadString(IDS_STRING_KILLLIST_INPUTSOURFOLD);//双击这里选择或者输入源目录
        cClewText2.LoadString(IDS_STRING_KILLLIST_INPUTDESFOLD);//双击这里选择或者输入目的目录
        break;
    }
    InsertColumn(0,column1,LVCFMT_LEFT,iItemSourWidch,0);
    InsertColumn(1,column2,LVCFMT_LEFT,iItemDesWidth,0);

    InsertItem(iListCount,cClewText1);
    SetItemText(iListCount,1,cClewText2);
}
