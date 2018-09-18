#include "stdafx.h"
#include "GenNotesList.h"
#include "TCHAR.h"

static BOOL IsNoteFuncHeader(LPWSTR pComment,DWORD dCur,DWORD dFileLen);

BOOL GetNotesList_Unicode(LPCTSTR pathname, struct strNotesList **NotesList)
{
	CFile notesfile;
	TCHAR ch;
	TCHAR chold = 0;
	TCHAR QuotationFlag = 0;
	DWORD dwRead = 0;
	UCHAR flag;
	struct strNotesList *pCurNote = NULL;
	struct strNotesList *pNewNote = NULL;
	DWORD flen;
	char *buf1=NULL;
	LPWSTR buf=NULL;
	DWORD curp=0;
	int   CrLnCount = 0;

	*NotesList = NULL;
	if (!notesfile.Open(pathname,CFile::modeRead))
		return FALSE;
	flen = notesfile.GetLength();
	buf1 = new char[flen+1];
	buf = new TCHAR[flen+1];
//	dwRead = notesfile.ReadHuge(buf1,flen);////remove for vs 2010
    dwRead = notesfile.Read(buf1,flen);
    if (buf1[0] == 0xFEFF)//��ǰ�ļ�����UNICODE��ʽ
    {
        memcpy(buf,buf1,flen);
    }
    else
    {
        MultiByteToWideChar(936,MB_PRECOMPOSED,(LPCSTR)buf1,-1,	buf,flen);
    }
	flag = NON_COMMENT;
	do
	{
		ch = buf[curp++];
		if (ch == CH_R)
			CrLnCount++;
		if (ch == INVERTED || ch == DMARKS)
			if (QuotationFlag == 0)
				QuotationFlag = ch;
			else if (QuotationFlag == ch)
				QuotationFlag = 0;
		if (ch == CH_SLASH && QuotationFlag == 0 && (flag & 0x7F) == NON_COMMENT)
		{
			if (curp<2)
				flag |= SKIP_COMMENT;
			else if (buf[curp-2] == CH_R || buf[curp-2] == CH_N)
				flag |= SKIP_COMMENT;

			ch = buf[curp++];
			if (ch == CH_R)
				CrLnCount++;
			switch(ch)
			{
			case CH_SLASH:
				flag |= LINE_COMMENT;
				pNewNote = new struct strNotesList;
				pNewNote->NextNote = NULL;
				pNewNote->LocStart = curp-2-CrLnCount;
				pNewNote->NotesType = flag;
				if (!*NotesList)
					*NotesList = pNewNote;
				if (pCurNote)
					pCurNote->NextNote = pNewNote;
				pCurNote = pNewNote;
				break;
			case CH_ASTERISK:
				flag |= BLOCK_COMMENT;
				pNewNote = new struct strNotesList;
				pNewNote->NextNote = NULL;
				pNewNote->LocStart = curp-2-CrLnCount;
				pNewNote->NotesType = flag;
				if (!*NotesList)
					*NotesList = pNewNote;
				if (pCurNote)
					pCurNote->NextNote = pNewNote;
				pCurNote = pNewNote;
				break;
			default:
				flag = NON_COMMENT;
			}
		}
		switch(flag & 0x7F)
		{
		case LINE_COMMENT:
			if (ch == CH_R)
			{
				pCurNote->LocEnd = curp-CrLnCount;
				flag = NON_COMMENT;
			}
			break;
		case BLOCK_COMMENT:
			if (ch == CH_SLASH && chold == CH_ASTERISK)
			{
				pCurNote->LocEnd = curp-CrLnCount;
				flag = NON_COMMENT;
			}
			break;
		default:
			break;
		}
		chold = ch;
	}
	while (curp < flen);
	notesfile.Close();
	delete buf;
    delete buf1;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//���ɾ��ע��֮����ļ�����,
//pathname:�ļ���,dKillType:ɾ������,
//bAddSpace�Ƿ���ע�����ڵ�λ�ò���ո�
//BackCodeType:��Ҫ���ص��ַ����ı����ʽ
//pAsciiString:�������ɵ�ascii�ַ���
//dAsciiLen:���ɵ��ַ����ĳ���
CString GetNoteStringUnicode(LPCTSTR pathname,DWORD dKillType,BOOL bAddSpace,
                             BACKCODETYPE BackCodeType,LPSTR pAsciiString,DWORD * dAsciiLen)
{
    CFile notesfile;
	TCHAR ch,chold,ch_pre;
    DWORD flen,curp = 0;
	enum enumNotesFlags flag;
    unsigned char * pBufAscii = NULL;
    LPWSTR pBufUnicode = NULL;
    CString cNoneNoteCode = "";
    BOOL bQuotationFlag = FALSE;//�Ƿ�ڶ��ζ�ȡ�ļ��ַ�
    BOOL bFuncHeader = FALSE;//�Ƿ���ں���ͷע��
    CString cComment = "";//�ļ�ͷע������
    CODETYPE CodeType;//��ǰ�ļ������ʽ

    if (!notesfile.Open(pathname,CFile::modeRead))
		return cNoneNoteCode;

	flen        = notesfile.GetLength();
    pBufAscii   = new unsigned char[flen + 1];
    pBufUnicode = new TCHAR[flen + 1];
    memset(pBufAscii,0x00,flen + 1);
    memset(pBufUnicode,0x00,flen + 1);
//    notesfile.ReadHuge(pBufAscii,flen);//remove for vs 2010
    notesfile.Read(pBufAscii,flen);
    notesfile.Close();
    if ((pBufAscii[0] == 0xFF) && (pBufAscii[1] == 0xFE))
    {
        memcpy(pBufUnicode,pBufAscii + 2,flen);
        CodeType = CODE_UNICODE;
    }
    else
    {
        MultiByteToWideChar(936,MB_PRECOMPOSED,(LPCSTR)pBufAscii,-1,pBufUnicode,flen);
        CodeType = CODE_ASCII;
    }
    delete pBufAscii;

	flag = NON_COMMENT;

    if (dKillType & KILL_HOLDFILEHEADER)
    {
        while (pBufUnicode[curp])//�����Ƿ�����ļ�ͷע��
        {
            ch = pBufUnicode[curp];
            if ((ch == CH_SPACE) || (ch == CH_TAB) || (ch == CH_R) ||(ch == CH_N))
            {
                cNoneNoteCode += ch;
                curp++;
                continue;
            }
            if (ch == CH_SLASH)
            {
                curp++;
                if (pBufUnicode[curp] == CH_SLASH)//�ҵ��ļ�ͷע��
                {
                    cNoneNoteCode = cNoneNoteCode + ch + pBufUnicode[curp];
                    curp++;
                    while (pBufUnicode[curp])//��ʼ����ɨ��
                    {
                        cNoneNoteCode += pBufUnicode[curp];
                        if ((pBufUnicode[curp] == CH_R) || (pBufUnicode[curp] == CH_N))
                            break;

                        curp++;
                    }
                }
                else if (pBufUnicode[curp] == CH_ASTERISK)//�ҵ��ļ�ͷע��
                {
                    chold = pBufUnicode[curp];
                    cNoneNoteCode = cNoneNoteCode + ch + pBufUnicode[curp];
                    curp++;
                    while (1)
                    {
                        if (pBufUnicode[curp] == NULL)
                            break;
                        ch = pBufUnicode[curp];
                        cNoneNoteCode += ch;
                        curp++;
                        if ((ch == CH_SLASH) && (chold == CH_ASTERISK))
                        {
                            cNoneNoteCode += pBufUnicode[curp];
                            break;
                        }
                        chold = ch;
                    }
                }
                else//�������ļ�ͷע��
                {
                    break;
                }
            }
            else//�������ļ�ͷע��
            {
                break;
            }
            curp++;
        }
    }
    while (1)
    {
        ch = pBufUnicode[curp++];
        if ((curp > flen) || (ch == 0x00))
            break;

        ch_pre = ch;

        PushQuotation(ch);
        bQuotationFlag = FALSE;
        if ((ch == CH_SLASH) && (!GetQuotation()) && (flag == NON_COMMENT))//'/'
        {
            bQuotationFlag = TRUE;//�ڶ��ζ�ȡ�ļ��ַ�
            ch = pBufUnicode[curp++];

            if ((curp > flen) || (ch == 0x00))
                break;

            switch(ch)
			{
			case CH_SLASH://'/'
				flag = LINE_COMMENT;
                if (dKillType & KILL_HOLDFUNCHEADER)
                {
                    LPWSTR pComment = &pBufUnicode[curp];
                    cComment = cComment + STR_SLASH + STR_SLASH;
                    while (* pComment)//��¼ע����
                    {
                        if ((* pComment == CH_R) || (* pComment == CH_N))
                        {
                            pComment++;
                            cComment = cComment + CH_R/* + CH_N*/;
                            break;
                        }
                        cComment += * pComment;
                        pComment++;
                    }
                    pComment++;
                    bFuncHeader = IsNoteFuncHeader(pComment,curp,flen);
                    //��������ͷע�͵�ʱ��,�жϵ�ǰ��ע�����Ƿ��Ǻ�������
                    if (bFuncHeader)
                    {
                        cNoneNoteCode += cComment;
                    }
                    cComment = "";
                }
				break;
			case CH_ASTERISK://'*'
				flag = BLOCK_COMMENT;
                if (dKillType & KILL_HOLDFUNCHEADER)
                {
                    LPWSTR pComment = &pBufUnicode[curp];
                    TCHAR tchold = 0,tch;
                    cComment += STR_ASTERISKSLASH;//_T("/*");
                    while (* pComment)
                    {
                        tch = * pComment;
                        cComment += tch;
                        if ((tch == CH_SLASH) && (tchold == CH_ASTERISK))
                        {
                            pComment++;
                            break;
                        }
                        tchold = * pComment;
                        pComment++;
                    }
                    while ((* pComment == CH_R) || (* pComment == CH_N))
                        pComment++;

                    pComment++;
                    bFuncHeader = IsNoteFuncHeader(pComment,curp,flen);
                    //��������ͷע�͵�ʱ��,�жϵ�ǰ��ע�����Ƿ��Ǻ�������
                    if (bFuncHeader)
                        cNoneNoteCode += cComment;

                    cComment = "";
                }
				break;
			default:
				flag = NON_COMMENT;
			}
        }
        switch(flag)
		{
		case LINE_COMMENT:
            if ((ch == CH_R) || (ch == CH_N))
            {
                flag = NON_COMMENT;
                if (!bFuncHeader)
                    cNoneNoteCode += ch;
            }
			break;
		case BLOCK_COMMENT:
			if (ch == CH_SLASH && chold == CH_ASTERISK)
			{
				flag = NON_COMMENT;
			}
            else
            {
                if ((ch == CH_R) || (ch == CH_N))
                {
                    if (!bFuncHeader)
                        cNoneNoteCode += ch;
                }
            }
			break;
		default:
            if (bQuotationFlag)
                cNoneNoteCode = cNoneNoteCode + ch_pre + ch;
            else
                cNoneNoteCode += ch;
			break;
		}
		chold = ch;
    }
    delete pBufUnicode;
    //��Ҫ���ص��ַ����ı����ʽ
    if (BackCodeType == BACK_UNICODE)//����UNICODE��ʽ
        return cNoneNoteCode;

    if (BackCodeType == BACK_ASCII)//����ASCII���ʽ
    {
        if (pAsciiString != NULL)
        {
            * dAsciiLen = UnicodeToAsciiString(cNoneNoteCode,pAsciiString);
        }
        return cNoneNoteCode;
    }
    else if (BackCodeType == BACK_SOURCE)//����ԭ���ĸ�ʽ
    {
        if (CodeType == CODE_UNICODE)
            return cNoneNoteCode;

        if (pAsciiString != NULL)
        {
            * dAsciiLen = UnicodeToAsciiString(cNoneNoteCode,pAsciiString);
        }
        return cNoneNoteCode;
    }
    else
    {
        return cNoneNoteCode;
    }
}
//��unicode������ַ���ת����ascii��ʽ
DWORD UnicodeToAsciiString(CString cUnicode,LPSTR pAsciiString)
{
    LPSTR   pResultAscii;
    LPCWSTR lpMultiByteStr;
    DWORD   iResultLen;

    iResultLen = cUnicode.GetLength();

    lpMultiByteStr = (LPCWSTR)cUnicode.GetBuffer(iResultLen);
    pResultAscii = new CHAR[iResultLen * 2 + 1];
    memset(pResultAscii,0x00,iResultLen * 2 + 1);
    WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,
        lpMultiByteStr,iResultLen,pResultAscii,iResultLen * 2 + 1,NULL,NULL);

    iResultLen = strlen(pResultAscii);
    memcpy(pAsciiString,pResultAscii,iResultLen + 1);
    cUnicode.ReleaseBuffer();

    delete pResultAscii;
    return iResultLen;
}
//��������ͷע�͵�ʱ��,�жϵ�ǰ��ע�����Ƿ��Ǻ�������
//pComment:ɨ����ʼλ��
static BOOL IsNoteFuncHeader(LPWSTR pComment,DWORD dCur,DWORD dFileLen)
{
    CString cComment = "";
    BOOL bFuncHeader = FALSE;
    unsigned int iBracket = 0;//��¼�����ŵĸ���,����1��ζ�ŵ�ǰ�в��Ǻ���ͷ

    if (pComment != NULL)
    {
        while (((* pComment == CH_R) || (* pComment == CH_N)) && 
            (* pComment) && (dCur < dFileLen))
        {
            pComment++;
            dCur++;
        }
        if (((* pComment == CH_SPACE) || (* pComment == CH_TAB) || (* pComment == CH_SLASH)) && 
            (* pComment))
            return FALSE;

        while ((* pComment) && (dCur <dFileLen))
        {
            if ((* pComment == CH_R) || (* pComment == CH_N))
                break;
            if ((* pComment == CH_SEMICOLON) || (* pComment == CH_WELL))
            {
                bFuncHeader = FALSE;
                break;
            }
            if (* pComment == CH_BRACKET_L)
            {
                if (iBracket == 0)
                {
                    bFuncHeader = TRUE;
                    iBracket++;
                }
                else
                {
                    bFuncHeader = FALSE;
                    break;
                }
            }
            pComment++;
            dCur++;
        }
    }
    return bFuncHeader;
}
//////////////////////////////////////////////////////////////////////////
static TCHAR cQuotation[3];
BOOL GetQuotation(void)//�Ƿ��ڵ�ǰ�ַ���ǰ�ߴ��ڵ����Ż�����˫����
{
    return (_tcslen(cQuotation));        
}
BOOL PushQuotation(TCHAR ch)//�����ź�˫���ŵ���ջ����
{
    if (ch == DMARKS)//'"'
    {
        if ((cQuotation[0] == CHEND) && (cQuotation[1] == CHEND))//'\0'
        {
            cQuotation[0] = DMARKS;//'"'
            cQuotation[1] = CHEND;//'\0'
        }
        else if (cQuotation[0] == DMARKS)//'"'
        {
            memset(cQuotation,0x00,sizeof(cQuotation));
        }
        else if (cQuotation[0] == INVERTED)//'\''
        {
            cQuotation[1] = DMARKS;//'"'
        }
        return TRUE;
    }
    else if (ch == INVERTED)//'\''
    {
        if ((cQuotation[0] == CHEND) && (cQuotation[1] == CHEND))//'\0'
        {
            cQuotation[0] = INVERTED;//'\''
            cQuotation[1] = CHEND;//'\0'
        }
        else if (cQuotation[0] == INVERTED)//'\''
        {
            memset(cQuotation,0x00,sizeof(cQuotation));
        }
        else if (cQuotation[0] == DMARKS)//'"'
        {
            cQuotation[1] = INVERTED;//'\''
        }
        return TRUE;
    }
    else if ((ch == CH_R) || (ch == CH_N))//'\r'//'\n'
    {
        memset(cQuotation,0x00,sizeof(cQuotation));
        return TRUE;
    }
    return FALSE;
}
//���ݴ��������ļ�������Ŀ¼�����������Ŀ¼
BOOL CreateDirectoryFromString(CString csPath)
{
    TCHAR CurrentDirectory[256];
    GetCurrentDirectory(sizeof(CurrentDirectory),CurrentDirectory);

    CString singlepath = "";
    int ispace;

    ispace = csPath.Find(FOLDSPACESIGN);

    if (-1 == ispace)
    {
        SetCurrentDirectory(CurrentDirectory);
        return FALSE;
    }
    while (ispace != -1)
    {
        if (singlepath != "")
            singlepath = singlepath + STR_STRING2 + csPath.Left(ispace);//���ĳһ��Ŀ¼��"\\"
        else
            singlepath = csPath.Left(ispace);//���ĳһ��Ŀ¼��

        if (!SetCurrentDirectory(singlepath))//Ŀ¼������
        {
            if (!CreateDirectory(singlepath,NULL))
            {
                SetCurrentDirectory(CurrentDirectory);
                return FALSE;
            }
        }
        csPath = csPath.Right(csPath.GetLength() - ispace - 1);
        ispace = csPath.Find(FOLDSPACESIGN);
    }
    SetCurrentDirectory(CurrentDirectory);

    return TRUE;
}
//BOOL GetNotesList(LPCTSTR pathname, struct strNotesList **NotesList)
//{
//    CFile notesfile;
//	TCHAR ch;
//	TCHAR chold = 0;
//	BOOL QuotationFlag = FALSE;
//    DWORD flen;
//    DWORD curp = 0;
//    int CrLnCount = 0;
//	enum enumNotesFlags flag;
//    char * pBufAscii = NULL;
//    LPWSTR pBufUnicode = NULL;
//	struct strNotesList *pCurNote = NULL;
//	struct strNotesList *pNewNote = NULL;
//
//	*NotesList = NULL;
//	if (!notesfile.Open(pathname,CFile::modeRead))
//		return FALSE;
//
//    flen = notesfile.GetLength();
//    pBufAscii = new char[flen + 1];
//    pBufUnicode = new TCHAR[flen + 1];
//    notesfile.ReadHuge(pBufAscii,flen);
//    notesfile.Close();
//    if (pBufAscii[0] == 0xFEFF)
//    {
//        memcpy(pBufUnicode,pBufAscii,flen);
//    }
//    else
//    {
//        MultiByteToWideChar(936,MB_PRECOMPOSED,(LPCSTR)pBufAscii,-1,pBufUnicode,flen);
//    }
//
//	flag = NON_COMMENT;
//    while (1)
//    {
//        ch = pBufUnicode[curp++];
//        if (curp > flen)
//            break;
//
//        if (ch == CH_R)
//            CrLnCount++;
//
//        PushQuotation(ch);
//        QuotationFlag = GetQuotation();
//
//        if ((ch == CH_SLASH) && (!QuotationFlag) && (flag == NON_COMMENT))//'/'
//        {
//            ch = pBufUnicode[curp++];
//            if (curp > flen)
//                break;
//
//            if (ch == CH_R)
//                CrLnCount++;
//
//            switch(ch)
//			{
//			case CH_SLASH://'/'
//				flag = LINE_COMMENT;
//				pNewNote = new struct strNotesList;
//				pNewNote->NextNote = NULL;
//				pNewNote->LocStart = curp - 2 - CrLnCount;
//				pNewNote->NotesType = flag;
//				if (!*NotesList)
//					*NotesList = pNewNote;
//				if (pCurNote)
//					pCurNote->NextNote = pNewNote;
//				pCurNote = pNewNote;
//				break;
//			case CH_ASTERISK://'*'
//				flag = BLOCK_COMMENT;
//				pNewNote = new struct strNotesList;
//				pNewNote->NextNote = NULL;
//				pNewNote->LocStart = curp - 2 - CrLnCount;
//				pNewNote->NotesType = flag;
//				if (!*NotesList)
//					*NotesList = pNewNote;
//				if (pCurNote)
//					pCurNote->NextNote = pNewNote;
//				pCurNote = pNewNote;
//				break;
//			default:
//				flag = NON_COMMENT;
//			}
//        }
//        switch(flag)
//		{
//		case LINE_COMMENT:
//			if (ch == CH_N)//0x0A
//			{
//				pCurNote->LocEnd = curp - CrLnCount;
//				flag = NON_COMMENT;
//			}
//			break;
//		case BLOCK_COMMENT:
//			if (ch == CH_SLASH && chold == CH_ASTERISK)//'/'//'*'
//			{
//				pCurNote->LocEnd = curp - CrLnCount;
//				flag = NON_COMMENT;
//			}
//			break;
//		default:
//			break;
//		}
//		chold = ch;
//    }
//    delete pBufAscii;
//    delete pBufUnicode;
//    return TRUE;
//}
