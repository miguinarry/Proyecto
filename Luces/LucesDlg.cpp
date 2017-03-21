
// LucesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Luces.h"
#include "LucesDlg.h"
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


// CLucesDlg dialog



CLucesDlg::CLucesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLucesDlg::IDD, pParent)
	, m_port(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLucesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Control(pDX, IDC_STARTSTOP, m_startstop);
}

BEGIN_MESSAGE_MAP(CLucesDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, &CLucesDlg::OnBnClickedCheck1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLucesDlg message handlers

BOOL CLucesDlg::OnInitDialog()
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
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	// Set leds
	m_freno.SubclassDlgItem(IDC_FRENO, this);
	m_intermitente_di.SubclassDlgItem(IDC_INTERMITENTE_DI, this);
	m_intermitente_dd.SubclassDlgItem(IDC_INTERMITENTE_DD, this);
	m_intermitente_ti.SubclassDlgItem(IDC_INTERMITENTE_TI, this);
	m_intermitente_td.SubclassDlgItem(IDC_INTERMITENTE_TD, this);
	// Set port
	m_port = 504;
	UpdateData(false);
	// Initialize SImulated Memory
	Memory = new CMyMemory();
	Memory->Reset();
	// Initialize ModBus Protocol
	MyModBus = new ModBus(Memory);
	MyModBus->SetServer();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLucesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLucesDlg::OnPaint()
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
HCURSOR CLucesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLucesDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here

	switch (m_startstop.GetCheck())
	{
	case BST_CHECKED:
		m_startstop.SetWindowText(_T("Stop"));

		// Create Socket
		FirstSocket = new CMySocket(this);
		AcceptedSocket = new CMySocket(this);
		FirstSocket->Create(m_port, SOCK_STREAM);
		FirstSocket->Listen();

		// TImer intermitentes
		SetTimer(ID_TIMER_INTERMITENTES, 400, NULL);

		break;

	case BST_UNCHECKED:
		m_startstop.SetWindowText(_T("Start"));

		// Close Sockets
		AcceptedSocket->Close();
		delete[] AcceptedSocket;
		FirstSocket->Close();
		delete[] FirstSocket;

		break;
	default:
		break;
	}
}


void CLucesDlg::UpdateValues(){
	/*
	Cuando se reciban datos por el ModBus, se llamará a esta función para cambiar lo que se deba
	*/
	// Freno
	if (Memory->Registers[0].Value == 1){ m_freno.color = m_freno.rojo; }
	else{ m_freno.color = m_freno.gris; }
	// Intermitente izquierda
	if (Memory->Registers[1].Value == 1){ 
		m_intermitente_di.IsOn = true;
		m_intermitente_ti.IsOn = true;
	}
	else{
		m_intermitente_di.IsOn = false;
		m_intermitente_ti.IsOn = false;
	}
	// Intermitente izquierda	
	if (Memory->Registers[2].Value == 1){
		m_intermitente_dd.IsOn = true;
		m_intermitente_td.IsOn = true;
	}
	else{
		m_intermitente_dd.IsOn = false;
		m_intermitente_td.IsOn = false;
	}
}

void CLucesDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ID_TIMER_INTERMITENTES){
		if (LightsOn){
			LightsOn = false;
			if (m_intermitente_di.IsOn){
				m_intermitente_di.color = m_intermitente_di.ambar;
				m_intermitente_ti.color = m_intermitente_ti.ambar;
			}
			if (m_intermitente_dd.IsOn){
				m_intermitente_dd.color = m_intermitente_dd.ambar;
				m_intermitente_td.color = m_intermitente_td.ambar;
			}
		}
		else{
			LightsOn = true;
			m_intermitente_dd.color = m_intermitente_dd.gris;
			m_intermitente_td.color = m_intermitente_td.gris;
			m_intermitente_di.color = m_intermitente_di.gris;
			m_intermitente_ti.color = m_intermitente_ti.gris;
		}

		Invalidate(0);
	}

	CDialogEx::OnTimer(nIDEvent);
}
