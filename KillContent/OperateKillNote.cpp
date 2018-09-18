// OperateKillNote.cpp : implementation file
//

#include "stdafx.h"
#include "killcontent.h"
#include "OperateKillNote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int iItemFileWidch = 240;
static const int iItemSizeWidth = 50;
static const int iItemStatusWidth = 60;
static const int iItemTimeWidth = 40;
static const CString cTempExtName = _T(".tmp");
/////////////////////////////////////////////////////////////////////////////
// COperateKillNote dialog

COperateKillNote::COperateKillNote(CWnd* pParent /*=NULL*/)
    : CDialog(COperateKillNote::IDD, pParent)
{
    //{{AFX_DATA_INIT(COperateKillNote)
    //}}AFX_DATA_INIT
    OperateKillNote_Type = KILLFILES;
}


void COperateKillNote::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COperateKillNote)
    DDX_Control(pDX, IDC_STATIC_DES, m_Static_DesFile);
    DDX_Control(pDX, IDC_STATIC_SOUR, m_Static_SourFile);
    DDX_Control(pDX, IDC_STATIC_SCANTIME, m_ScanTime);
    DDX_Control(pDX, IDC_LIST_DETAIL, m_DetailList);
    DDX_Control(pDX, IDCANCEL, m_Cancel);
    DDX_Control(pDX, IDC_BUTTON_DETAIL, m_Detail);
    DDX_Control(pDX, IDC_EDIT_DESFILENAME, m_Edit_DesFile);
    DDX_Control(pDX, IDC_EDIT_SOURFILENAME, m_Edit_SourFile);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COperateKillNote, CDialog)
    //{{AFX_MSG_MAP(COperateKillNote)
    ON_BN_CLICKED(IDC_BUTTON_DETAIL, OnButtonDetail)
    ON_WM_TIMER()
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_DETAIL, OnDblclkListDetail)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDCANCEL, &COperateKillNote::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COperateKillNote message handlers
//���ò�������,��������ļ���ϻ���Ŀ¼�����
//cSourItems:Դ�ļ�����ԴĿ¼����ϣ��÷ֺŸ���
//cDesItems��Ŀ���ļ�����Ŀ��Ŀ¼����ϣ��÷ֺŸ���
BOOL COperateKillNote::SetOperateType(OPERATEKILLNOTE_TYPE operatekillnote_type,
                                      CString cSourItems,CString cDesItems)
{
    if ((operatekillnote_type != KILLFILES) && (operatekillnote_type != KILLPATHS))
        return FALSE;
    
    OperateKillNote_Type = operatekillnote_type;
    cSourceItem          = cSourItems;
    cIntentionItem       = cDesItems;
    return TRUE;
}

