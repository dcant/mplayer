
// MCIPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MCIPlayer.h"
#include "MCIPlayerDlg.h"
#include "afxdialogex.h"
#include <strsafe.h>
#pragma comment(lib,"winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CMCIPlayerDlg dialog




CMCIPlayerDlg::CMCIPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMCIPlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_PopMenu.LoadMenuW(IDR_POP_MENU);
}

CMCIPlayerDlg::~CMCIPlayerDlg()
{
	m_PopMenu.DestroyMenu();
}

void CMCIPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER, m_Slider);
	DDX_Control(pDX, IDC_START_BUTTON, m_Start);
}

BEGIN_MESSAGE_MAP(CMCIPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
ON_MESSAGE(MM_MCINOTIFY,&CMCIPlayerDlg::MciNotify)
ON_BN_CLICKED(IDC_OPEN_BUTTON, &CMCIPlayerDlg::OnClickedOpenButton)
ON_BN_CLICKED(IDC_START_BUTTON, &CMCIPlayerDlg::OnClickedStartButton)
ON_BN_CLICKED(IDC_PAUSE_BUTTON, &CMCIPlayerDlg::OnClickedPauseButton)
ON_BN_CLICKED(IDC_STOP_BUTTON, &CMCIPlayerDlg::OnClickedStopButton)
ON_BN_CLICKED(IDC_CLOSE_BUTTON, &CMCIPlayerDlg::OnClickedCloseButton)
ON_BN_CLICKED(IDC_EXIT_BUTTON, &CMCIPlayerDlg::OnClickedExitButton)
ON_WM_TIMER()
ON_WM_HSCROLL()
ON_WM_RBUTTONDOWN()
ON_COMMAND(ID_POP_OPEN, &CMCIPlayerDlg::OnPopOpen)
ON_COMMAND(ID_POP_CLOSE, &CMCIPlayerDlg::OnPopClose)
ON_COMMAND(ID_POP_EXIT, &CMCIPlayerDlg::OnPopExit)
ON_BN_CLICKED(IDC_BACK_BUTTON, &CMCIPlayerDlg::OnClickedBackButton)
ON_BN_CLICKED(IDC_FORWORD_BUTTON, &CMCIPlayerDlg::OnClickedForwordButton)
ON_MESSAGE(WM_SHOWTASK,&CMCIPlayerDlg::OnShowTask)
//ON_WM_SIZE()
END_MESSAGE_MAP()


// CMCIPlayerDlg message handlers

BOOL CMCIPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	m_PSign = FALSE;
	m_ASign = FALSE;
	m_MCIDeviceID = 0;
	m_Timer = 0;
	dCurrentPosition = 0;
	m_Length = 0;
	mciStatusParms.dwItem = MCI_STATUS_POSITION;
	m_Slider.SetPos(0);
	m_Hide = FALSE;
	m_Open = FALSE;


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMCIPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	if(nID == SC_MINIMIZE)
	{
		ToTray();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMCIPlayerDlg::OnPaint()
{
	CPaintDC dc(this);
	if (IsIconic())
	{
		//CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
		// Draw the icon
	}
	else
	{
		/*CRect rect;
		GetClientRect(&rect);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmapW(IDB_BITMAP);
		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap *pbmpOld = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
		//*/CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMCIPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}








void CMCIPlayerDlg::OnClickedOpenButton()
{
	// TODO: Add your control notification handler code here
	/*CString filename;
	MCI_OPEN_PARMS mciOpenParms;
	DWORD dwError;
	static TCHAR BASED_CODE szFilter[] = _T("mp3&wma(*.mp3;*.wma)|*.mp3;*.wma|");
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,NULL,0,TRUE);

	if(IDOK == dlg.DoModal())
	{
		filename = dlg.GetPathName();
		m_Name = dlg.GetFileName();
		if(m_PSign)
		{
			dwError = mciSendCommand(m_MCIDeviceID,MCI_CLOSE,0,NULL);
			if(dwError)
			{
				if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
					MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
				else
					MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
				return;
			}
		}
		mciOpenParms.lpstrDeviceType = _T("mpegvideo");
		mciOpenParms.lpstrElementName = filename;
		dwError = mciSendCommand(0,MCI_OPEN,MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,(DWORD)(LPVOID)&mciOpenParms);
		if(dwError)
		{
			if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
				MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
			else
				MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
			return;
		}*/

	    char buffer[MAX_PATH];
		char path[MAX_PATH];
		strcat(path,"\\");
		ZeroMemory(buffer,MAX_PATH);
		ZeroMemory(path,MAX_PATH);
		BROWSEINFO bi;
		LPITEMIDLIST lpItemIDList = NULL;
		bi.hwndOwner=this->m_hWnd; 
		bi.pidlRoot=NULL; 
		bi.pszDisplayName=(LPWSTR)buffer;
		bi.lpszTitle=_T("Please choose"); 
		bi.ulFlags=BIF_STATUSTEXT | BIF_USENEWUI | BIF_RETURNONLYFSDIRS | BIF_NONEWFOLDERBUTTON;
		bi.lpfn=NULL;
		bi.lParam=0;
		bi.iImage=0;
		if((lpItemIDList = SHBrowseForFolder(&bi)) != NULL);
		{
			SHGetPathFromIDList(lpItemIDList,(LPWSTR)path);
		}

		FindFirst();

		if(m_Hide)
		{
			ShowWindow(SW_RESTORE);
			Shell_NotifyIcon(NIM_DELETE,&nid);
		}
		/*GetDlgItem(IDC_STATIC)->SetWindowTextW(dlg.GetFileName());
		m_MCIDeviceID = mciOpenParms.wDeviceID;

		MCI_STATUS_PARMS mciStatus;
		mciStatus.dwItem = MCI_STATUS_LENGTH;
		dwError = mciSendCommand(m_MCIDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)(LPVOID)&mciStatus);
		if(dwError)
		{
			if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
				MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
			else
				MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
			return;
		}
		m_Length = mciStatus.dwReturn;
		m_Slider.SetRange(0,m_Length);

		m_PSign = FALSE;
		m_ASign = FALSE;


		m_pPop->EnableMenuItem(ID_POP_OPEN,MF_GRAYED);
		m_pPop->EnableMenuItem(ID_POP_CLOSE,MF_ENABLED);
	}*/
}

void CMCIPlayerDlg::OnClickedStartButton()
{
	// TODO: Add your control notification handler code here
	if(m_MCIDeviceID)
	{
		MCI_PLAY_PARMS mciPlayParms;
		if(!m_PSign)
		{
			mciPlayParms.dwCallback = (long)GetSafeHwnd();
			mciPlayParms.dwFrom = 0;
			m_Timer = SetTimer(1,500,0);
			dwError = mciSendCommand(m_MCIDeviceID,MCI_PLAY,MCI_FROM|MCI_NOTIFY,(DWORD)(LPVOID)&mciPlayParms);
			if(dwError)
			{
				if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
					MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
				else
					MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
				return;
			}
			m_PSign = TRUE;
			m_ASign = FALSE;
			m_Icon = AfxGetApp()->LoadIcon(IDI_PAUSE);
			m_Start.SetIcon(m_Icon);
			m_Start.SetTooltip(_T("pause"));
		}
		else
			OnClickedPauseButton();
	}
}


void CMCIPlayerDlg::OnClickedPauseButton()
{
	// TODO: Add your control notification handler code here
	if(m_PSign)
	{
		if(!m_ASign)
		{
			dwError = mciSendCommand(m_MCIDeviceID,MCI_PAUSE,0,NULL);
			KillTimer(m_Timer);
			if(dwError)
			{
				if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
					MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
				else
					MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
				return;
			}
			m_ASign = TRUE;
			m_Icon = AfxGetApp()->LoadIcon(IDI_PLAY);
			m_Start.SetIcon(m_Icon);
			m_Start.SetTooltip(_T("Play"));
		}
		else
		{
			m_Timer = SetTimer(1,500,0);
			dwError = mciSendCommand(m_MCIDeviceID,MCI_RESUME,0,NULL);
			if(dwError)
			{
				if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
					MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
				else
					MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
				return;
			}
			m_ASign = FALSE;
			m_Icon = AfxGetApp()->LoadIcon(IDI_PAUSE);
			m_Start.SetIcon(m_Icon);
			m_Start.SetTooltip(_T("pause"));
		}
	}
}


void CMCIPlayerDlg::OnClickedStopButton()
{
	// TODO: Add your control notification handler code here
	if(m_MCIDeviceID)
	{
		dwError = mciSendCommand(m_MCIDeviceID,MCI_STOP,MCI_WAIT,NULL);
		KillTimer(m_Timer);
		m_Slider.SetPos(0);
		m_Icon = AfxGetApp()->LoadIcon(IDI_PLAY);
		m_Start.SetIcon(m_Icon);
		m_Start.SetTooltip(_T("play"));
		if(dwError)
		{
			if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
				MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
			else
				MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
			return;
		}
		m_PSign = FALSE;
		m_ASign = FALSE;
	}
}


void CMCIPlayerDlg::OnClickedCloseButton()
{
	// TODO: Add your control notification handler code here
	dwError = mciSendCommand(m_MCIDeviceID,MCI_STOP,MCI_WAIT,NULL);
	if(m_Timer)
		KillTimer(m_Timer);
	if(m_PSign)
	{
		m_Icon = AfxGetApp()->LoadIcon(IDI_PLAY);
		m_Start.SetIcon(m_Icon);
		m_Start.SetTooltip(_T("play"));	
	}
	if(dwError)
	{
		if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
			MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
		else
			MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
		return;
	}
	dwError = mciSendCommand(m_MCIDeviceID,MCI_CLOSE,0,NULL);
	if(dwError)
	{
		if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
			MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
		else
			MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
		return;
	}
	if(m_Hide)
	{
		StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), L"MCIPlayer");
		Shell_NotifyIcon(NIM_MODIFY,&nid);
	}
	GetDlgItem(IDC_STATIC)->SetWindowTextW(_T("Music Player"));
	m_Slider.SetPos(0);
	m_PSign = FALSE;
	m_ASign = FALSE;
	m_MCIDeviceID = 0;
	m_pPop->EnableMenuItem(ID_POP_OPEN,MF_ENABLED);
	m_pPop->EnableMenuItem(ID_POP_CLOSE,MF_GRAYED);
}


void CMCIPlayerDlg::OnClickedExitButton()
{
	// TODO: Add your control notification handler code here
	if(m_MCIDeviceID)
		OnClickedCloseButton();
	if(m_Hide)
		Shell_NotifyIcon(NIM_DELETE,&nid);
	CDialog::OnOK();
}

LRESULT CMCIPlayerDlg::MciNotify(WPARAM wParam,LPARAM lParam)
{
	if(wParam == MCI_NOTIFY_SUCCESSFUL)
	{
		m_PSign = FALSE;
		m_ASign = FALSE;
		m_Slider.SetPos(0);
		//OnClickedStartButton();
		FindNext();
		return 0;
	}
	return -1;
}




void CMCIPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	mciSendCommand(m_MCIDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)(LPVOID)&mciStatusParms);
/*	if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
		MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
	else
		MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
	return;*/
	dCurrentPosition = mciStatusParms.dwReturn;
	m_Slider.SetPos(dCurrentPosition);

	CDialogEx::OnTimer(nIDEvent);
}



void CMCIPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if(m_PSign)
	{
		if(m_ASign)
		{
			m_Slider.SetPos(dCurrentPosition);
			return;
		}
		if(pScrollBar->GetDlgCtrlID() == IDC_SLIDER)
		{
			if(m_PSign)
			{
				dCurrentPosition = m_Slider.GetPos();
				MCI_PLAY_PARMS mciPlayParms;
				mciPlayParms.dwCallback = (long)GetSafeHwnd();
				mciPlayParms.dwFrom = dCurrentPosition;
				dwError = mciSendCommand(m_MCIDeviceID,MCI_PLAY,MCI_FROM|MCI_NOTIFY,(DWORD)(LPVOID)&mciPlayParms);

				if(dwError)
				{
					if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
						MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
					else
						MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
					return;
				}

			}
		}
	}
	else
		m_Slider.SetPos(0);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}




void CMCIPlayerDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pPop = m_PopMenu.GetSubMenu(0);
	if(m_MCIDeviceID)
	{
		m_pPop->EnableMenuItem(ID_POP_OPEN,MF_GRAYED);
		m_pPop->EnableMenuItem(ID_POP_CLOSE,MF_ENABLED);
	}
	else
	{
		m_pPop->EnableMenuItem(ID_POP_OPEN,MF_ENABLED);
		m_pPop->EnableMenuItem(ID_POP_CLOSE,MF_GRAYED);
	}
	ClientToScreen(&point);
	m_pPop->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CMCIPlayerDlg::OnPopOpen()
{
	// TODO: Add your command handler code here
	OnClickedOpenButton();
}


