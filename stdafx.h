// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__434EA653_F692_4EDB_A459_8314C0677D18__INCLUDED_)
#define AFX_STDAFX_H__434EA653_F692_4EDB_A459_8314C0677D18__INCLUDED_

// Change these values to use different versions
#define WINVER		0x0501
//#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0600

#if _MSC_VER>1600
#define _RICHEDIT_VER	0x0300
#else
#define _RICHEDIT_VER	0x0200
#endif

#include <atlbase.h>

#if _MSC_VER>1600
#include <atlapp.h>
#include <atlstr.h>
using ATL::CString;
#else
#define _WTL_USE_CSTRING
#include <atlapp.h>
#include <atlmisc.h>
using WTL::CString;
#endif


extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__434EA653_F692_4EDB_A459_8314C0677D18__INCLUDED_)