BOOL COperateKillNote::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    // TODO: Add extra initialization here  
    SetText();//��������

    ListFiles.RemoveAll();

    dKillOption = GetKillOption();//ɾ��ѡ��
    KillNoteStatus = KILLNOTEING;//����ɾ��ע��

    bOpenSourFile = FALSE;
    bOpenDesFile  = FALSE;

    WindowStatus  = NODETAIL;
    RECT rect,rMinRect;
    GetClientRect(&rect);//���ڴ�С

    WINDOWPLACEMENT WindowPlaceMent;

    memset(&WindowPlaceMent,0x00,sizeof(WINDOWPLACEMENT));
    rMinRect.top    = rect.top;
    rMinRect.left   = rect.left;
    rMinRect.right  = rect.right;
    rMinRect.bottom = rect.bottom - 230;

    rMinRect.top    = (GetSystemMetrics(SM_CYSCREEN) - rMinRect.bottom) / 2;
    rMinRect.left   = (GetSystemMetrics(SM_CXSCREEN) - rMinRect.right ) / 2;
    rMinRect.right  = rMinRect.left + rMinRect.right + 5;
    rMinRect.bottom = rMinRect.top + rMinRect.bottom;

    WindowPlaceMent.length = sizeof(WindowPlaceMent);
    WindowPlaceMent.flags = WPF_SETMINPOSITION;
    WindowPlaceMent.showCmd = SW_SHOW;
    WindowPlaceMent.rcNormalPosition = rMinRect;
    SetWindowPlacement(&WindowPlaceMent);

    //////////////////////////////////////////////////////////////////////////    
    rNoDetail       = rMinRect;
    rDetail.top     = rMinRect.top;//(GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2;
    rDetail.left    = (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2;
    rDetail.right   = rDetail.left + rect.right + 5;
    rDetail.bottom  = rDetail.top + rect.bottom + 26;
    //////////////////////////////////////////////////////////////////////////
    DWORD oldStyle = m_DetailList.GetExtendedStyle();
    m_DetailList.SetExtendedStyle(oldStyle |LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_GRIDLINES|LVS_EX_TRACKSELECT);

    CString cSourFile,cSize,cDesFile,cStatus,cTime;
    cSourFile.LoadString(IDS_STRING_SOURFILE);
    cSize.LoadString(IDS_STRING_SIZE);
    cDesFile.LoadString(IDS_STRING_DESFILE);
    cStatus.LoadString(IDS_STRING_STATUS);
    cTime.LoadString(IDS_STRING_COSTTIME);
    PauseClew.LoadString(IDS_STRING_PAUSECLEW);

    m_DetailList.InsertColumn(0,cSourFile,LVCFMT_LEFT,iItemFileWidch,0);//Դ�ļ�
    m_DetailList.InsertColumn(1,cSize,LVCFMT_LEFT,iItemSizeWidth,0);//��С
    m_DetailList.InsertColumn(2,cDesFile,LVCFMT_LEFT,iItemFileWidch,0);//Ŀ���ļ�
    m_DetailList.InsertColumn(3,cSize,LVCFMT_LEFT,iItemSizeWidth,0);//��С
    m_DetailList.InsertColumn(4,cStatus,LVCFMT_LEFT,iItemStatusWidth,0);//״̬
    m_DetailList.InsertColumn(5,cTime,LVCFMT_LEFT,iItemTimeWidth,0);//��ʱ

    dSourFileNumber = 0;
    dDesFileNumber  = 0;
    dSanByte        = 0;
    iUsedSecond     = 0;//ɨ���ʱ������
    iUsedMinute     = 0;
    iUsedHour       = 0;
    m_nTimer        = SetTimer(SCANING_TIMER, 1000, NULL);//�򿪼�ʱ����ʾʱ��
    //////////////////////////////////////////////////////////////////////////    
    InitializeCriticalSection(&lpcritical_section);

    DWORD dThread;//�̺߳�
    hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)KillNoteThread,//��ʼ�߳�
        this,0,&dThread);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

