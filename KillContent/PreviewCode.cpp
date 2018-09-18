 // PreviewCode.cpp : implementation file
//
#include "stdafx.h"
#include "KillContent.h"
#include "PreviewCode.h"
#include <ATLCONV.H>
#include <RICHEDIT.H>
#include "AFX.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
static BOOL bLbuttonKeydown;//鼠标左键是否已经按下
static const CString cTempExtName = _T(".tmp");
extern CString GetCurrentAppPath (void);
/////////////////////////////////////////////////////////////////////////////
// CPreviewCode dialog


CPreviewCode::CPreviewCode(CWnd* pParent /*=NULL*/)
	: CDialog(CPreviewCode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreviewCode)
	//}}AFX_DATA_INIT
    cSourFileName = "";
    cDesFileName  = "";
    dKillOption = KILLALL;
}


void CPreviewCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreviewCode)
	DDX_Control(pDX, IDC_EDIT_RESULT, m_File_Result);
	DDX_Control(pDX, IDC_EDIT_PREVIEW, m_File_Preview);
	DDX_Control(pDX, IDC_STATIC_SPLITTER, m_Static_Splitter);
	DDX_Control(pDX, IDC_RICHEDIT_RESULT, m_Result);
	DDX_Control(pDX, IDC_RICHEDIT_PREVIEW, m_Preview);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreviewCode, CDialog)
	//{{AFX_MSG_MAP(CPreviewCode)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_BUTTON_PREVIEW, OnButtonPreview)
    ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipText )
	ON_COMMAND(ID_BUTTON_FIND, OnButtonFind)
	ON_COMMAND(ID_BUTTON_PREVIEW_ALL, OnButtonPreviewAll)
	ON_COMMAND(ID_BUTTON_PREVIEW_HEAD, OnButtonPreviewHead)
	ON_COMMAND(ID_BUTTON_PREVIEW_FUNC, OnButtonPreviewFunc)
	ON_COMMAND(ID_BUTTON_CLOSE, OnButtonClose)
	ON_COMMAND(ID_BUTTON_PREVIEW_SAVE, OnButtonPreviewSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewCode message handlers

void CPreviewCode::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW)
		return;		// arrangement not needed
    
    if (m_Preview.GetSafeHwnd() == NULL)
        return;

    RECT rect,rPreviewRect,rResultRect,rSplitterRect,rFilePreviewRect,rFileResultRect;
	GetClientRect(&rect);//窗口大小
    //////////////////////////////////////////////////////////////////////////
    int iSplitterWidth;
    m_Static_Splitter.GetWindowRect(&rSplitterRect);
    iSplitterWidth = rSplitterRect.right - rSplitterRect.left;
    ScreenToClient(&rSplitterRect);
    rSplitterRect.left   = (rect.right - rect.left) / 2;
    rSplitterRect.right  = rSplitterRect.left + iSplitterWidth;
    rSplitterRect.bottom = rect.bottom;
    m_Static_Splitter.MoveWindow(&rSplitterRect);
    //////////////////////////////////////////////////////////////////////////    
    m_Preview.GetWindowRect(&rPreviewRect);
    ScreenToClient(&rPreviewRect);

    rPreviewRect.left   = rect.left;
    rPreviewRect.right  = rSplitterRect.left;//rect.right / 2;
    rPreviewRect.bottom = rect.bottom;

    m_Preview.MoveWindow(&rPreviewRect);
    //////////////////////////////////////////////////////////////////////////    
    m_Result.GetWindowRect(&rResultRect);
    ScreenToClient(&rResultRect);
    rResultRect.left    = rSplitterRect.right;//rPreviewRect.right;
    rResultRect.right   = rect.right;
    rResultRect.bottom  = rect.bottom;
    m_Result.MoveWindow(&rResultRect);
    //////////////////////////////////////////////////////////////////////////
    int iEditHeight;
    m_File_Preview.GetWindowRect(&rFilePreviewRect);
    iEditHeight = rFilePreviewRect.bottom - rFilePreviewRect.top;
    rFilePreviewRect.left   = rect.left;
    rFilePreviewRect.right  = rPreviewRect.right;
    rFilePreviewRect.bottom = rPreviewRect.top - 1;
    rFilePreviewRect.top    = rFilePreviewRect.bottom - iEditHeight;
    m_File_Preview.MoveWindow(&rFilePreviewRect);
    //////////////////////////////////////////////////////////////////////////
    m_File_Result.GetWindowRect(&rFileResultRect);
    rFileResultRect.top     = rFilePreviewRect.top;
    rFileResultRect.left    = rResultRect.left;
    rFileResultRect.right   = rResultRect.right;
    rFileResultRect.bottom  = rFilePreviewRect.bottom;
    m_File_Result.MoveWindow(&rFileResultRect);
}

