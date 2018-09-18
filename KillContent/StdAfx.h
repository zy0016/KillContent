// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__215F11AB_3E8C_4434_B9E9_0498D8E4BD65__INCLUDED_)
#define AFX_STDAFX_H__215F11AB_3E8C_4434_B9E9_0498D8E4BD65__INCLUDED_

#undef WINVER
#define WINVER 0X5000
#define _WIN32_WINNT 0x05000

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#undef _RICHEDIT_VER
//#define _RICHEDIT_VER 0x200
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__215F11AB_3E8C_4434_B9E9_0498D8E4BD65__INCLUDED_)
