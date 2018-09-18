// FileAddEdit.cpp : implementation file
//

#include "stdafx.h"
#include "KillContent.h"
#include "FileAddEdit.h"
#include "TCHAR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define BIF_USENEWUI                0x0050
/////////////////////////////////////////////////////////////////////////////
// CFileAddEdit dialog
static const int     iItemMaxCount  = 20;
static const CString IniFileName    = _T("FileFold.ini");
static const CString csSeperator    = _T("=");
static const CString csFileSourSign = _T("FileSour");//保存项目的时候，文件开始的标志
static const CString csFileDesSign  = _T("FileDes"); //保存项目的时候，文件开始的标志
static const CString csFOldSourSign = _T("FoldSour");//保存项目的时候，目录开始的标志
static const CString csFOldDesSign  = _T("FoldDes"); //保存项目的时候，目录开始的标志
static const CString csNewLine      = _T("\r\n");
static const CString cNewLine1      = _T("\r");
static const CString cNewLine2      = _T("\n");
static const CString csSlash        = _T("\\");
static const TCHAR   ccSlash        = _T('\\');
static const TCHAR   ccPoint        = _T('.');

CFileAddEdit::CFileAddEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CFileAddEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileAddEdit)
	//}}AFX_DATA_INIT
    OperateType = OPERATETYPE_ADD;
}


void CFileAddEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileAddEdit)
	DDX_Control(pDX, IDCANCEL, m_Button_Cancel);
	DDX_Control(pDX, IDOK, m_Button_OK);
	DDX_Control(pDX, IDC_STATIC_DES, m_Static_Des);
	DDX_Control(pDX, IDC_STATIC_SOUR, m_Static_Sour);
	DDX_Control(pDX, IDC_COMBO_DES, m_ComDes);
	DDX_Control(pDX, IDC_COMBO_SOUR, m_ComSour);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileAddEdit, CDialog)
	//{{AFX_MSG_MAP(CFileAddEdit)
	ON_BN_CLICKED(IDC_BUTTON_SOURFILE, OnButtonSourfile)
	ON_BN_CLICKED(IDC_BUTTON_DESFILE, OnButtonDesfile)
	ON_CBN_DROPDOWN(IDC_COMBO_DES, OnDropdownComboDes)
	ON_CBN_DROPDOWN(IDC_COMBO_SOUR, OnDropdownComboSour)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileAddEdit message handlers

BOOL CFileAddEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
    CString cCaption,cOk,cCancel;
    cOk.LoadString(IDS_STRING_OK);
    cCancel.LoadString(IDS_STRING_CANCEL);

    m_Button_OK.SetWindowText(cOk);
    m_Button_Cancel.SetWindowText(cCancel);
    switch (OperateType)
    {
    case OPERATETYPE_ADD:
        cCaption.LoadString(IDS_STRING_ADD);//添加
        break;
    case OPERATETYPE_EDIT:
        cCaption.LoadString(IDS_STRING_EDIT);//编辑        
        m_ComSour.SetWindowText(initSourFile_Fold);
        m_ComDes.SetWindowText(initDesFile_Fold);
        break;
    }
    SetWindowText(cCaption);

    CString cSour,cDes;
    switch (DialogType)
    {
    case DIALOGTYPE_FILE:
        cSour.LoadString(IDS_STRING_SELECT_SOURFILE);//选择源文件:
        cDes.LoadString(IDS_STRING_SELECT_DESFILE);//选择目的文件:
        break;
    case DIALOGTYPE_FOLD:
        cSour.LoadString(IDS_STRING_SELECT_SOURFOLD);//选择源目录:
        cDes.LoadString(IDS_STRING_SELECT_DESFOLD);//选择目的目录:
        break;
    }
    m_Static_Sour.SetWindowText(cSour);
    m_Static_Des.SetWindowText(cDes);

    ReadFoldList(m_ComSour);
    ReadFoldList(m_ComDes);

    CenterWindow();//使窗口运行时居于屏幕正中央
    DWORD cAnimate[] =
    {
        AW_CENTER,
        AW_BLEND,
        AW_HOR_POSITIVE,
        AW_HOR_NEGATIVE,
        AW_VER_POSITIVE,
        AW_VER_NEGATIVE
    };
    srand((int)time(0));