void CPreviewCode::SetPreviewFileName(CString sourfilename,CString desfilename)
{
    cSourFileName = sourfilename;
    cDesFileName = desfilename;
}

BOOL CPreviewCode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CFile	file;

    CWaitCursor wait;
    if (!file.Open(cSourFileName,CFile::modeRead,NULL))//打开失败
    {
        CString csour,copenfail,csztext,caption,format;
        csour.LoadString(IDS_STRING_SOURFILE);
        copenfail.LoadString(IDS_STRING_OPENFAIL);
        format.LoadString(IDS_STRING_PREVIEW_1);
        csztext.Format(format,csour,cSourFileName,copenfail);
        caption.LoadString(IDS_STRING_CLEW);

        MessageBox(csztext,caption,MB_ICONASTERISK |MB_OK);        
        return TRUE;
    }

    m_File_Preview.SetWindowText(cSourFileName);
    m_File_Result.SetWindowText(cDesFileName);
    DWORD filesize = file.GetLength();
    CString cFileContent = "";
    LPTSTR lptstr;

    lptstr = new TCHAR[2 * filesize + 1];
    memset(lptstr,0x00,2 * filesize + 1);

//    file.ReadHuge(lptstr,filesize);////remove for vs 2010
    file.Read(lptstr,filesize);
    file.Close();

    if (lptstr[0] == 0xFEFF)
    {
        cFileContent = lptstr + 1;
    }
    else
    {
        MultiByteToWideChar(936,MB_PRECOMPOSED,(LPCSTR)lptstr,-1,
            cFileContent.GetBuffer(filesize),filesize);
        cFileContent.ReleaseBuffer();
    }
    m_Preview.SetWindowText(cFileContent);

    delete lptstr;

    CString cFont;
    cFont.LoadString(IDS_STRING_FONT_SONG);

    memset(&chFormat,0x00,sizeof(CHARFORMAT));
	chFormat.cbSize			= sizeof(CHARFORMAT);
