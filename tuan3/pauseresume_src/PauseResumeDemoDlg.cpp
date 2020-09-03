/*
	This code is distributed as is with no warranty, Feel free to change use it in any way you like.
	Author is not responsible for anything.
	If you have any problem feel free to contact NarendraChandel@hotmail.com
	Have fun.
*/



#include "stdafx.h"
#include "PauseResumeDemo.h"
#include "PauseResumeDemoDlg.h"
#include <math.h>

#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPauseResumeDemoDlg dialog

CPauseResumeDemoDlg::CPauseResumeDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPauseResumeDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPauseResumeDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bStopped = FALSE;

}

void CPauseResumeDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPauseResumeDemoDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPauseResumeDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CPauseResumeDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPauseResumeDemoDlg message handlers

BOOL CPauseResumeDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_STATIC_MSG)->SetWindowText("Sample program to demonstrate how Pause and Resume can be implemented. Click 'Start' button any time to start the download. This program is hard coded to connect to ftp://gnu.org and download the file 'find-ls.txt.gz' from root directory. Once started you can click 'Stop' to stop the download. Start and Stop it as many times until download is completed.");
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPauseResumeDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPauseResumeDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPauseResumeDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPauseResumeDemoDlg::OnButton1() 
{
	StartDownloadingHardCodedFile();	
}

void CPauseResumeDemoDlg::GetLastResponse(string& strResponse)
{
	DWORD dwError;
	TCHAR tpszBuff[2*1024];
	DWORD dwBuffLen = 2*1024;
	BOOL bRet = ::InternetGetLastResponseInfo(&dwError, tpszBuff, &dwBuffLen); 
	strResponse = tpszBuff;
}

#define NKBYTE 10