//    AnimateWindow(GetSafeHwnd(),500,cAnimate[rand() % 6]);//remove for vs2010
//    AnimateWindow(500,cAnimate[rand() % 6]);//for vs2010
    //动画方式打开窗口 AW_CENTER AW_BLEND AW_HOR_POSITIVE AW_HOR_NEGATIVE AW_VER_POSITIVE AW_VER_NEGATIVE

    m_ComSour.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFileAddEdit::CFileOperateType(OPERATETYPE Operate_Type,//设置对话框风格
                                    DIALOGTYPE Dialog_Type,//对话框类型
                                    CString cFileSour,CString cFileDes)
{
    switch (Operate_Type)
    {
    case OPERATETYPE_ADD:
        OperateType         = Operate_Type;
        DialogType          = Dialog_Type;
        return TRUE;
    case OPERATETYPE_EDIT:
        OperateType         = Operate_Type;
        DialogType          = Dialog_Type;
        initSourFile_Fold   = cFileSour;
        initDesFile_Fold    = cFileDes;
        return TRUE;
    default:
        return FALSE;
    }
}

void CFileAddEdit::OnButtonSourfile() //添加源文件
{
	// TODO: Add your control notification handler code here
    AddEditFile(m_ComSour);
}

//添加,编辑项目
void CFileAddEdit::AddEditFile(CComboBox & combox)
{
    CString cItemName;//项目名    
    CString cInitDir = "";//用于设置当前文件所在目录

    combox.GetWindowText(cItemName);//获得commbox控件中的内容

    if (DialogType == DIALOGTYPE_FILE)
    {
        if (cItemName != "")
        {
            int iExtName = cItemName.ReverseFind(ccPoint);//_T('.')
            //查找combox控件的文字中是否包含文件扩展名,如果包含扩展名,则认为包含文件名
            
            if (iExtName != -1)//包含文件名
            {
                int iReverseFind = cItemName.ReverseFind(ccSlash);//_T('\\')
                cInitDir = (iReverseFind != -1) ? cItemName.Left(iReverseFind) : cItemName;
            }
            else//不包含文件名,则认为commbox控件中的内容可能是目录
            {
                cInitDir = cItemName;
            }
        }    

        CString cSelectFile,cFilter,cdefext;
        cSelectFile.LoadString(IDS_STRING_SELECTFILE);
        cFilter.LoadString(IDS_STRING_FILTER);
        cdefext.LoadString(IDS_STRING_DEFAULT_EXTENTNAME);

        CFileDialog open((combox == m_ComSour),NULL,NULL,
            OFN_HIDEREADONLY|OFN_EXTENSIONDIFFERENT,cFilter);

        open.m_ofn.lpstrTitle		= cSelectFile;//"选择文件";
        open.m_ofn.lpstrInitialDir	= cInitDir;
        open.m_ofn.lpstrDefExt		= cdefext;//"*.c;*.cpp;*.h";
        if (open.DoModal() == IDOK)
        {
            cItemName = open.GetPathName();
            combox.SetWindowText(cItemName);
        }
    }
    else
    {
        CString cSelectFold;
        BROWSEINFO bi;         
        TCHAR Dir[MAX_PATH];
        ITEMIDLIST * pidl;
        cSelectFold.LoadString(IDS_STRING_SELECTFOLD);

        bi.hwndOwner		= this->GetSafeHwnd();
        bi.pidlRoot			= NULL;
        bi.pszDisplayName	= Dir; 
        bi.lpszTitle		= cSelectFold;//"选择目录并按下[确定]按钮"; //选择目录
        bi.ulFlags			= BIF_USENEWUI;//BIF_RETURNONLYFSDIRS ; 
        bi.lpfn				= SetSelProc;
        bi.lParam			= (LPARAM)(LPCTSTR)cItemName;
        bi.iImage			= 0; 
        pidl                = SHBrowseForFolder(&bi);

        if (pidl == NULL) 
            memset(Dir,0,sizeof(MAX_PATH));
        if (SHGetPathFromIDList(pidl,Dir))/*Retrieve folder name from ITEMLIST structure.*/ 
        {
            combox.SetWindowText(Dir);
        }
    }
}

