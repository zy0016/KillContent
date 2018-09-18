#ifndef _KILLNOTELIST_
#define _KILLNOTELIST_

#define DMARKS              _T('"')
#define CHEND               _T('\0')
#define INVERTED            _T('\'')
#define CH_R                _T('\r')
#define CH_N                _T('\n')
#define CH_SLASH            _T('/')
#define CH_ASTERISK         _T('*')
#define CH_SPACE            _T(' ')
#define CH_TAB              _T('\t')
#define CH_SEMICOLON        _T(';')
#define CH_BRACKET_L        _T('(')
#define CH_SLASH1			_T('\\')
#define CH_POINT			_T('.')
#define CH_WELL             _T('#')

#define STR_SLASH           _T("/")
#define STR_ASTERISKSLASH   _T("/*")
#define STR_STRING1			_T("\\*.*")
#define STR_STRING2			_T("\\")
#define FOLDSPACESIGN       _T("\\")
struct strNotesList 
{
	ULONG				    LocStart;
	ULONG				    LocEnd;
	UCHAR				    NotesType;
	struct strNotesList	   * NextNote;
};

enum enumNotesFlags
{
	NON_COMMENT,
	LINE_COMMENT,
	BLOCK_COMMENT,
};
typedef enum
{
	CODE_ASCII,
	CODE_UNICODE
}CODETYPE;//��ǰ��������

typedef enum
{
	BACK_ASCII,//ASCII����
	BACK_UNICODE,//UNICODE����
	BACK_SOURCE,//�ļ�ԭ��������
}BACKCODETYPE;//�������ݵı�������

typedef enum
{
    KILLALL             = 0x00, //ɾ������ע��
    KILL_HOLDFILEHEADER = 0x01, //�����ļ�ͷע��
    KILL_HOLDFUNCHEADER = 0x02, //��������ͷע��
}KILLOPTION;

//enum enumNotesFlags
//#define	NON_COMMENT		0x00
//#define LINE_COMMENT	0x01
//#define BLOCK_COMMENT	0x02
#define SKIP_COMMENT	0x80

//////////////////////////////////////////////////////////////////////////
BOOL PushQuotation(TCHAR ch);
BOOL GetQuotation(void);
BOOL GetNotesList(LPCTSTR pathname, struct strNotesList **NotesList);
BOOL GetNotesList_Unicode(LPCTSTR pathname, struct strNotesList **NotesList);
CString GetNoteStringUnicode(LPCTSTR pathname,DWORD dKillType,BOOL bAddSpace,
                             BACKCODETYPE BackCodeType,LPSTR pAsciiString,DWORD * dAsciiLen);
DWORD UnicodeToAsciiString(CString cUnicode,LPSTR pAsciiString);
BOOL CreateDirectoryFromString(CString csPath);

#endif
