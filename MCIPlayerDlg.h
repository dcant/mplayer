
// MCIPlayerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxbutton.h"


// CMCIPlayerDlg dialog
class CMCIPlayerDlg : public CDialogEx
{
// Construction
public:
	CMCIPlayerDlg(CWnd* pParent = NULL);	// standard constructor
	~CMCIPlayerDlg();

// Dialog Data
	enum { IDD = IDD_MCIPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	BOOL m_PSign;
	BOOL m_ASign;
	DWORD dwError;
	MCIDEVICEID m_MCIDeviceID;
	CString szErrorBuf[MAXERRORLENGTH];
	UINT m_Timer;
	UINT m_Length;
	MCI_STATUS_PARMS mciStatusParms;
	UINT dCurrentPosition;
	CMenu m_PopMenu;
	CMenu *m_pPop;
	HICON m_Icon;
	NOTIFYICONDATA nid;
	BOOL m_Hide;
	CString m_Name;
	MCI_OPEN_PARMS mciOpenParms;
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	CString m_Path[MAX_PATH];
	CString m_Filename[MAX_PATH];
	BOOL m_Open;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT MciNotify(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedOpenButton();
	afx_msg void OnClickedStartButton();
	afx_msg void OnClickedPauseButton();
	afx_msg void OnClickedStopButton();
	afx_msg void OnClickedCloseButton();
	afx_msg void OnClickedExitButton();
	CSliderCtrl m_Slider;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPopOpen();
	afx_msg void OnPopClose();
	afx_msg void OnPopExit();
	CMFCButton m_Start;
	afx_msg void OnClickedBackButton();
	afx_msg void OnClickedForwordButton();
	afx_msg void ToTray();
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam);
	void FindFirst();
	void FindNext();
};