int CALLBACK CFileAddEdit::SetSelProc(HWND hWnd, UINT uMsg, LPARAM /*lParam*/, LPARAM lpData)
{
    if (uMsg == BFFM_INITIALIZED)
        ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);

    return 0;
}

void CFileAddEdit::OnButtonDesfile() //添加目的文件
{
	// TODO: Add your control notification handler code here
	AddEditFile(m_ComDes);
}

void CFileAddEdit::OnOK() 
{
	// TODO: Add extra validation here
	CString cFileSour = "";
    CString cFileDes = "";    
    CStdioFile	file;
    CString cClew;

    cClew.LoadString(IDS_STRING_CLEW);

    m_ComSour.GetWindowText(cFileSour);//获得源文件
    m_ComDes.GetWindowText(cFileDes);//获得目的文件
    switch (DialogType)
    {
    case DIALOGTYPE_FILE:
        if (cFileSour == "")
        {
            CString cSour;
            cSour.LoadString(IDS_STRING_INPUTSOURFILE);
            MessageBox(cSour,cClew,MB_ICONASTERISK |MB_OK);//请输入源文件
            m_ComSour.SetFocus();
            return;
        }
        if (cFileDes == "")
        {
            CString cDes;
            cDes.LoadString(IDS_STRING_INPUTDESFILE);
            MessageBox(cDes,cClew,MB_ICONASTERISK |MB_OK);//请输入目的文件
            m_ComDes.SetFocus();
            return;
        }
        if (cFileSour == cFileDes)
        {
            CString cSourIsDes;
            cSourIsDes.LoadString(IDS_STRING_FILESOURISDES);//目的文件就是源文件,这样做会覆盖源文件,是否继续?
            if (MessageBox(cSourIsDes,cClew, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
            {
                return;
            }
        }
        
        if (!file.Open(cFileSour,CFile::modeRead,NULL))//打开失败
        {
            CString cOpenFail,cue;
            cOpenFail.LoadString(IDS_STRING_FINDSOURFILEFAIL);//源文件%s不存在,请重新选择
            cue.Format(cOpenFail,cFileSour);
            MessageBox(cue,cClew,MB_ICONASTERISK |MB_OK);
            m_ComSour.SetFocus();
            return;
        }
        file.Close();
        break;

    case DIALOGTYPE_FOLD:
        TCHAR cCurDirBak[256];
        GetCurrentDirectory(sizeof(cCurDirBak),cCurDirBak);

        if (cFileSour == "")
        {
            CString cSour;
            cSour.LoadString(IDS_STRING_INPUTSOURFOLD);//请输入源目录
            MessageBox(cSour,cClew,MB_ICONASTERISK |MB_OK);
            m_ComSour.SetFocus();
            return;
        }
        if (cFileDes == "")
        {
            CString cDes;
            cDes.LoadString(IDS_STRING_INPUTDESFOLD);//请输入目的目录
            MessageBox(cDes,cClew,MB_ICONASTERISK |MB_OK);
            m_ComDes.SetFocus();
            return;
        }
        if (cFileSour == cFileDes)
        {
            CString cSourIsDes;
            cSourIsDes.LoadString(IDS_STRING_FOLDSOURISDES);//目的目录就是源目录,这样做会覆盖源文件,是否继续?
            if(MessageBox(cSourIsDes,cClew,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
            {
                return;
            }
        }
        if (!SetCurrentDirectory(cFileSour))
        {
            CString cOpenFail,cue;
            cOpenFail.LoadString(IDS_STRING_FINDSOURFOLDFAIL);//目录%s不存在,请重新选择
            cue.Format(cOpenFail,cFileSour);
            MessageBox(cue,cClew,MB_ICONASTERISK |MB_OK);
            m_ComSour.SetFocus();
            SetCurrentDirectory(cCurDirBak);
            return;
        }
        SetCurrentDirectory(cCurDirBak);
        break;
    }    

    Item_Sour = cFileSour;
    Item_Des  = cFileDes;

    SaveFoldList(m_ComSour,Item_Sour);
    SaveFoldList(m_ComDes ,Item_Des );    

	CDialog::OnOK();
}

void CFileAddEdit::GetFileName(CString & cFileNameSour,CString & cFileNameDes)//获得源文件,目的文件
{
    cFileNameSour = Item_Sour;
    cFileNameDes  = Item_Des;
}

BOOL CFileAddEdit::SaveFoldList(CComboBox & combox,CString sItem)//保存项目
{
	int i,iComboxCount;
	
	iComboxCount = combox.GetCount();
	for (i = iComboxCount - 1;i >= 0;i--)
	{
        CString	csFoldList;
		combox.GetLBText(i,csFoldList);
		if (sItem == csFoldList)//该目录在列表框中已经存在
			combox.DeleteString(i);//删掉这一项
	}
	combox.InsertString(0,sItem);

	iComboxCount = combox.GetCount();
	if (iComboxCount > iItemMaxCount)//删掉多余的一项
		combox.DeleteString(iComboxCount - 1);

    //////////////////////////////////////////////////////
    CString cPreType;//前缀类别
    switch (DialogType)
    {
    case DIALOGTYPE_FILE:
        cPreType = (combox == m_ComSour) ? csFileSourSign : csFileDesSign;
        break;
    case DIALOGTYPE_FOLD:
        cPreType = (combox == m_ComSour) ? csFOldSourSign : csFOldDesSign;
        break;
    }

    CString     cFileName = "";
    CString     cResult = "";
    CString     bufRead;    
    CString     cSaveContent;//需要保存的内容
    CFile       cfile,cstdofile;

    LPSTR       pResultAscii;
    LPCWSTR     lpMultiByteStr;
    int         iResultLen;

    cFileName = GetCurrentAppPath() + csSlash +IniFileName;//_T("\\")

    if (cstdofile.Open(cFileName,CFile::modeReadWrite))//文件存在
    {
        iComboxCount = combox.GetCount();
        for (i = 0,cSaveContent = "";i < iComboxCount;i++)
        {
            CString csFoldList;
            if (i > iItemMaxCount - 1)
                break;
            combox.GetLBText(i,csFoldList);
            cSaveContent = cSaveContent + cPreType + csSeperator + csFoldList + csNewLine;
        }

        //ascii转换成unicode
        CString cUnicode = "";
        DWORD dFileLen;
        unsigned char* pBufAscii = NULL;
        LPWSTR pBufUnicode = NULL;

        dFileLen = cstdofile.GetLength();
        if (dFileLen != 0)
        {
            pBufAscii   = new unsigned char[2 * dFileLen + 1];
            pBufUnicode = new TCHAR[2 * dFileLen + 1];
            memset(pBufAscii,0x00,2 * dFileLen + 1);
            memset(pBufUnicode,0x00,2 * dFileLen + 1);
            //cstdofile.ReadHuge(pBufAscii,dFileLen);//remove for vs 2010
            cstdofile.Read(pBufAscii,dFileLen);
            MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)pBufAscii,-1,
                pBufUnicode,dFileLen);

            delete pBufAscii;
            cUnicode = pBufUnicode;
            delete pBufUnicode;

            while (1)
            {
                int iPre,iLine;
                CString cSingleLine;
                iLine = cUnicode.Find(csNewLine);
                if (-1 == iLine)
                {
                    iPre = cUnicode.Find(csSeperator);
                    if (-1 == iPre)
                        break;
                    if (cUnicode.Left(iPre) != cPreType)
                        cResult = cResult + cUnicode + csNewLine;

                    break;
                }
                cSingleLine = cUnicode.Left(iLine);

                iPre = cSingleLine.Find(csSeperator);
                if (-1 == iPre)
                    break;
                while ((cSingleLine.Right(1) == cNewLine1) ||//删除最后的回车符 13
                    (cSingleLine.Right(1) == cNewLine2))
                {
                    cSingleLine = cSingleLine.Left(cSingleLine.GetLength() - 1);
                }
                if (cSingleLine.Left(iPre) != cPreType)
                    cResult = cResult + cSingleLine + csNewLine;

                cUnicode = cUnicode.Right(cUnicode.GetLength() - iLine - 2);

                while ((cUnicode.Right(1) == cNewLine1) ||//删除最后的回车符 13
                    (cUnicode.Right(1) == cNewLine2))
                {
                    cUnicode = cUnicode.Left(cUnicode.GetLength() - 1);
                }
                while ((cUnicode.Left(1) == cNewLine1) ||//删除最后的回车符 13
                    (cUnicode.Left(1) == cNewLine2))
                {
                    cUnicode = cUnicode.Right(cUnicode.GetLength() - 1);
                }
            }
        }
        if (cResult != "")
            cResult = cResult + csNewLine + cSaveContent;
        else
            cResult = cSaveContent;

        iResultLen = cResult.GetLength();

        lpMultiByteStr = (LPCWSTR)cResult.GetBuffer(iResultLen);
        pResultAscii = new CHAR[2 * iResultLen + 1];
        memset(pResultAscii,0x00,2 * iResultLen + 1);
        WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,
            lpMultiByteStr,iResultLen,
            pResultAscii,2 * iResultLen + 1,NULL,NULL);

        cResult.ReleaseBuffer();
        cstdofile.Close();

        if (cfile.Open(cFileName,CFile::modeWrite))
        {
            cfile.Write(pResultAscii,strlen(pResultAscii));
            cfile.Close();
        }
        else
        {
            CString cSaveFile,cClew;
            cSaveFile.LoadString(IDS_STRING_SAVEFILEFAIL);//无法保存文件
            cClew.LoadString(IDS_STRING_CLEW);//提示
            MessageBox(cSaveFile,cClew,MB_ICONSTOP|MB_OK);
        }
        delete pResultAscii;
    }
    else//文件不存在
    {
        if (cfile.Open(cFileName,CFile::modeCreate | CFile::modeWrite))
        {            
            iComboxCount = combox.GetCount();
            for (i = 0;i < iComboxCount;i++)
            {
                CString csFoldList;
                if (i > iItemMaxCount - 1)
                    break;
                combox.GetLBText(i,csFoldList);
                cSaveContent = cPreType + csSeperator + csFoldList + csNewLine;

                iResultLen = cSaveContent.GetLength();
                lpMultiByteStr = (LPCWSTR)cSaveContent.GetBuffer(iResultLen);
                pResultAscii = new CHAR[2 * iResultLen + 1];
                memset(pResultAscii,0x00,2 * iResultLen + 1);
                WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,
                    lpMultiByteStr,iResultLen,
                    pResultAscii,2 * iResultLen + 1,NULL,NULL);

                cSaveContent.ReleaseBuffer();

                cfile.Write(pResultAscii,strlen(pResultAscii));
                delete pResultAscii;
            }
            cfile.Close();
        }
        else
        {
            CString cSaveFile,cClew;
            cSaveFile.LoadString(IDS_STRING_SAVEFILEFAIL);//无法保存文件
            cClew.LoadString(IDS_STRING_CLEW);//提示
            MessageBox(cSaveFile,cClew,MB_ICONSTOP|MB_OK);
        }
    }
	return TRUE;
}

