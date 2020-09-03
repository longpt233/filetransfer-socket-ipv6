// PauseResumeDemo.cpp : Defines the class behaviors for the application.
//
/*
	This code is distributed as is with no warranty, Feel free to change use it in any way you like.
	Author is not responsible for anything.
	If you have any problem feel free to contact NarendraChandel@hotmail.com
	Have fun.
*/

#include "stdafx.h"
#include "PauseResumeDemo.h"
#include "PauseResumeDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPauseResumeDemoApp

BEGIN_MESSAGE_MAP(CPauseResumeDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CPauseResumeDemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPauseResumeDemoApp construction

CPauseResumeDemoApp::CPauseResumeDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPauseResumeDemoApp object

CPauseResumeDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPauseResumeDemoApp initialization

BOOL CPauseResumeDemoApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CPauseResumeDemoDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