UINT COperateKillNote::KillNoteThread(LPVOID lpvData)
{
    COperateKillNote * pThis = (COperateKillNote*)lpvData;

    CString cSourFileNum,cDesFileNum;
    switch (pThis->OperateKillNote_Type)
    {
    case KILLFILES:
        while (pThis->KillNoteStatus == KILLNOTEING)
        {
            CString cSourFileName,cDesFileName;
            int iSpace;

            if ((pThis->cSourceItem == "") || (pThis->cIntentionItem == ""))
            {
                CString cScanfile,cOutfile,cClose;
                cScanfile.LoadString(IDS_STRING_SCANFILERESULT);
                cOutfile.LoadString(IDS_STRING_OUTFILERESULT);
                cClose.LoadString(IDS_STRING_CLOSE);

                cSourFileNum.Format(cScanfile,pThis->dSourFileNumber);//��ɨ���ļ�%ld��
                cDesFileNum.Format(cOutfile,pThis->dDesFileNumber);//������ļ�%ld��
                pThis->m_Edit_SourFile.SetWindowText(cSourFileNum);
                pThis->m_Edit_DesFile.SetWindowText(cDesFileNum);

                pThis->KillTimer(pThis->m_nTimer);
                pThis->m_nTimer = NULL;

                pThis->KillNoteStatus = STOPKILLNOTE;
                pThis->m_Cancel.SetWindowText(cClose);//�ر�

                pThis->ListFiles.RemoveAll();
                /*DWORD dThread;
                if (GetExitCodeThread(pThis->hThread,&dThread))
                {
                    CloseHandle(pThis->hThread);
                    ExitThread(dThread);
                }*/
                return 0;
            }

            iSpace = pThis->cSourceItem.Find(SPACESIGN);
            if (-1 == iSpace)
            {
                cSourFileName = pThis->cSourceItem;
                pThis->cSourceItem = "";
            }
            else
            {
                cSourFileName = pThis->cSourceItem.Left(iSpace);
                pThis->cSourceItem = pThis->cSourceItem.Right(pThis->cSourceItem.GetLength() - iSpace - 1);
            }

            iSpace = pThis->cIntentionItem.Find(SPACESIGN);
            if (-1 == iSpace)
            {
                cDesFileName = pThis->cIntentionItem;
                pThis->cIntentionItem = "";
            }
            else
            {
                cDesFileName = pThis->cIntentionItem.Left(iSpace);
                pThis->cIntentionItem = pThis->cIntentionItem.Right(pThis->cIntentionItem.GetLength() - iSpace - 1);
            }
            pThis->KillNoteFile(cSourFileName,cDesFileName,pThis->dKillOption);
        }
        break;
    case KILLPATHS:
        while (pThis->KillNoteStatus == KILLNOTEING)
        {
            CString cSourFoldName,cDesFoldName;
            int iSpace;

            if ((pThis->cSourceItem == "") || (pThis->cIntentionItem == ""))
            {
                CString cScanfile,cOutfile,cClose;
                cScanfile.LoadString(IDS_STRING_SCANFILERESULT);
                cOutfile.LoadString(IDS_STRING_OUTFILERESULT);
                cClose.LoadString(IDS_STRING_CLOSE);

                cSourFileNum.Format(cScanfile,pThis->dSourFileNumber);//��ɨ���ļ�%ld��
                cDesFileNum.Format(cOutfile,pThis->dDesFileNumber);//������ļ�%ld��
                pThis->m_Edit_SourFile.SetWindowText(cSourFileNum);
                pThis->m_Edit_DesFile.SetWindowText(cDesFileNum);

                pThis->KillTimer(pThis->m_nTimer);
                pThis->m_nTimer = NULL;

                pThis->KillNoteStatus = STOPKILLNOTE;
                pThis->m_Cancel.SetWindowText(cClose);//�ر�

                pThis->ListFiles.RemoveAll();
                return 0;
            }

            iSpace = pThis->cSourceItem.Find(SPACESIGN);
            if (-1 == iSpace)
            {
                cSourFoldName = pThis->cSourceItem;
                pThis->cSourceItem = "";
            }
            else
            {
                cSourFoldName = pThis->cSourceItem.Left(iSpace);
                pThis->cSourceItem = pThis->cSourceItem.Right(pThis->cSourceItem.GetLength() - iSpace - 1);
            }

            iSpace = pThis->cIntentionItem.Find(SPACESIGN);
            if (-1 == iSpace)
            {
                cDesFoldName = pThis->cIntentionItem;
                pThis->cIntentionItem = "";
            }
            else
            {
                cDesFoldName = pThis->cIntentionItem.Left(iSpace);
                pThis->cIntentionItem = pThis->cIntentionItem.Right(pThis->cIntentionItem.GetLength() - iSpace - 1);
            }
            //���ϲ��ֻ��ԴĿ¼��Ŀ��Ŀ¼
            CStringList listPaths;
            CString     strPath,strFilePath,filename,cExpend,cdesfilename;
            int         iExpend,iPreFoldSpace;
            BOOL        bWorking;
            CFileFind   find;

            iPreFoldSpace = cSourFoldName.ReverseFind(CH_SLASH1);//'\\'

            listPaths.RemoveAll();
            strPath = cSourFoldName + STR_STRING1;//"\\*.*"
            do 
            {
                bWorking = find.FindFile(strPath);
                while (bWorking && (pThis->KillNoteStatus == KILLNOTEING))
                {
                    bWorking    = find.FindNextFile();
                    strFilePath = find.GetFilePath();

                    if (find.IsDots())
                        continue;

                    if (find.IsDirectory())
                    {
                        listPaths.AddHead(strFilePath);
                        continue;
                    }
                    else
                    {
                        filename = find.GetFilePath();//����ļ���(����·��)
                        iExpend  = filename.ReverseFind(CH_POINT);//�����չ����ߵ�"."
                        cExpend  = filename.Right(filename.GetLength() - iExpend - 1);//�����չ��
                        cExpend.MakeLower();
                        if ((cExpend == _T("c")) || (cExpend == _T("h")) || (cExpend == _T("cpp")))
                        {
                            if (cSourFoldName == cDesFoldName)//Դ�ļ�����Ŀ���ļ�
                            {
                                if (!pThis->IsInListFiles(filename))
                                {
                                    pThis->KillNoteFile(filename,filename,pThis->dKillOption);
                                    pThis->ListFiles.AddHead(filename);
                                }
                            }
                            else if (iPreFoldSpace != -1)//Դ�ļ�����Ŀ���ļ�,��Ҫ����Ŀ���ļ�
                            {
                                cdesfilename = cDesFoldName + STR_STRING2 + //"\\"
                                    filename.Right(filename.GetLength() - iPreFoldSpace - 1);

                                if (!pThis->IsInListFiles(filename))
                                {
                                    pThis->KillNoteFile(filename,cdesfilename,pThis->dKillOption);
                                    pThis->ListFiles.AddHead(filename);
                                }
                            }
                        }
                    }
                }
                if(listPaths.IsEmpty())
                    break;

                strPath = listPaths.RemoveHead() + STR_STRING1;//"\\*.*"
                find.Close();
            }while (pThis->KillNoteStatus == KILLNOTEING);
        }
        break;
    }    
    return 0;
}

