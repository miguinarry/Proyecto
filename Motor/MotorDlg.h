
// MotorDlg.h : header file
//

#pragma once
#include "MySocket.h"
#include "MyMemory.h"
#include "afxwin.h"
#include "afxcmn.h"

// CMotorDlg dialog
class CMotorDlg : public CDialogEx
{
// Construction
public:
	CMotorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MOTOR_DIALOG };

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
	CMySocket *FirstSocket;
	CMySocket *AcceptedSocket;
	int m_port;
	int m_slider_rpm;
	int m_slider_temperatura;
	CString m_valor_rpm;
	CString m_valor_temperatura;
	afx_msg void OnReleasedcaptureSliderRpm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSliderTemperatura(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedStartstop();
	CButton m_startstop;
	CSliderCtrl m_s_rpm;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_s_temperatura;
};