void CFileAddEdit::ReadFoldList(CComboBox & combox)//读取目录列表
{
	CFile       file;
    CString     cFileName;    
    CString     cPreType;//前缀类别
    DWORD       dFileLen;
    LPWSTR      pBufUnicode = NULL;
    unsigned char * pBufAscii = NULL;

    switch (DialogType)
    {
    case DIALOGTYPE_FILE:
        cPreType = (combox == m_ComSour) ? csFileSourSign : csFileDesSign;
        break;
    case DIALOGTYPE_FOLD:
        cPreType = (combox == m_ComSour) ? csFOldSourSign : csFOldDesSign;
        break;
    }
    cFileName = GetCurrentAppPath() + csSlash + IniFileName;//_T("\\")
    if (!file.Open(cFileName,CFile::modeRead))
        return;

    dFileLen    = file.GetLength();
    pBufAscii   = new unsigned char[2 * dFileLen + 1];
    pBufUnicode = new TCHAR[2 * dFileLen + 1];
    memset(pBufAscii,0x00,2 * dFileLen + 1);
    memset(pBufUnicode,0x00,2 * dFileLen + 1);
    //file.ReadHuge(pBufAscii,dFileLen);//remvoe for vs 2010
    file.Read(pBufAscii,dFileLen);
    file.Close();
    //ascii转换成unicode
    MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)pBufAscii,-1,pBufUnicode,2 * dFileLen + 1);
    delete pBufAscii;

    CString cUnicode = pBufUnicode;
    while (1)
    {
        int iPre,iLine;
        CString cSingleLine;
        
        iLine = cUnicode.Find(csNewLine);
        if (-1 == iLine)
        {
            iPre = cUnicode.Find(csSeperator);
            if (-1 == iPre)
                break;

            if (cUnicode.Left(iPre) == cPreType)
            {
                cUnicode = cUnicode.Right(cUnicode.GetLength() - iPre - 1);
                combox.AddString(cUnicode);
            }
            break;
        }
        cSingleLine = cUnicode.Left(iLine);

        iPre = cSingleLine.Find(csSeperator);
        if (-1 == iPre)
            break;
        while ((cSingleLine.Right(1) == cNewLine1) || //删除最后的回车符13
            (cSingleLine.Right(1) == cNewLine2))
        {
            cSingleLine = cSingleLine.Left(cSingleLine.GetLength() - 1);
        }
        if (cSingleLine.Left(iPre) == cPreType)
        {
            cSingleLine = cSingleLine.Right(cSingleLine.GetLength() - iPre - 1);
            combox.AddString(cSingleLine);
        }
        cUnicode = cUnicode.Right(cUnicode.GetLength() - iLine - 2);

        while ((cUnicode.Left(1) == cNewLine1) || (cUnicode.Left(1) == cNewLine2))
        {
            cUnicode = cUnicode.Right(cUnicode.GetLength() - 1);
        }
        while ((cUnicode.Right(1) == cNewLine1) || (cUnicode.Right(1) == cNewLine2))
        {
            cUnicode = cUnicode.Left(cUnicode.GetLength() - 1);
        }
    }

    delete pBufUnicode;
}

void CFileAddEdit::OnDropdownComboDes() 
{
	// TODO: Add your control notification handler code here
    SetComboWidth(m_ComDes);	
}

void CFileAddEdit::SetComboWidth(CComboBox & combox)//设置下拉列表部分的宽度
{
    int		i,iComboxCount,iMaxLen;
	CString	csFoldList;

    iMaxLen      = 0;
	iComboxCount = combox.GetCount();
	for (i = iComboxCount - 1;i >= 0;i--)
	{
		combox.GetLBText(i,csFoldList);
		if (csFoldList.GetLength() > iMaxLen)
            iMaxLen = csFoldList.GetLength();
	}
    combox.SendMessage(CB_SETDROPPEDWIDTH,iMaxLen * 7,0);
}

void CFileAddEdit::OnDropdownComboSour() 
{
	// TODO: Add your control notification handler code here
	SetComboWidth(m_ComSour);
}

void CFileAddEdit::OnCancel() 
{
	// TODO: Add extra cleanup here
	//AnimateWindow(GetSafeHwnd(),1000,AW_CENTER|AW_HIDE);//在窗口关闭时，同样以动画的方式结束
    //不管用啊，怎么回事?
	CDialog::OnCancel();
}
