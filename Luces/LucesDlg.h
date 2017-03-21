
// LucesDlg.h : header file
//

#pragma once
#include "Led.h"
#include "afxwin.h"
#include "MySocket.h"
#include "MyMemory.h"

#define ID_TIMER_INTERMITENTES 1



// CLucesDlg dialog
class CLucesDlg : public CDialogEx
{
// Construction
public:
	CLucesDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LUCES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool LightsOn = false;
	// Simulated memory
	CMyMemory *Memory;
	// Modbus Object
	ModBus *MyModBus;
	CMySocket *FirstSocket;
	CMySocket *AcceptedSocket;
	CLed m_freno,
		m_intermitente_di,
		m_intermitente_dd,
		m_intermitente_ti,
		m_intermitente_td;
	int m_port;
	void UpdateValues();
	afx_msg void OnBnClickedCheck1();
	CButton m_startstop;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
