
// AccionamientosDlg.h : header file
//

#pragma once
#include "MySocket.h"
#include "afxwin.h"
#include "MyMemory.h"

//FSM
#define STATE_STOP 0
#define STATE_START 1


// CAccionamientosDlg dialog
class CAccionamientosDlg : public CDialogEx
{
// Construction
public:
	CAccionamientosDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ACCIONAMIENTOS_DIALOG };

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
	// Simulated memory
	CMyMemory *Memory;
	// Modbus Object
	ModBus *MyModBus;
	int m_port;
	CMySocket *FirstSocket;
	CMySocket *AcceptedSocket;
	afx_msg void OnBnClickedStartstop();
	CButton m_der;
	CButton m_freno;
	CButton m_izq;
	CButton m_startstop;
	afx_msg void OnBnClickedFreno();
	afx_msg void OnBnClickedIzq();
	afx_msg void OnBnClickedDer();
};
