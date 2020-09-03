// PauseResumeDemo.h : main header file for the PAUSERESUMEDEMO application
//
/*
	This code is distributed as is with no warranty, Feel free to change use it in any way you like.
	Author is not responsible for anything.
	If you have any problem feel free to contact NarendraChandel@hotmail.com
	Have fun.
*/

#if !defined(AFX_PAUSERESUMEDEMO_H__FAD6C0E0_8A86_492C_9A09_A585BE861544__INCLUDED_)
#define AFX_PAUSERESUMEDEMO_H__FAD6C0E0_8A86_492C_9A09_A585BE861544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPauseResumeDemoApp:
// See PauseResumeDemo.cpp for the implementation of this class
//

class CPauseResumeDemoApp : public CWinApp
{
public:
	CPauseResumeDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPauseResumeDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPauseResumeDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAUSERESUMEDEMO_H__FAD6C0E0_8A86_492C_9A09_A585BE861544__INCLUDED_)
