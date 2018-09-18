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
static const CString csFileSourSign = _T("FileSour");//������Ŀ��ʱ���ļ���ʼ�ı�־
static const CString csFileDesSign  = _T("FileDes"); //������Ŀ��ʱ���ļ���ʼ�ı�־
static const CString csFOldSourSign = _T("FoldSour");//������Ŀ��ʱ��Ŀ¼��ʼ�ı�־
static const CString csFOldDesSign  = _T("FoldDes"); //������Ŀ��ʱ��Ŀ¼��ʼ�ı�־
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
        cCaption.LoadString(IDS_STRING_ADD);//���
        break;
    case OPERATETYPE_EDIT:
        cCaption.LoadString(IDS_STRING_EDIT);//�༭        
        m_ComSour.SetWindowText(initSourFile_Fold);
        m_ComDes.SetWindowText(initDesFile_Fold);
        break;
    }
    SetWindowText(cCaption);

    CString cSour,cDes;
    switch (DialogType)
    {
    case DIALOGTYPE_FILE:
        cSour.LoadString(IDS_STRING_SELECT_SOURFILE);//ѡ��Դ�ļ�:
        cDes.LoadString(IDS_STRING_SELECT_DESFILE);//ѡ��Ŀ���ļ�:
        break;
    case DIALOGTYPE_FOLD:
        cSour.LoadString(IDS_STRING_SELECT_SOURFOLD);//ѡ��ԴĿ¼:
        cDes.LoadString(IDS_STRING_SELECT_DESFOLD);//ѡ��Ŀ��Ŀ¼:
        break;
    }
    m_Static_Sour.SetWindowText(cSour);
    m_Static_Des.SetWindowText(cDes);

    ReadFoldList(m_ComSour);
    ReadFoldList(m_ComDes);

    CenterWindow();//ʹ��������ʱ������Ļ������
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
    //������ʽ�򿪴��� AW_CENTER AW_BLEND AW_HOR_POSITIVE AW_HOR_NEGATIVE AW_VER_POSITIVE AW_VER_NEGATIVE

    m_ComSour.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFileAddEdit::CFileOperateType(OPERATETYPE Operate_Type,//���öԻ�����
                                    DIALOGTYPE Dialog_Type,//�Ի�������
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

void CFileAddEdit::OnButtonSourfile() //���Դ�ļ�
{
	// TODO: Add your control notification handler code here
    AddEditFile(m_ComSour);
}

//���,�༭��Ŀ
void CFileAddEdit::AddEditFile(CComboBox & combox)
{
    CString cItemName;//��Ŀ��    
    CString cInitDir = "";//�������õ�ǰ�ļ�����Ŀ¼

    combox.GetWindowText(cItemName);//���commbox�ؼ��е�����

    if (DialogType == DIALOGTYPE_FILE)
    {
        if (cItemName != "")
        {
            int iExtName = cItemName.ReverseFind(ccPoint);//_T('.')
            //����combox�ؼ����������Ƿ�����ļ���չ��,���������չ��,����Ϊ�����ļ���
            
            if (iExtName != -1)//�����ļ���
            {
                int iReverseFind = cItemName.ReverseFind(ccSlash);//_T('\\')
                cInitDir = (iReverseFind != -1) ? cItemName.Left(iReverseFind) : cItemName;
            }
            else//�������ļ���,����Ϊcommbox�ؼ��е����ݿ�����Ŀ¼
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

        open.m_ofn.lpstrTitle		= cSelectFile;//"ѡ���ļ�";
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
        bi.lpszTitle		= cSelectFold;//"ѡ��Ŀ¼������[ȷ��]��ť"; //ѡ��Ŀ¼
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

void CFileAddEdit::OnButtonDesfile() //���Ŀ���ļ�
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

    m_ComSour.GetWindowText(cFileSour);//���Դ�ļ�
    m_ComDes.GetWindowText(cFileDes);//���Ŀ���ļ�
    switch (DialogType)
    {
    case DIALOGTYPE_FILE:
        if (cFileSour == "")
        {
            CString cSour;
            cSour.LoadString(IDS_STRING_INPUTSOURFILE);
            MessageBox(cSour,cClew,MB_ICONASTERISK |MB_OK);//������Դ�ļ�
            m_ComSour.SetFocus();
            return;
        }
        if (cFileDes == "")
        {
            CString cDes;
            cDes.LoadString(IDS_STRING_INPUTDESFILE);
            MessageBox(cDes,cClew,MB_ICONASTERISK |MB_OK);//������Ŀ���ļ�
            m_ComDes.SetFocus();
            return;
        }
        if (cFileSour == cFileDes)
        {
            CString cSourIsDes;
            cSourIsDes.LoadString(IDS_STRING_FILESOURISDES);//Ŀ���ļ�����Դ�ļ�,�������Ḳ��Դ�ļ�,�Ƿ����?
            if (MessageBox(cSourIsDes,cClew, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
            {
                return;
            }
        }
        
        if (!file.Open(cFileSour,CFile::modeRead,NULL))//��ʧ��
        {
            CString cOpenFail,cue;
            cOpenFail.LoadString(IDS_STRING_FINDSOURFILEFAIL);//Դ�ļ�%s������,������ѡ��
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
            cSour.LoadString(IDS_STRING_INPUTSOURFOLD);//������ԴĿ¼
            MessageBox(cSour,cClew,MB_ICONASTERISK |MB_OK);
            m_ComSour.SetFocus();
            return;
        }
        if (cFileDes == "")
        {
            CString cDes;
            cDes.LoadString(IDS_STRING_INPUTDESFOLD);//������Ŀ��Ŀ¼
            MessageBox(cDes,cClew,MB_ICONASTERISK |MB_OK);
            m_ComDes.SetFocus();
            return;
        }
        if (cFileSour == cFileDes)
        {
            CString cSourIsDes;
            cSourIsDes.LoadString(IDS_STRING_FOLDSOURISDES);//Ŀ��Ŀ¼����ԴĿ¼,�������Ḳ��Դ�ļ�,�Ƿ����?
            if(MessageBox(cSourIsDes,cClew,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
            {
                return;
            }
        }
        if (!SetCurrentDirectory(cFileSour))
        {
            CString cOpenFail,cue;
            cOpenFail.LoadString(IDS_STRING_FINDSOURFOLDFAIL);//Ŀ¼%s������,������ѡ��
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

void CFileAddEdit::GetFileName(CString & cFileNameSour,CString & cFileNameDes)//���Դ�ļ�,Ŀ���ļ�
{
    cFileNameSour = Item_Sour;
    cFileNameDes  = Item_Des;
}

BOOL CFileAddEdit::SaveFoldList(CComboBox & combox,CString sItem)//������Ŀ
{
	int i,iComboxCount;
	
	iComboxCount = combox.GetCount();
	for (i = iComboxCount - 1;i >= 0;i--)
	{
        CString	csFoldList;
		combox.GetLBText(i,csFoldList);
		if (sItem == csFoldList)//��Ŀ¼���б�����Ѿ�����
			combox.DeleteString(i);//ɾ����һ��
	}
	combox.InsertString(0,sItem);

	iComboxCount = combox.GetCount();
	if (iComboxCount > iItemMaxCount)//ɾ�������һ��
		combox.DeleteString(iComboxCount - 1);

    //////////////////////////////////////////////////////
    CString cPreType;//ǰ׺���
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
    CString     cSaveContent;//��Ҫ���������
    CFile       cfile,cstdofile;

    LPSTR       pResultAscii;
    LPCWSTR     lpMultiByteStr;
    int         iResultLen;

    cFileName = GetCurrentAppPath() + csSlash +IniFileName;//_T("\\")

    if (cstdofile.Open(cFileName,CFile::modeReadWrite))//�ļ�����
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

        //asciiת����unicode
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
                while ((cSingleLine.Right(1) == cNewLine1) ||//ɾ�����Ļس��� 13
                    (cSingleLine.Right(1) == cNewLine2))
                {
                    cSingleLine = cSingleLine.Left(cSingleLine.GetLength() - 1);
                }
                if (cSingleLine.Left(iPre) != cPreType)
                    cResult = cResult + cSingleLine + csNewLine;

                cUnicode = cUnicode.Right(cUnicode.GetLength() - iLine - 2);

                while ((cUnicode.Right(1) == cNewLine1) ||//ɾ�����Ļس��� 13
                    (cUnicode.Right(1) == cNewLine2))
                {
                    cUnicode = cUnicode.Left(cUnicode.GetLength() - 1);
                }
                while ((cUnicode.Left(1) == cNewLine1) ||//ɾ�����Ļس��� 13
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
            cSaveFile.LoadString(IDS_STRING_SAVEFILEFAIL);//�޷������ļ�
            cClew.LoadString(IDS_STRING_CLEW);//��ʾ
            MessageBox(cSaveFile,cClew,MB_ICONSTOP|MB_OK);
        }
        delete pResultAscii;
    }
    else//�ļ�������
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
            cSaveFile.LoadString(IDS_STRING_SAVEFILEFAIL);//�޷������ļ�
            cClew.LoadString(IDS_STRING_CLEW);//��ʾ
            MessageBox(cSaveFile,cClew,MB_ICONSTOP|MB_OK);
        }
    }
	return TRUE;
}

void CFileAddEdit::ReadFoldList(CComboBox & combox)//��ȡĿ¼�б�
{
	CFile       file;
    CString     cFileName;    
    CString     cPreType;//ǰ׺���
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
    //asciiת����unicode
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
        while ((cSingleLine.Right(1) == cNewLine1) || //ɾ�����Ļس���13
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

void CFileAddEdit::SetComboWidth(CComboBox & combox)//���������б��ֵĿ��
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
	//AnimateWindow(GetSafeHwnd(),1000,AW_CENTER|AW_HIDE);//�ڴ��ڹر�ʱ��ͬ���Զ����ķ�ʽ����
    //�����ð�����ô����?
	CDialog::OnCancel();
}