//	chFormat.dwMask			= CFM_COLOR|CFM_FACE|CFM_BOLD|CFM_ITALIC|CFM_SIZE|CFM_UNDERLINE|CFM_STRIKEOUT;
	chFormat.dwMask			= CFM_COLOR|CFM_FACE|CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_STRIKEOUT;
	chFormat.dwEffects		&= ~CFE_AUTOCOLOR;
	chFormat.crTextColor	= RGB(0,128,0);
    _tcscpy(chFormat.szFaceName, cFont.GetBuffer(cFont.GetLength()));
    cFont.ReleaseBuffer();

    SetCommentColor(cSourFileName,m_Preview);//设置颜色

    cFileContent = GetNoteStringUnicode(cSourFileName,dKillOption,TRUE,BACK_UNICODE,NULL,0);
    m_Result.SetWindowText(cFileContent);//显示从一个文件中删除注释的结果

    cTempFile = CreateTempFile(cSourFileName,cFileContent);
    //SetCommentColor(cTempFile,m_Result);//设置颜色
    //////////////////////////////////////////////////////////////////////////
    SetText();

    /////创建工具条/////////////////////////////////////////////////////////////////////
    if(!cPreToolBar.CreateEx(this,TBSTYLE_FLAT,WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS) || 
        !cPreToolBar.LoadToolBar(IDR_TOOLBAR_PREVIEW))
    {
        TRACE0("Failed to Create Dialog Toolbar\n");
        EndDialog(IDCANCEL);
    }
    CRect	rcClientOld; // 旧客户区RECT
    CRect	rcClientNew; // 加入TOOLBAR后的CLIENT RECT
    GetClientRect(rcClientOld); // 
    // Called to reposition and resize control bars in the client area of a window
    // The reposQuery FLAG does not really traw the Toolbar.  It only does the calculations.
    // And puts the new ClientRect values in rcClientNew so we can do the rest of the Math.
    //重新计算RECT大小
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0,
        reposQuery,rcClientNew);

    // All of the Child Windows (Controls) now need to be moved so the Tollbar does not cover them up.
    //所有的子窗口将被移动，以免被TOOLBAR覆盖
    // Offest to move all child controls after adding Tollbar
    //计算移动的距离
    CPoint ptOffset(rcClientNew.left-rcClientOld.left,10);//rcClientNew.top-rcClientOld.top

    CRect	rcChild;
    CWnd*	pwndChild = GetWindow(GW_CHILD);  //得到子窗口
    while(pwndChild) // 处理所有子窗口
    {//移动所有子窗口
        pwndChild->GetWindowRect(rcChild);
        ScreenToClient(rcChild); 
        rcChild.OffsetRect(ptOffset); 
        pwndChild->MoveWindow(rcChild,FALSE); 
        pwndChild = pwndChild->GetNextWindow();
    }

    CRect	rcWindow;
    GetWindowRect(rcWindow); // 得到对话框RECT
    rcWindow.right += rcClientOld.Width() - rcClientNew.Width(); // 修改对话框尺寸
    rcWindow.bottom += rcClientOld.Height() - rcClientNew.Height(); 
    MoveWindow(rcWindow,FALSE); // Redraw Window
    
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);

    //cPreToolBar.SetBarStyle(cPreToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY |TBSTYLE_FLAT);//CBRS_SIZE_DYNAMIC
    cPreToolBar.ModifyStyle(0, TBSTYLE_FLAT|CBRS_TOOLTIPS);
    //cPreToolBar.SetButtonStyle(2,TBBS_CHECKBOX|TBBS_PRESSED);
    cPreToolBar.SetButtonStyle(2,TBBS_CHECKBOX|TBBS_CHECKED);
    cPreToolBar.SetButtonStyle(3,TBBS_CHECKBOX);
    cPreToolBar.SetButtonStyle(4,TBBS_CHECKBOX);

    //////////////////////////////////////////////////////////////////////////
    RECT rect;
    POINT pMax,pMin;
	GetClientRect(&rect);//窗口大小

    pMax.x = GetSystemMetrics(SM_CXSCREEN);
    pMax.y = GetSystemMetrics(SM_CYSCREEN);

    pMin.x = rect.right;
    pMin.y = rect.bottom;

    WINDOWPLACEMENT WindowPlaceMent;
    memset(&WindowPlaceMent,0x00,sizeof(WINDOWPLACEMENT));
    WindowPlaceMent.length = sizeof(WindowPlaceMent);
    WindowPlaceMent.flags = WPF_RESTORETOMAXIMIZED;//WPF_SETMINPOSITION;
    WindowPlaceMent.showCmd = SW_SHOWMAXIMIZED;
    WindowPlaceMent.rcNormalPosition = rect;
    WindowPlaceMent.ptMaxPosition = pMax;
    WindowPlaceMent.ptMinPosition = pMin;
    SetWindowPlacement(&WindowPlaceMent);
    //////////////////////////////////////////////////////////////////////////
    SetTimer(PREVIEWCODE_TIMER,50,NULL);
    bLbuttonKeydown = FALSE;

    m_Static_Splitter.SetFocus();

    return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPreviewCode::SetCommentColor(CString cfilename,CRichEditCtrl & richedit)//设置颜色
{
	struct strNotesList *NotesList = NULL, *p = NULL;

//    GetNotesList(cfilename,&NotesList);
    GetNotesList_Unicode(cfilename,&NotesList);

	if (NotesList)
    {
        while (NotesList->NextNote)
		{
			p = NotesList;
			NotesList = NotesList->NextNote;

            richedit.SetSel(p->LocStart,p->LocEnd);
//            richedit.SetSelectionCharFormat(chFormat);//jieping
			richedit.SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&chFormat);

			delete p;
		}
        richedit.SetSel(NotesList->LocStart,NotesList->LocEnd);
//        richedit.SetSelectionCharFormat(chFormat);//jieping
		richedit.SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&chFormat);

        delete NotesList;
    }
    return TRUE;
}