bool CPauseResumeDemoDlg::StartDownloadingHardCodedFile() 
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);

	
	DWORD dwPacketSize =5;

	DWORD dwSize, dwToRead = dwPacketSize * 1024;
	PBYTE  pBuffer  = new BYTE[dwPacketSize * 1024];

	double dFileSize = 352256;
	double dOffsetToSeek =0;
	
	HINTERNET  hConnect = NULL;
	HINTERNET hOpen = NULL;
	HINTERNET	hOpenFile = NULL;
	BOOL bRet = FALSE;
	BOOL bInComplete = FALSE;
	INTERNET_PORT nServerPort =21;
	FILE * pFile = NULL;
	
	string strTmpFileName = "Temp_find-ls.txt.gz";
	string strFileNameAtLocalMachine= "find-ls.txt.gz";
	string strFileNameAtServer= "/find-ls.txt.gz";

	CString strStaus;

	double nTripsToServerCnt;
	int nTotalSpan = 100;
	DWORD dwBytesWrrittenToFile =0;

	nTripsToServerCnt = ceil(dFileSize / ((NKBYTE * 1024)*1.0));
	if(nTripsToServerCnt ==0) 
		nTripsToServerCnt  =1;	
	

	while(nTripsToServerCnt > nTotalSpan)
	{
		nTotalSpan *= 10;
	}
	
	DeleteFile(strFileNameAtLocalMachine.c_str());


	m_ProgressCtl.SetRange(0,nTotalSpan);
	//m_ProgressCtl.SetPos(1);

	strStaus.Format("Initializing WinInet..");
	GetDlgItem(IDC_STATIC_STATUSNEW)->SetWindowText(strStaus);

	
	hOpen = InternetOpen("PauseResumeDemo",INTERNET_OPEN_TYPE_PRECONFIG ,NULL, NULL, NULL);
	if(!hOpen)
	{
         return false;
	}

	DWORD dwContext = 123;

	strStaus.Format("Connecting to server..");
	GetDlgItem(IDC_STATIC_STATUSNEW)->SetWindowText(strStaus);

	if ( !(hConnect = InternetConnect ( hOpen, "ftp.gnu.org" , nServerPort, NULL, NULL, INTERNET_SERVICE_FTP, 0  , 0) ) )
	{
		CString strMsg;
		string strReposeFromServer;
		GetLastResponse(strReposeFromServer);
		strMsg.Format("Error:%d\nDescription:%s", GetLastError(), strReposeFromServer.c_str());
		AfxMessageBox(strMsg, MB_OK);
		return false;
	}
	
	strStaus.Format("Finding out if this server supoprts Pause & Resume..");
	GetDlgItem(IDC_STATIC_STATUSNEW)->SetWindowText(strStaus);

	if(FindPauseResumeSupported(hConnect))
	{
		strStaus.Format("Pause & Resume Supported..");
		GetDlgItem(IDC_STATIC_STATUSNEW)->SetWindowText(strStaus);
	}
	else
	{
		strStaus.Format("Pause & Resume not Supported..");
		GetDlgItem(IDC_STATIC_STATUSNEW)->SetWindowText(strStaus);
	}


	double dCurrentFileSize;

	if(IfLocalFileExist(strTmpFileName.c_str(), &dCurrentFileSize)) 
	{
		dOffsetToSeek				= dCurrentFileSize;
	}
	
	if(dOffsetToSeek >0)
	{
	
		strStaus.Format("Sending REST...");
		GetDlgItem(IDC_STATIC_STATUSNEW)->SetWindowText(strStaus);
		
		//Seek to the file first
		char pszOffset[30];
		HINTERNET hResponse;
		
		ltoa(dOffsetToSeek, pszOffset, 10);
		//sprintf(pszOffset,"%ld", dOffsetToSeek);
		string strSeekOffsetCMD = "REST " + (string)pszOffset;

		bRet = ::FtpCommand(hConnect, // WinInet Connection handle
						   FALSE,        // No, I don't expect a response
						   FTP_TRANSFER_TYPE_BINARY, // I'm receiving ASCII
						   strSeekOffsetCMD.c_str(),      // This is the FTP command I am passing
						   0,           // No context needed
						   &hResponse); // The handle to read the response 
		if (!bRet)
		{

			CString strMsg;
			strMsg.Format("Error:%d", GetLastError());
			AfxMessageBox(strMsg, MB_OK);

		}

	}


	hOpenFile = ::FtpOpenFile(hConnect,strFileNameAtServer.c_str(), GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, 1);
	if(hOpenFile == NULL)
	{
		CString strMsg;
		string strReposeFromServer;
		GetLastResponse(strReposeFromServer);
		strMsg.Format("Error:%d\nDescription:%s", GetLastError(), strReposeFromServer.c_str());
		AfxMessageBox(strMsg, MB_OK); 
        return false;
	}


   if  ( !(pFile = fopen (strTmpFileName.c_str(), "ab" ) ) )
   {
		CString strMsg;
		strMsg.Format("Error:%d", GetLastError());
		AfxMessageBox(strMsg, MB_OK); 
        return false;
   }

   if(dOffsetToSeek >=0)
   {
		strStaus.Format("Seeking to local file...");
		GetDlgItem(IDC_STATIC_STATUSNEW)->SetWindowText(strStaus);

	   int n = fseek(pFile, dOffsetToSeek, SEEK_SET);
   }


	dwBytesWrrittenToFile = dOffsetToSeek;
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);

   do
       {
		   PUMPMESSAGE();

		   //Intentional delay so that user get chance to hit stop button
		   Sleep(100);
		   
		   strStaus.Format("Sending RETRIVE..");
		   GetDlgItem(IDC_STATIC_STATUSNEW)->SetWindowText(strStaus);
			
			if(m_bStopped)
			{
				strStaus.Format("Stopped..");
				GetDlgItem(IDC_STATIC_STATUSNEW)->SetWindowText(strStaus);
				
				AfxMessageBox("Download aborted\nThere is an incomplete temp file 'Temp_find-ls.txt.gz' at current directory. Don't delete it just check it out. It has useful data downloaded so far...\nOnce you are done click Start again, now download will start from exact location where it left off");

				m_bStopped = FALSE;
				bInComplete = TRUE;
				GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
				break;
			}
			
			if (!InternetReadFile (hOpenFile, (LPVOID)pBuffer, dwToRead,  &dwSize) )
			{
				fclose (pFile);
				CString strMsg;
				string strReposeFromServer;
				GetLastResponse(strReposeFromServer);
				strMsg.Format("Error:%d\nDescription:%s", GetLastError(), strReposeFromServer.c_str());
				AfxMessageBox(strMsg, MB_OK); 
				return false;
			}
          if (!dwSize)
		  {
			m_ProgressCtl.SetPos(100);
			CString	strTmpPercentage;
			strTmpPercentage.Format("Percentage Completed:%d%%", 100);
			GetDlgItem(IDC_STATIC_PERCENTAGE)->SetWindowText(strTmpPercentage);
			break;  // Condition of dwSize=0 indicate EOF. Stop.
		  }
          else
		  {
			fwrite(pBuffer, sizeof (char), dwSize , pFile);

			dwBytesWrrittenToFile = dwBytesWrrittenToFile + dwSize;

			int nPercentageComplete = (int)floor(((dwBytesWrrittenToFile) * 100)/dFileSize);

			CString	strTmpPercentage;
			strTmpPercentage.Format("Percentage Completed:%d%%", nPercentageComplete);
			GetDlgItem(IDC_STATIC_PERCENTAGE)->SetWindowText(strTmpPercentage);

			//m_pMainFrame->m_ProgressCtl.SetBkColor(RGB(0, 255, 0));
			int nLower, nUpper;
			m_ProgressCtl.GetRange(nLower, nUpper );

			m_ProgressCtl.SetPos(((nUpper-nLower)/100) * nPercentageComplete);
		  }
       }   // do
      while (TRUE);

	fflush (pFile);
	fclose (pFile);
	delete [] pBuffer;


	if(!bInComplete)
	{
		strStaus.Format("Renaming file at local machine..");
		GetDlgItem(IDC_STATIC_STATUSNEW)->SetWindowText(strStaus);

		if(!MoveFile(strTmpFileName.c_str(), strFileNameAtLocalMachine.c_str() ))
		{
			CString strMsg;
			strMsg.Format("Error:%d", GetLastError());
			AfxMessageBox(strMsg, MB_OK); 

		}
		strStaus.Format("Done..");
		GetDlgItem(IDC_STATIC_STATUSNEW)->SetWindowText(strStaus);
	}

	if(!bInComplete)
	{
		AfxMessageBox("File has downloaded..");
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	}

	if(hConnect)
	  InternetCloseHandle(hConnect);
	if(hOpen)
		InternetCloseHandle(hOpen);
	if(hOpenFile)
		InternetCloseHandle(hOpenFile);

	  return TRUE;
}