void COperateKillNote::OnCancel() 
{
    // TODO: Add extra cleanup here
    if (KillNoteStatus == KILLNOTEING)
    {
        CString cClew;
        CWaitCursor wait;
        Sleep(500);//������仰�Ļ��п��ܵ��µ���SuspendThread֮������쳣
        KillTimer(SCANING_TIMER);
        m_nTimer = NULL;

        cClew.LoadString(IDS_STRING_CLEW);
        if (SuspendThread(hThread) == 0xFFFFFFFF)
        {
            CString cSuspend;
            cSuspend.LoadString(IDS_STRING_SUSPENDTHREADERROR);            
            MessageBox(cSuspend,cClew,MB_ICONINFORMATION|MB_OK);//�߳���ֹ�쳣
        }
        //CString cCause;
        //cCause.LoadString(IDS_STRING_PAUSECLEW);
        if (AfxMessageBox(PauseClew, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)//�Ƿ���ֹ����?
        {
            m_nTimer = SetTimer(SCANING_TIMER, 1000, NULL);//�򿪼�ʱ����ʾʱ��
            ResumeThread(hThread);
            return;
        }

        KillNoteStatus = STOPKILLNOTE;

        ListFiles.RemoveAll();
        CloseHandle(hThread);
        CString cClose,cPause;
        cClose.LoadString(IDS_STRING_CLOSE);
        cPause.LoadString(IDS_STRING_PAUSE);

        m_Cancel.SetWindowText(cClose);//�ر�
        int iListCount = m_DetailList.GetItemCount();
        m_DetailList.SetItemText(iListCount - 1,4,cPause);//��ֹ
    }
    else
    {
        CDialog::OnCancel();
    }
}


//��һ���ļ���ɾ��ע��,dKillTypeɾ�����,��KillOption.h�ļ��ж���
BOOL COperateKillNote::KillNoteFile(CString cSourFileName,CString cDesFileName,
                                    DWORD dKillType)
{
    LPTSTR  lpstr;
    DWORD dFileSize;
    int iListCount = m_DetailList.GetItemCount();
    CString cFileSize = "";
    CFile fSourFile;
    CFile fDesFile;

    EnterCriticalSection(&lpcritical_section);//ʹ���ų���
    if (!fSourFile.Open(cSourFileName,CFile::modeRead))
    {
        CString cOpenFile;
        cOpenFile.LoadString(IDS_STRING_OPENFILEFAIL);
        m_DetailList.InsertItem(iListCount,cSourFileName);//���һ��
        m_DetailList.SetItemText(iListCount,4,cOpenFile);//���״̬��Ŀ,��ʧ��
        LeaveCriticalSection(&lpcritical_section);//�˳��ų���
        return FALSE;
    }
    dFileSize = fSourFile.GetLength();
    fSourFile.Close();
    LeaveCriticalSection(&lpcritical_section);//�˳��ų���

    ///////��ʾ�йص���Ϣ///////////////////////////////////////////////////////////////////        
    m_Edit_SourFile.SetWindowText(cSourFileName);
    m_Edit_DesFile.SetWindowText(cDesFileName);
    CString cScaning;
    cScaning.LoadString(IDS_STRING_SCANING);

    dScanSingleFileTime = 0;//ɨ��һ���ļ���ʱ
    dSourFileNumber++;//ɨ���ļ�������һ
    dSanByte += dFileSize;//��¼ɨ���ֽ�
    cFileSize.Format(FILESIZE,dFileSize);//��ǰ�ļ���С
    m_DetailList.InsertItem(iListCount,cSourFileName);//���һ��(Դ�ļ���)
    m_DetailList.SetItemText(iListCount,1,cFileSize);//����ļ�����
    m_DetailList.SetItemText(iListCount,4,cScaning);//ɨ����
    SIZE size;
    size.cx = 0;
    size.cy = iListCount + 1;
    m_DetailList.Scroll(size);
    //////////////////////////////////////////////////////////////////////////
    CString cTime,cEnd,cSecond;
    EnterCriticalSection(&lpcritical_section);//ʹ���ų���

    lpstr = new TCHAR[dFileSize * 2 + 1];
    memset(lpstr,0x00,dFileSize * 2 + 1);

    GetNoteStringUnicode(cSourFileName,dKillType,FALSE,BACK_SOURCE,
        (LPSTR)lpstr,&dFileSize);

    if (cSourFileName != cDesFileName)//Դ�ļ�����Ŀ���ļ�,ֱ������Ŀ���ļ�
    {
        CreateDirectoryFromString(cDesFileName);//���������Ŀ¼
        if (!fDesFile.Open(cDesFileName,CFile::modeCreate | CFile::modeWrite))
        {//�������ļ�
            delete lpstr;
            CString cOutFail;
            cOutFail.LoadString(IDS_STRING_OUTFILEFAIL);

            fDesFile.Close();
            m_DetailList.SetItemText(iListCount,4,cOutFail);//���ʧ��
            LeaveCriticalSection(&lpcritical_section);//�˳��ų���
            return FALSE;
        }
        fDesFile.Write(lpstr,dFileSize);
        dFileSize = fDesFile.GetLength();
        fDesFile.Close();
        cEnd.LoadString(IDS_STRING_END);
    }
    else//Դ�ļ�����Ŀ���ļ�,��ô��������һ����ʱ�ļ�,�ٽ�Դ�ļ�ɾ��,Ȼ����ʱ�ļ�����,��ΪԴ�ļ���
    {
        CString cTempName;

        cTempName = cDesFileName + cTempExtName;//������ʱ�ļ�
        if (!fDesFile.Open(cTempName,CFile::modeCreate | CFile::modeWrite))
        {//�������ļ�
            delete lpstr;
            CString cOutFail;
            cOutFail.LoadString(IDS_STRING_OUTFILEFAIL);

            fDesFile.Close();
            m_DetailList.SetItemText(iListCount,4,cOutFail);//���ʧ��
            LeaveCriticalSection(&lpcritical_section);//�˳��ų���
            return FALSE;
        }
        fDesFile.Write((const TCHAR *)lpstr,dFileSize);
        dFileSize = fDesFile.GetLength();
        fDesFile.Close();

        TRY
        {
            cEnd.LoadString(IDS_STRING_END);
            CFile::Remove(cSourFileName);//��Դ�ļ�ɾ��
            CFile::Rename(cTempName,cSourFileName);//����ʱ�ļ�����
        }
        CATCH( CFileException , e )
        {
            TRACE(cSourFileName + _T(" Output fail!"));
            cEnd.LoadString(IDS_STRING_OUTFILEFAIL);
        }
        END_CATCH
    }
    delete lpstr;
    LeaveCriticalSection(&lpcritical_section);//�˳��ų���

    //////////////////////////////////////////////////////////////////////////    
    cSecond.LoadString(IDS_STRING_SCANINGSECOND);

    cFileSize.Format(FILESIZE,dFileSize);
    cTime.Format(cSecond,dScanSingleFileTime);//%ld��
    m_DetailList.SetItemText(iListCount,2,cDesFileName);//���Ŀ���ļ�
    m_DetailList.SetItemText(iListCount,3,cFileSize);//���Ŀ���ļ�����
    m_DetailList.SetItemText(iListCount,4,cEnd);//���״̬,���
    m_DetailList.SetItemText(iListCount,5,cTime);//���ʱ��
    dDesFileNumber++;//����ļ�������һ
    //////////////////////////////////////////////////////////////////////////
    return TRUE;
}

void COperateKillNote::OnButtonDetail() 
{
    // TODO: Add your control notification handler code here
    WINDOWPLACEMENT WindowPlaceMent;
    CString cDetail1,cDetail2;

    cDetail1.LoadString(IDS_STRING_KILLDETAIL1);//��ϸ����>>
    cDetail2.LoadString(IDS_STRING_KILLDETAIL2);//��ϸ����<<

    memset(&WindowPlaceMent,0x00,sizeof(WINDOWPLACEMENT));
    WindowPlaceMent.length = sizeof(WindowPlaceMent);
    WindowPlaceMent.flags = WPF_SETMINPOSITION;
    WindowPlaceMent.showCmd = SW_SHOW;

    if (WindowStatus == DETAIL)//��ǰ����״̬Ϊ��ʾϸ��,ѡ��ť֮���л�Ϊ����ʾϸ��
    {
        WindowPlaceMent.rcNormalPosition = rNoDetail;
        WindowStatus = NODETAIL;
        m_Detail.SetWindowText(cDetail1);//��ϸ����>>
    }
    else//��ǰ����״̬Ϊ����ʾϸ��,ѡ��ť֮���л�Ϊ��ʾϸ��
    {
        WindowPlaceMent.rcNormalPosition = rDetail;
        WindowStatus = DETAIL;
        m_Detail.SetWindowText(cDetail2);//��ϸ����<<
    }
    SetWindowPlacement(&WindowPlaceMent);

    m_Cancel.SetFocus();
    DWORD style = m_Cancel.GetButtonStyle();
    style = style | BS_DEFPUSHBUTTON;
    m_Cancel.SetButtonStyle(style,TRUE);    
}

void COperateKillNote::OnTimer(UINT nIDEvent) 
{
    // TODO: Add your message handler code here and/or call default
    dScanSingleFileTime++;
    
    iUsedSecond++;
    if (iUsedSecond == 60)
    {
        iUsedMinute++;
        if (iUsedMinute == 60)
        {
            iUsedMinute = 0;
            iUsedHour++;
        }
        iUsedSecond = 0;
    }
    CString cTime,cScantext;

    cScantext.LoadString(IDS_STRING_SCANINGTIME);
    cTime.Format(cScantext,iUsedHour,iUsedMinute,iUsedSecond);//�Ѿ�ɨ����%02dСʱ%02d����%02d��
    m_ScanTime.SetWindowText(cTime);
    CDialog::OnTimer(nIDEvent);
}
//˫�����ļ�
void COperateKillNote::OnDblclkListDetail(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    POSITION pos = m_DetailList.GetFirstSelectedItemPosition();
    int nSelectedItem = m_DetailList.GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ

    CString cDesItem = m_DetailList.GetItemText(nSelectedItem,2);
    CString cStatus  = m_DetailList.GetItemText(nSelectedItem,4);
    CString cEnd;

    if (cDesItem == "")
        return;
    cEnd.LoadString(IDS_STRING_END);//���
    if (cStatus != cEnd)
    {
        CString cFailClew,cClew,cResult,cSourItem;

        cSourItem = m_DetailList.GetItemText(nSelectedItem,0);
        cFailClew.LoadString(IDS_STRING_FILEOUTFAIL);
        cClew.LoadString(IDS_STRING_CLEW);
        cResult.Format(cFailClew,cSourItem);
        MessageBox(cResult,cClew,MB_ICONERROR|MB_OK);
        return;
    }
    HINSTANCE hInst = ShellExecute(this->GetSafeHwnd(), _T("open"),
        cDesItem, NULL, NULL, SW_MAXIMIZE);
    if ((int)hInst < 32) //���ϵͳû����Ӧ�ĳ���򿪸����͵��ļ�������Notepad��
    {
        hInst = ShellExecute(this->GetSafeHwnd(), _T("open"), _T("notepad.exe"),
            cDesItem, NULL, SW_MAXIMIZE);
        if((int)hInst < 32)
        {
            CString cOpenFail,cClew;
            cOpenFail.LoadString(IDS_STRING_OPENFILEFAIL);
            cClew.LoadString(IDS_STRING_CLEW);
            MessageBox(cOpenFail,cClew,MB_ICONERROR |MB_OK);
        }
    }

    *pResult = 0;
}

void COperateKillNote::SetText(void)//��������
{
    CString cDetail,cBegin,cCancel,csSourFile,csDesFile;
    cDetail.LoadString(IDS_STRING_KILLDETAIL1);
    m_Detail.SetWindowText(cDetail);

    cBegin.LoadString(IDS_STRING_BEGINOPERATE);
    SetWindowText(cBegin);
    
    cCancel.LoadString(IDS_STRING_KILL_CANCEL);
    m_Cancel.SetWindowText(cCancel);
    
    csSourFile.LoadString(IDS_STRING_SOURFILE);
    m_Static_SourFile.SetWindowText(csSourFile);
    csDesFile.LoadString(IDS_STRING_DESFILE1);
    m_Static_DesFile.SetWindowText(csDesFile);
}

BOOL COperateKillNote::IsInListFiles(CString FileName)//���ļ��Ƿ����ļ�������
{
	int i,iStringListCount;
	CString csFile;
	POSITION ps;

	iStringListCount = ListFiles.GetCount();
	ps = ListFiles.GetHeadPosition();

    FileName.MakeLower();
	for (i = 0;i < iStringListCount;i++)
	{
		csFile = ListFiles.GetNext(ps);//��������е�һ��
		csFile.MakeLower();
		if (FileName == csFile)
			return TRUE;
	}
	return FALSE;
}
//���ݴ��������ļ�������Ŀ¼�����������Ŀ¼
//BOOL COperateKillNote::CreateDirectoryFromString(CString csPath)
//{
//    TCHAR CurrentDirectory[256];
//    GetCurrentDirectory(sizeof(CurrentDirectory),CurrentDirectory);
//
//    CString singlepath = "";
//    int ispace;
//
//    ispace = csPath.Find(FOLDSPACESIGN);
//
//    if (-1 == ispace)
//    {
//        SetCurrentDirectory(CurrentDirectory);
//        return FALSE;
//    }
//    while (ispace != -1)
//    {
//        if (singlepath != "")
//            singlepath = singlepath + STR_STRING2 + csPath.Left(ispace);//���ĳһ��Ŀ¼��"\\"
//        else
//            singlepath = csPath.Left(ispace);//���ĳһ��Ŀ¼��
//
//        if (!SetCurrentDirectory(singlepath))//Ŀ¼������
//        {
//            if (!CreateDirectory(singlepath,NULL))
//            {
//                SetCurrentDirectory(CurrentDirectory);
//                return FALSE;
//            }
//        }
//        csPath = csPath.Right(csPath.GetLength() - ispace - 1);
//        ispace = csPath.Find(FOLDSPACESIGN);
//    }
//    SetCurrentDirectory(CurrentDirectory);
//
//    return TRUE;
//}



void COperateKillNote::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    COperateKillNote::OnCancel();
}