void CPreviewCode::SetText(void)//设置文字
{
    CString cCaption;
    cCaption.LoadString(IDS_STRING_PREVIEW);
    SetWindowText(cCaption);
    CString cClose;
    cClose.LoadString(IDS_STRING_CLOSE);
}

void CPreviewCode::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    int iVisibleLine,iCurLineNum;

    iCurLineNum  = m_Result.GetFirstVisibleLine();
    iVisibleLine = m_Preview.GetFirstVisibleLine();		//获得可见行
    if (iCurLineNum == iVisibleLine)
        return;

    POINT curPoint;
    GetCursorPos(&curPoint);//获得当前光标位置,根据光标的位置判断用户正在拖动哪个滚动条
    RECT rSplitterRect;
    m_Static_Splitter.GetWindowRect(&rSplitterRect);

    if (curPoint.x < rSplitterRect.left)
        m_Result.LineScroll(iVisibleLine - iCurLineNum,0);//滚动到该行
    else
        m_Preview.LineScroll(iCurLineNum - iVisibleLine,0);//滚动到该行

	CDialog::OnTimer(nIDEvent);
}

void CPreviewCode::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	RECT rect,rSplitterRect,rPreviewRect,rResultRect;
    m_Static_Splitter.GetWindowRect(&rSplitterRect);
    if (!bLbuttonKeydown)
    {
        if ((rSplitterRect.left <= point.x) && (point.x <= rSplitterRect.right) &&
            (rSplitterRect.top <= point.y) && (point.y <= rSplitterRect.bottom))
        {
            SetCursor(LoadCursor(NULL, IDC_SIZEWE));
        }
    }
    else
    {
        GetClientRect(&rect);//窗口大小

        int iSplitterWidth;
        iSplitterWidth = rSplitterRect.right - rSplitterRect.left;
        ScreenToClient(&rSplitterRect);
        rSplitterRect.left  = point.x;
        rSplitterRect.right = rSplitterRect.left + iSplitterWidth;
        m_Static_Splitter.MoveWindow(&rSplitterRect);
        //////////////////////////////////////////////////////////////////////////
        m_Preview.GetWindowRect(&rPreviewRect);
        ScreenToClient(&rPreviewRect);

        rPreviewRect.left   = rect.left;
        rPreviewRect.right  = rSplitterRect.left;

        m_Preview.MoveWindow(&rPreviewRect);
        //////////////////////////////////////////////////////////////////////////    
        m_Result.GetWindowRect(&rResultRect);
        ScreenToClient(&rResultRect);
        rResultRect.left    = rSplitterRect.right;
        rResultRect.right   = rect.right;
        m_Result.MoveWindow(&rResultRect);
    }
	CDialog::OnMouseMove(nFlags, point);
}

void CPreviewCode::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	RECT rSplitterRect;
    m_Static_Splitter.GetWindowRect(&rSplitterRect);
    if ((rSplitterRect.left <= point.x) && (point.x <= rSplitterRect.right) &&
        (rSplitterRect.top <= point.y) && (point.y <= rSplitterRect.bottom))
    {
        bLbuttonKeydown = TRUE;
        SetCursor(LoadCursor(NULL, IDC_SIZEWE));
    }
	CDialog::OnLButtonDown(nFlags, point);
}

void CPreviewCode::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    bLbuttonKeydown = FALSE;
	CDialog::OnLButtonUp(nFlags, point);
}

void CPreviewCode::OnButtonPreview() 
{
	// TODO: Add your command handler code here
    CDialog::OnCancel();
}