void CMCIPlayerDlg::OnPopClose()
{
	// TODO: Add your command handler code here
	OnClickedCloseButton();
}


void CMCIPlayerDlg::OnPopExit()
{
	// TODO: Add your command handler code here
	OnClickedExitButton();
}


void CMCIPlayerDlg::OnClickedBackButton()
{
	// TODO: Add your control notification handler code here

	if(m_PSign)
	{
		dCurrentPosition = dCurrentPosition - 3000;
		if(dCurrentPosition < 0)
			dCurrentPosition = 0;
		m_Slider.SetPos(dCurrentPosition);
		MCI_PLAY_PARMS mciPlayParms;
		mciPlayParms.dwCallback = (long)GetSafeHwnd();
		mciPlayParms.dwFrom = dCurrentPosition;
		dwError = mciSendCommand(m_MCIDeviceID,MCI_PLAY,MCI_FROM|MCI_NOTIFY,(DWORD)(LPVOID)&mciPlayParms);

		if(dwError)
		{
			if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
				MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
			else
				MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
			return;
		}
	}
}




void CMCIPlayerDlg::OnClickedForwordButton()
{
	// TODO: Add your control notification handler code here
	if(m_PSign)
	{
		dCurrentPosition = dCurrentPosition + 3000;
		if(dCurrentPosition > m_Length)
			dCurrentPosition = m_Length;
		m_Slider.SetPos(dCurrentPosition);
		MCI_PLAY_PARMS mciPlayParms;
		mciPlayParms.dwCallback = (long)GetSafeHwnd();
		mciPlayParms.dwFrom = dCurrentPosition;
		dwError = mciSendCommand(m_MCIDeviceID,MCI_PLAY,MCI_FROM|MCI_NOTIFY,(DWORD)(LPVOID)&mciPlayParms);

		if(dwError)
		{
			if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
				MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
			else
				MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
			return;
		}
	}
}


