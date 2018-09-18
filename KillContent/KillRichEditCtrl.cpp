// KillRichEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "killcontent.h"
#include "KillRichEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKillRichEditCtrl
extern void SetMouseLeftButtonStatus(void);

CKillRichEditCtrl::CKillRichEditCtrl()
{
}

CKillRichEditCtrl::~CKillRichEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CKillRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CKillRichEditCtrl)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKillRichEditCtrl message handlers

void CKillRichEditCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetMouseLeftButtonStatus();
	CRichEditCtrl::OnLButtonUp(nFlags, point);
}
