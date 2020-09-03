// PauseResumeDemoDlg.h : header file
//
/*
	This code is distributed as is with no warranty, Feel free to change use it in any way you like.
	Author is not responsible for anything.
	If you have any problem feel free to contact NarendraChandel@hotmail.com
	Have fun.
*/


#if !defined(AFX_PAUSERESUMEDEMODLG_H__6F5A5632_8D9F_4261_A81F_39D0BD8FF59E__INCLUDED_)
#define AFX_PAUSERESUMEDEMODLG_H__6F5A5632_8D9F_4261_A81F_39D0BD8FF59E__INCLUDED_

#include "WinInet.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPauseResumeDemoDlg dialog

#define PUMPMESSAGE()\
		{\
			MSG msg;\
			while( PeekMessage( &msg, NULL, NULL, NULL, PM_REMOVE ) )\
			{\
				TranslateMessage(&msg);\
				DispatchMessage(&msg);\
			}\
		}


class CPauseResumeDemoDlg : public CDialog
{
// Construction
public:
	BOOL m_bStopped;
	CPauseResumeDemoDlg(CWnd* pParent = NULL);	// standard constructor
	bool StartDownloadingHardCodedFile();
	void GetLastResponse(string& strResponse);
	bool  IfLocalFileExist(const char *pszFilePath, double *pdwFileSize);
	bool FindPauseResumeSupported(HINTERNET hConnect);

// Dialog Data
	//{{AFX_DATA(CPauseResumeDemoDlg)
	enum { IDD = IDD_PAUSERESUMEDEMO_DIALOG };
	CProgressCtrl	m_ProgressCtl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPauseResumeDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPauseResumeDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAUSERESUMEDEMODLG_H__6F5A5632_8D9F_4261_A81F_39D0BD8FF59E__INCLUDED_)