void CMCIPlayerDlg::ToTray()
{
	m_Hide = TRUE;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;
	if(m_ASign)
		nid.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_PAUSE));
	else
		nid.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	if(m_MCIDeviceID)
	{
		StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), (LPCWSTR)m_Name);
	}
	else
		StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), L"MCIPlayer");
	Shell_NotifyIcon(NIM_ADD,&nid);
	ShowWindow(SW_HIDE);
}


LRESULT CMCIPlayerDlg::OnShowTask(WPARAM wParam,LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME)
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP:
		{
			LPPOINT point = new tagPOINT;
			::GetCursorPos(point);
			m_pPop = m_PopMenu.GetSubMenu(0);
			if(m_MCIDeviceID)
			{
				m_pPop->EnableMenuItem(ID_POP_OPEN,MF_GRAYED);
				m_pPop->EnableMenuItem(ID_POP_CLOSE,MF_ENABLED);
			}
			else
			{
				m_pPop->EnableMenuItem(ID_POP_OPEN,MF_ENABLED);
				m_pPop->EnableMenuItem(ID_POP_CLOSE,MF_GRAYED);
			}
			SetForegroundWindow();
			m_pPop->TrackPopupMenu(TPM_LEFTALIGN,point->x,point->y,this);
			delete point;
		}break;
	case WM_LBUTTONDBLCLK:
		{
			//HWND hWnd = ::GetWindow(::GetForegroundWindow(),GW_HWNDFIRST);
			m_Hide = FALSE;
			ShowWindow(SW_RESTORE);
			Shell_NotifyIcon(NIM_DELETE,&nid);
		}break;
	case WM_LBUTTONUP:
		{
			if(m_MCIDeviceID)
			{
				OnClickedStartButton();
				if(!m_ASign)
				{
					nid.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_PLAY));
					Shell_NotifyIcon(NIM_MODIFY,&nid);
				}
				else
				{
					nid.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_PAUSE));
					Shell_NotifyIcon(NIM_MODIFY,&nid);
				}
			}
		}break;
	}
	return 0;
}