void CPauseResumeDemoDlg::OnButton2() 
{
	m_bStopped = TRUE;	
}


bool  CPauseResumeDemoDlg::IfLocalFileExist(const char *pszFilePath, double *pdwFileSize)
{
	struct _finddata_t c_file;
	long hFile;

	if( (hFile = _findfirst( pszFilePath, &c_file )) == -1L )
	{
	   *pdwFileSize = 0;
		return false;
	}
	else
	{
		CString strStaus;
		strStaus.Format("Reading size of local file");
		GetDlgItem(IDC_STATIC_STATUSNEW)->SetWindowText(strStaus);

	   *pdwFileSize = c_file.size;
	   _findclose(hFile);
	   return true;
	}

	return false;
}



bool CPauseResumeDemoDlg::FindPauseResumeSupported(HINTERNET hConnect)
{
	if(!hConnect)
	{
		return FALSE;
	}


	HINTERNET hResponse;
	BOOL bRet = ::FtpCommand(hConnect,FALSE,FTP_TRANSFER_TYPE_ASCII, 
							"REST 0",
							0,           // No context needed
							&hResponse); // The handle to read the response 

    if (!bRet)
    {
		return FALSE;
    }

	DWORD dwError;
	TCHAR tpszBuff[2*1024];
	DWORD dwBuffLen = 2*1024;
	bRet = ::InternetGetLastResponseInfo(&dwError, tpszBuff, &dwBuffLen); 
	
	if(strstr(tpszBuff, "350")) //a successfull 
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}