//工具栏提示
BOOL CPreviewCode::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);
	// UNICODE消息
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = pNMHDR->idFrom;
	
    switch (nID)
    {
    case ID_BUTTON_CLOSE:
        strTipText.LoadString(IDS_STRING_CLOSE);//关闭
        break;
    case ID_BUTTON_FIND:
        strTipText.LoadString(IDS_STRING_PREVIEW_FIND);//查找
        break;
    case ID_BUTTON_PREVIEW_ALL:
        strTipText.LoadString(IDS_STRING_OPTION_KILLALL);//删除所有注释
        break;
    case ID_BUTTON_PREVIEW_HEAD:
        strTipText.LoadString(IDS_STRING_OPTION_SAVEFILEHEADER);//保留文件头注释
        break;
    case ID_BUTTON_PREVIEW_FUNC:
        strTipText.LoadString(IDS_STRING_OPTION_SAVEFUNCHEADER);//保留函数头注释
        break;
    case ID_BUTTON_PREVIEW_SAVE:
        strTipText.LoadString(IDS_STRING_OUTPUTFILE);//按照当前方式保存
        break;
    default:
        break;
    }
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{   // idFrom为工具条的HWND 
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	if (nID != 0) //不为分隔符
	{
#ifndef _UNICODE
		if (pNMHDR->code == TTN_NEEDTEXTA)
			lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		else
			_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
#else
		if (pNMHDR->code == TTN_NEEDTEXTA)
			_wcstombsz(pTTTA->szText, (LPCTSTR)strTipText,sizeof(pTTTA->szText));
		else
			lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
#endif
		*pResult = 0;
		// 使工具条提示窗口在最上面
		::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,SWP_NOACTIVATE|
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER); 
		return TRUE;
	}
	return TRUE;
}

//CKFind ckfind;
void CPreviewCode::OnButtonFind() 
{
	// TODO: Add your command handler code here
//    ckfind.Create(IDD_DIALOG_FIND,this);
//    ckfind.ShowWindow(SW_SHOW);
	//CFindReplaceDialog* findDlg = new CFindReplaceDialog;

    CString strFind = "";
    //strFind = this->GetSelText();
    //findDlg->Create(TRUE, (LPCSTR)strFind, NULL, FR_DOWN, this);

    CRichEditCtrl * p_Prevew = ((CRichEditCtrl *)(GetDlgItem(IDC_RICHEDIT_PREVIEW)));
    CRichEditCtrl * p_Result = ((CRichEditCtrl *)(GetDlgItem(IDC_RICHEDIT_RESULT )));
    CRichEditCtrl * p_RichEditCtrl;
    if (GetFocus() == p_Prevew)
        p_RichEditCtrl = &m_Preview;
    else if (GetFocus() == p_Result)
        p_RichEditCtrl = &m_Result;
    else
        return;

    strFind = p_RichEditCtrl->GetSelText();
    CKFind ckfind(strFind);
    //ckfind.SetInitString(strFind);

    if (ckfind.DoModal() == IDCANCEL)
        return;

    if ((strFind = ckfind.GetFindString()) == "")
        return;

    FINDTEXTEX findtextex;
    memset(&findtextex,0x00,sizeof(FINDTEXTEX));
    findtextex.lpstrText = strFind.GetBuffer(strFind.GetLength());//jieping
	p_RichEditCtrl->GetSel(findtextex.chrg);
	findtextex.chrg.cpMin = findtextex.chrg.cpMax;
	findtextex.chrg.cpMax = -1;
	findtextex.chrgText.cpMax = -1;
	findtextex.chrgText.cpMin = 0;
//    long lFindText = p_RichEditCtrl->FindText(FR_MATCHCASE,&findtextex);
	p_RichEditCtrl->SendMessage(EM_FINDTEXTEX, (WPARAM) (UINT) SCF_SELECTION, (LPARAM)&findtextex);
    strFind.ReleaseBuffer();
	p_RichEditCtrl->SetSel(findtextex.chrgText);

//FINDREPLACE fr;       // common dialog box structure
//HWND hwnd;            // owner window
//CHAR szFindWhat[80];  // buffer receiving string
//HWND hdlg = NULL;     // handle to Find dialog box
//
//UINT uFindReplaceMsg;  // message identifier for FINDMSGSTRING 
//
//uFindReplaceMsg = RegisterWindowMessage(FINDMSGSTRING);
//
//// Initialize FINDREPLACE
//ZeroMemory(&fr, sizeof(FINDREPLACE));
//fr.lStructSize = sizeof(FINDREPLACE);
//fr.hwndOwner = p_RichEditCtrl->GetSafeHwnd();
//fr.lpstrFindWhat = szFindWhat;
//fr.wFindWhatLen = 80;
//fr.Flags = 0;
//
//hdlg = FindText(&fr);
}

