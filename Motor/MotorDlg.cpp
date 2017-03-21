
// MotorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Motor.h"
#include "MotorDlg.h"
#include "afxdialogex.h"

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
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMotorDlg dialog



CMotorDlg::CMotorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMotorDlg::IDD, pParent)
	, m_port(0)
	, m_slider_rpm(0)
	, m_slider_temperatura(0)
	, m_valor_rpm(_T(""))
	, m_valor_temperatura(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMotorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Slider(pDX, IDC_SLIDER_RPM, m_slider_rpm);
	DDX_Slider(pDX, IDC_SLIDER_TEMPERATURA, m_slider_temperatura);
	DDX_Text(pDX, IDC_VALOR_RPM, m_valor_rpm);
	DDX_Text(pDX, IDC_VALOR_TEMPERATURA, m_valor_temperatura);
	DDX_Control(pDX, IDC_STARTSTOP, m_startstop);
	DDX_Control(pDX, IDC_SLIDER_RPM, m_s_rpm);
	DDX_Control(pDX, IDC_SLIDER_TEMPERATURA, m_s_temperatura);
}

BEGIN_MESSAGE_MAP(CMotorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RPM, &CMotorDlg::OnReleasedcaptureSliderRpm)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_TEMPERATURA, &CMotorDlg::OnReleasedcaptureSliderTemperatura)
	ON_BN_CLICKED(IDC_STARTSTOP, &CMotorDlg::OnBnClickedStartstop)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CMotorDlg message handlers

BOOL CMotorDlg::OnInitDialog()
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
	// set port
	m_port = 502;
	UpdateData(false);
	// Initialize SImulated Memory
	Memory = new CMyMemory();
	Memory->Reset();
	// Initialize ModBus Protocol
	MyModBus = new ModBus(Memory);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMotorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMotorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMotorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMotorDlg::OnReleasedcaptureSliderRpm(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_valor_rpm.Format("Valor Actual: %d", 7000 * m_slider_rpm / 100);
	UpdateData(false);

	// Meter el valor en la direccìón de memoria 401
	Memory->Registers[1].Value = 7000 * m_slider_rpm / 100;
	*pResult = 0;
}


void CMotorDlg::OnReleasedcaptureSliderTemperatura(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_valor_temperatura.Format("Valor Actual: %d", 300 * m_slider_temperatura / 100);
	UpdateData(false);

	// Meter el valor en la direccìón de memoria 400
	Memory->Registers[0].Value = 300 * m_slider_temperatura / 100;
	*pResult = 0;
}


void CMotorDlg::OnBnClickedStartstop()
{
	UpdateData(true);

	switch (m_startstop.GetCheck())
	{
	case BST_CHECKED:
		m_startstop.SetWindowText(_T("Stop"));

		// Create Socket
		FirstSocket = new CMySocket(this);
		AcceptedSocket = new CMySocket(this);
		FirstSocket->Create(m_port, SOCK_STREAM);
		FirstSocket->Listen();
		break;

	case BST_UNCHECKED:
		m_startstop.SetWindowText(_T("Start"));

		// Close Sockets
		AcceptedSocket->Close();
		delete[] AcceptedSocket;
		FirstSocket->Close();
		delete[] FirstSocket;
		break;
	}
}


void CMotorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(true);

	m_slider_rpm = m_s_rpm.GetPos();
	m_valor_rpm.Format("Valor Actual: %d", 7000 * m_slider_rpm / 100);
	// Meter el valor en la direccìón de memoria 401
	Memory->Registers[1].Value = 7000 * m_slider_rpm / 100;

	m_slider_temperatura = m_s_temperatura.GetPos();
	m_valor_temperatura.Format("Valor Actual: %d", 300 * m_slider_temperatura / 100);
	// Meter el valor en la direccìón de memoria 400
	Memory->Registers[0].Value = 300 * m_slider_temperatura / 100;
	
	UpdateData(false);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
