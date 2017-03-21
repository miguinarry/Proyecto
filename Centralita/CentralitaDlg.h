
// CentralitaDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "Led.h"
#include "Gauge.h"
#include "MySocket.h"
#include "MyMemory.h"

/*
	Define constant values
*/
// Timers
#define ID_TIMER_POLLING 1
#define ID_TIMER_INTERMITENTES 2
#define ID_TIMER_RECONNECT 3

#define NUMBER_RECONNECTIOS 3 + 1
#define RECONNECT_TIME 5000
#define RECONNECT_TIME_2PLANO 30000



// CCentralitaDlg dialog
class CCentralitaDlg : public CDialogEx
{
// Construction
public:
	CCentralitaDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CENTRALITA_DIALOG };

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
	afx_msg void OnFileExit();
	CString m_luces_ip;
	CListBox m_list;
	CString m_accionamientos_ip;
	int m_accionamientos_port;
	int m_luces_port;
	CString m_motor_ip;
	int m_motor_port;
	int m_polling_time;
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedPollingStartstop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public: 
	bool LightsOn = false;
	// Simulated memory
	CMyMemory *Memory;
	// Variables for user controls
	CLed m_motor_led,
		m_accionamientos_led,
		m_accionamientos_izq,
		m_accionamientos_der,
		m_accionamientos_freno,
		m_luces_led;
	CGauge m_motor_rpm, m_motor_temperatura;
	// Function to print a msg in the list
	void ShowLog(CString msg, bool NewLine = false);
	void UpdateValuesFromMemory();
	// Initialize threads
	CWinThread *THAccionamientos;
	CWinThread *THMotor;
	CWinThread *THLuces;
	// Sockets
	//CMySocket *FirstSocket;

	CMySocket *SocketAccionamientos;
	ModBus *ModBusAccionamientos;

	CMySocket *SocketMotor;
	ModBus *ModBusMotor;

	CMySocket *SocketLuces;
	ModBus *ModBusLuces;

	CButton m_polling;
};
