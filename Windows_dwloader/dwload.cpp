// dwload.cpp : Defines the class behaviors for the application.
//


#include "stdafx.h"
#include "Comser.h"
#include "dwload.h"
#include "filedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDwloadApp

BEGIN_MESSAGE_MAP(CDwloadApp, CWinApp)
	//{{AFX_MSG_MAP(CDwloadApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDwloadApp construction

CDwloadApp::CDwloadApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDwloadApp object

CDwloadApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDwloadApp initialization

BOOL CDwloadApp::InitInstance()
{
    CCommandLineInfo rCmdInfo;
    CWinApp::ParseCommandLine(rCmdInfo);
    param = rCmdInfo.m_strFileName;
    CFileDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return TRUE;
}