void CPreviewCode::OnButtonPreviewAll() 
{
	// TODO: Add your command handler code here
	//删除全部注释
    CString     cFileContent;
    CWaitCursor wait;
    UINT        dwStyle;

    /*if (dKillOption == KILLALL)
    {
        cPreToolBar.SetButtonStyle(2,TBBS_CHECKBOX|TBBS_PRESSED);
        return;
    }
    dKillOption = KILLALL;
    cFileContent = GetNoteStringUnicode(cSourFileName,dKillOption,TRUE,BACK_UNICODE,NULL,0);
    m_Result.SetWindowText(cFileContent);//显示从一个文件中删除注释的结果
    cPreToolBar.SetButtonStyle(2,TBBS_CHECKBOX|TBBS_PRESSED);
    cPreToolBar.SetButtonStyle(3,TBBS_CHECKBOX);
    cPreToolBar.SetButtonStyle(4,TBBS_CHECKBOX);*/

    dwStyle = cPreToolBar.GetButtonStyle(2);
    if (!(dwStyle & TBBS_CHECKED))//当前已经按下按钮,这时候需要将按钮抬起
    {
        m_Preview.GetWindowText(cFileContent);
        m_Result.SetWindowText(cFileContent);
        cTempFile = CreateTempFile(cSourFileName,cFileContent);
        SetCommentColor(cTempFile,m_Result);//设置颜色
    }
    else//当前按钮是抬起状态,需要修改为按下状态
    {
        dKillOption = KILLALL;
        cPreToolBar.SetButtonStyle(3,TBBS_CHECKBOX);
        cPreToolBar.SetButtonStyle(4,TBBS_CHECKBOX);
        cFileContent = GetNoteStringUnicode(cSourFileName,dKillOption,TRUE,BACK_UNICODE,NULL,0);
        m_Result.SetWindowText(cFileContent);//显示从一个文件中删除注释的结果
    }
    cPreToolBar.Invalidate(TRUE);
    cPreToolBar.UpdateWindow();
}

void CPreviewCode::OnButtonPreviewHead() 
{
	// TODO: Add your command handler code here
	//保留文件头注释
    CString cFileContent;
    UINT dwStyle;
    CWaitCursor wait;

    dwStyle = cPreToolBar.GetButtonStyle(3);
    if (!(dwStyle & TBBS_CHECKED))//当前已经按下按钮,这时候需要将按钮抬起
    {
        if (dKillOption == KILL_HOLDFILEHEADER)
        {
            m_Preview.GetWindowText(cFileContent);
            m_Result.SetWindowText(cFileContent);
            cTempFile = CreateTempFile(cSourFileName,cFileContent);
            SetCommentColor(cTempFile,m_Result);//设置颜色
        }
        else
        {
            dKillOption = KILL_HOLDFUNCHEADER;
            cFileContent = GetNoteStringUnicode(cSourFileName,dKillOption,TRUE,BACK_UNICODE,NULL,0);
            m_Result.SetWindowText(cFileContent);//显示从一个文件中删除注释的结果
            cTempFile = CreateTempFile(cSourFileName,cFileContent);
            SetCommentColor(cTempFile,m_Result);//设置颜色
        }
    }
    else//当前按钮是抬起状态,需要修改为按下状态
    {
        cPreToolBar.SetButtonStyle(2,TBBS_CHECKBOX);//将全部删除按钮设为抬起状态
        if (dKillOption == KILLALL)
            dKillOption = KILL_HOLDFILEHEADER;
        else
            dKillOption |= KILL_HOLDFILEHEADER;

        cFileContent = GetNoteStringUnicode(cSourFileName,dKillOption,TRUE,BACK_UNICODE,NULL,0);
        m_Result.SetWindowText(cFileContent);//显示从一个文件中删除注释的结果
        cTempFile = CreateTempFile(cSourFileName,cFileContent);
        SetCommentColor(cTempFile,m_Result);//设置颜色
    }
    /*cPreToolBar.SetButtonStyle(2,TBBS_CHECKBOX);//将全部删除按钮设为抬起状态
    dwStyle = cPreToolBar.GetButtonStyle(3);//将本按钮设置为按下状态
    if (!(dwStyle & TBBS_CHECKBOX))
        dwStyle |= TBBS_CHECKBOX;
    if (!(dwStyle & TBBS_PRESSED))
        dwStyle |= TBBS_PRESSED;

    cPreToolBar.SetButtonStyle(3,dwStyle);
    if (dKillOption == KILLALL)
    {
        dKillOption = KILL_HOLDFILEHEADER;
    }
    else if (dKillOption & KILL_HOLDFILEHEADER)
    {
        cPreToolBar.SetButtonStyle(3,TBBS_CHECKBOX|TBBS_PRESSED);
        return;
    }
    else
    {
        dKillOption |= KILL_HOLDFILEHEADER;
    }
    cFileContent = GetNoteStringUnicode(cSourFileName,dKillOption,TRUE,BACK_UNICODE,NULL,0);
    m_Result.SetWindowText(cFileContent);//显示从一个文件中删除注释的结果
    cTempFile = CreateTempFile(cSourFileName,cFileContent);
    SetCommentColor(cTempFile,m_Result);//设置颜色*/
}