void CMCIPlayerDlg::FindFirst()
{
	StrCat((LPWSTR)m_Path,_T("\\"));
	StrCat((LPWSTR)m_Path,_T("*.*"));
	MessageBox((LPCTSTR)m_Path,_T("MCI Error"),MB_ICONWARNING);
	hFind = ::FindFirstFile((LPCWSTR)m_Path,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		MessageBox(_T("No mp3|wma file"),_T("MCI Error"),MB_ICONWARNING);
		return;
	}
	if(StrStr(FindFileData.cFileName,_T(".mp3"))||StrStr(FindFileData.cFileName,_T(".wma")))
	{
		mciOpenParms.lpstrDeviceType = _T("mpegvideo");
		mciOpenParms.lpstrElementName = StrCat(StrCpy((LP)m_Filename,_T("\\")),FindFileData.cFileName);
		dwError = mciSendCommand(0,MCI_OPEN,MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,(DWORD)(LPVOID)&mciOpenParms);
		if(dwError)
		{
			FindNext();
			return;
		}
		m_Open = TRUE;
		m_MCIDeviceID = mciOpenParms.wDeviceID;

		MCI_STATUS_PARMS mciStatus;
		mciStatus.dwItem = MCI_STATUS_LENGTH;
		dwError = mciSendCommand(m_MCIDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)(LPVOID)&mciStatus);
		if(dwError)
		{
			if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
				MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
			else
				MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
			return;
		}
		m_Length = mciStatus.dwReturn;
		m_Slider.SetRange(0,m_Length);

		m_PSign = FALSE;
		m_ASign = FALSE;


		m_pPop->EnableMenuItem(ID_POP_OPEN,MF_GRAYED);
		m_pPop->EnableMenuItem(ID_POP_CLOSE,MF_ENABLED);
	}
	else
		FindNext();
}


void CMCIPlayerDlg::FindNext()
{
	if(!FindNextFile(hFind,&FindFileData))
	{
		if(m_Open)
			FindFirst();
		else
			MessageBox(_T("No mp3|wma file"),_T("MCI Error"),MB_ICONWARNING);
	}
	if(StrStr(FindFileData.cFileName,_T(".mp3"))||StrStr(FindFileData.cFileName,_T(".wma")))
	{
		mciOpenParms.lpstrDeviceType = _T("mpegvideo");
		mciOpenParms.lpstrElementName = StrCat(StrCpy((LP)m_Filename,_T("\\")),FindFileData.cFileName);
		dwError = mciSendCommand(0,MCI_OPEN,MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,(DWORD)(LPVOID)&mciOpenParms);
		if(dwError)
		{
			FindNext();
			return;
		}
		m_Open = TRUE;
		m_MCIDeviceID = mciOpenParms.wDeviceID;

		MCI_STATUS_PARMS mciStatus;
		mciStatus.dwItem = MCI_STATUS_LENGTH;
		dwError = mciSendCommand(m_MCIDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)(LPVOID)&mciStatus);
		if(dwError)
		{
			if(mciGetErrorString(dwError,(LPWSTR)szErrorBuf,MAXERRORLENGTH))
				MessageBox((LPCTSTR)szErrorBuf,_T("MCI Error"),MB_ICONWARNING);
			else
				MessageBox(_T("Unknown Error"),_T("MCI Error"),MB_ICONWARNING);
			return;
		}
		m_Length = mciStatus.dwReturn;
		m_Slider.SetRange(0,m_Length);

		m_PSign = FALSE;
		m_ASign = FALSE;


		m_pPop->EnableMenuItem(ID_POP_OPEN,MF_GRAYED);
		m_pPop->EnableMenuItem(ID_POP_CLOSE,MF_ENABLED);
	}
}