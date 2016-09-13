// dwload.h : main header file for the DWLOAD application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDwloadApp:
// See dwload.cpp for the implementation of this class
//
#define COM    ((CDwloadApp *)AfxGetApp())->m_Com
class CDwloadApp : public CWinApp
{
public:
	CDwloadApp();
	CComSer  m_Com;
    CString  param;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDwloadApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDwloadApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