void CPreviewCode::OnButtonPreviewFunc() 
{
	// TODO: Add your command handler code here
	//保留函数头注释
    CString cFileContent;
    UINT dwStyle;
    CWaitCursor wait;

    dwStyle = cPreToolBar.GetButtonStyle(4);
    if (!(dwStyle & TBBS_CHECKED))//当前已经按下按钮,这时候需要将按钮抬起
    {
        if (dKillOption == KILL_HOLDFUNCHEADER)
        {
            m_Preview.GetWindowText(cFileContent);
            m_Result.SetWindowText(cFileContent);
            cTempFile = CreateTempFile(cSourFileName,cFileContent);
            SetCommentColor(cTempFile,m_Result);//设置颜色
        }
        else
        {
            dKillOption = KILL_HOLDFILEHEADER;
            cFileContent = GetNoteStringUnicode(cSourFileName,dKillOption,TRUE,BACK_UNICODE,NULL,0);
            m_Result.SetWindowText(cFileContent);//显示从一个文件中删除注释的结果
            cTempFile = CreateTempFile(cSourFileName,cFileContent);
            SetCommentColor(cTempFile,m_Result);//设置颜色
        }
    }
    else//当前按钮是抬起状态,需要修改为按下状态
    {
        cPreToolBar.SetButtonStyle(2,TBBS_CHECKBOX);//将全部删除按钮设为抬起状态
        if (dKillOption == KILLALL)
            dKillOption = KILL_HOLDFUNCHEADER;
        else
            dKillOption |= KILL_HOLDFUNCHEADER;

        cFileContent = GetNoteStringUnicode(cSourFileName,dKillOption,TRUE,BACK_UNICODE,NULL,0);
        m_Result.SetWindowText(cFileContent);//显示从一个文件中删除注释的结果
        cTempFile = CreateTempFile(cSourFileName,cFileContent);
        SetCommentColor(cTempFile,m_Result);//设置颜色
    }
    /*cPreToolBar.SetButtonStyle(2,TBBS_CHECKBOX);//将全部删除按钮设为抬起状态

    dwStyle = cPreToolBar.GetButtonStyle(4);//将本按钮设置为按下状态
    if (!(dwStyle & TBBS_CHECKBOX))
        dwStyle |= TBBS_CHECKBOX;
    if (!(dwStyle & TBBS_PRESSED))
        dwStyle |= TBBS_PRESSED;

    cPreToolBar.SetButtonStyle(4,dwStyle);

    if (dKillOption == KILLALL)
    {
        dKillOption = KILL_HOLDFUNCHEADER;
    }
    else if (dKillOption & KILL_HOLDFUNCHEADER)
    {
        cPreToolBar.SetButtonStyle(4,TBBS_CHECKBOX|TBBS_PRESSED);
        return;
    }
    else
    {
        dKillOption |= KILL_HOLDFUNCHEADER;
    }
    cFileContent = GetNoteStringUnicode(cSourFileName,dKillOption,TRUE,BACK_UNICODE,NULL,0);
    m_Result.SetWindowText(cFileContent);//显示从一个文件中删除注释的结果

    cTempFile = CreateTempFile(cSourFileName,cFileContent);
    SetCommentColor(cTempFile,m_Result);//设置颜色*/
}
//将cContent的内容生成一个临时文件
CString CPreviewCode::CreateTempFile(CString cSourFileName,CString cContent)
{
    CString cTempFileName;
    CFile fTempFile;
    DWORD dContentlen;
    DWORD dAsciiLen;
    LPTSTR pAsciiString;

    cTempFileName = GetCurrentAppPath() + _T("\\temp.tmp");
    if (!fTempFile.Open(cTempFileName,CFile::modeCreate | CFile::modeWrite))
        return "";
    
    dContentlen = cContent.GetLength();

    pAsciiString = new TCHAR[dContentlen * 2 + 1];
    memset(pAsciiString,0x00,dContentlen * 2 + 1);
    
    dAsciiLen = UnicodeToAsciiString(cContent,(LPSTR)pAsciiString);//将unicode编码的字符串转换成ascii格式
    fTempFile.Write(pAsciiString,dAsciiLen);

    delete pAsciiString;
    fTempFile.Close();

    return cTempFileName;
}

void CPreviewCode::OnButtonClose() 
{
	// TODO: Add your command handler code here
	CFile::Remove(cTempFile);//将临时文件删除
    CDialog::OnCancel();
}

BOOL CPreviewCode::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CFile::Remove(cTempFile);//将临时文件删除
	return CDialog::DestroyWindow();
}

void CPreviewCode::OnButtonPreviewSave() 
{
	// TODO: Add your command handler code here
	//按照当前方式输出文件
    LPTSTR  lpstr;
    DWORD dFileSize;
    CString cContent = "";
    CFile fSourFile;
    CFile fDesFile;
    CWaitCursor wait;
    CString cOutFail,cClew,cSourIsDes,cOutSucc;

    cClew.LoadString(IDS_STRING_CLEW);
    if (cSourFileName == cDesFileName)
    {
        cSourIsDes.LoadString(IDS_STRING_FILESOURISDES);//目的文件就是源文件,这样做会覆盖源文件,是否继续?
        if (MessageBox(cSourIsDes,cClew, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
        {
            return;
        }
    }
    cOutFail.LoadString(IDS_STRING_OUTFILEFAIL);
    m_Result.GetWindowText(cContent);
    dFileSize = cContent.GetLength();
    lpstr = new TCHAR[dFileSize * 2 + 1];
    memset(lpstr,0x00,dFileSize * 2 + 1);
    dFileSize = UnicodeToAsciiString(cContent,(LPSTR)lpstr);

    if (cSourFileName != cDesFileName)//源文件不是目的文件,直接生成目的文件
    {
        CreateDirectoryFromString(cDesFileName);//创建所需的目录
        if (!fDesFile.Open(cDesFileName,CFile::modeCreate | CFile::modeWrite))
        {//创建新文件
            delete lpstr;
            fDesFile.Close();
            MessageBox(cOutFail,cClew,MB_ICONASTERISK |MB_OK);
            return;
        }
        fDesFile.Write(lpstr,dFileSize);
        dFileSize = fDesFile.GetLength();
        fDesFile.Close();
    }
    else//源文件就是目的文件,那么首先生成一个临时文件,再将源文件删除,然后将临时文件改名,改为源文件名
    {
        CString cTempName;

        cTempName = cDesFileName + cTempExtName;//生成临时文件
        if (!fDesFile.Open(cTempName,CFile::modeCreate | CFile::modeWrite))
        {//创建新文件
            delete lpstr;
            fDesFile.Close();
            MessageBox(cOutFail,cClew,MB_ICONASTERISK |MB_OK);
            return;
        }
        fDesFile.Write((const TCHAR *)lpstr,dFileSize);
        dFileSize = fDesFile.GetLength();
        fDesFile.Close();

        TRY
        {
            CFile::Remove(cSourFileName);//将源文件删除
            CFile::Rename(cTempName,cSourFileName);//将临时文件改名
        }
        CATCH( CFileException , e )
        {
            MessageBox(cOutFail,cClew,MB_ICONHAND |MB_OK);
            TRACE(cSourFileName + _T(" Output fail!"));
        }
        END_CATCH
    }
    delete lpstr;
    cOutSucc.LoadString(IDS_STRING_END);
    MessageBox(cOutSucc,cClew,MB_ICONEXCLAMATION | MB_OK);
}
