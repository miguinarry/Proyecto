
// AccionamientosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Accionamientos.h"
#include "AccionamientosDlg.h"
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


// CAccionamientosDlg dialog



CAccionamientosDlg::CAccionamientosDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAccionamientosDlg::IDD, pParent)
	, m_port(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAccionamientosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Control(pDX, IDC_DER, m_der);
	DDX_Control(pDX, IDC_FRENO, m_freno);
	DDX_Control(pDX, IDC_IZQ, m_izq);
	DDX_Control(pDX, IDC_STARTSTOP, m_startstop);
}

BEGIN_MESSAGE_MAP(CAccionamientosDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STARTSTOP, &CAccionamientosDlg::OnBnClickedStartstop)
	ON_BN_CLICKED(IDC_FRENO, &CAccionamientosDlg::OnBnClickedFreno)
	ON_BN_CLICKED(IDC_IZQ, &CAccionamientosDlg::OnBnClickedIzq)
	ON_BN_CLICKED(IDC_DER, &CAccionamientosDlg::OnBnClickedDer)
END_MESSAGE_MAP()


// CAccionamientosDlg message handlers

BOOL CAccionamientosDlg::OnInitDialog()
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
	m_port = 503;
	UpdateData(false);
	Memory = new CMyMemory();
	Memory->Reset();
	MyModBus = new ModBus(Memory);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAccionamientosDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAccionamientosDlg::OnPaint()
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
HCURSOR CAccionamientosDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CAccionamientosDlg::OnBnClickedStartstop()
{
	// Get actual value of the port
	UpdateData(true);

	switch (m_startstop.GetCheck())
	{
	case BST_UNCHECKED:
		m_startstop.SetWindowText(_T("Start"));

		// Close Sockets
		AcceptedSocket->Close();
		delete[] AcceptedSocket;
		FirstSocket->Close();
		delete[] FirstSocket;
		break;

	case BST_CHECKED:
		m_startstop.SetWindowText(_T("Stop"));

		// Create Socket
		FirstSocket = new CMySocket(this);
		AcceptedSocket = new CMySocket(this);
		FirstSocket->Create(m_port, SOCK_STREAM);
		FirstSocket->Listen();

		break;
	}
}




void CAccionamientosDlg::OnBnClickedFreno()
{
	// TODO: Add your control notification handler code here
	if (m_freno.GetCheck() == BST_CHECKED){
		Memory->Registers[0].Value = 1;
	}
	else{
		Memory->Registers[0].Value = 0;
	}
}


void CAccionamientosDlg::OnBnClickedIzq()
{
	// TODO: Add your control notification handler code here
	if (m_izq.GetCheck() == BST_CHECKED){
		Memory->Registers[1].Value = 1;
	}
	else{
		Memory->Registers[1].Value = 0;
	}
}


void CAccionamientosDlg::OnBnClickedDer()
{
	// TODO: Add your control notification handler code here
	if (m_der.GetCheck() == BST_CHECKED){
		Memory->Registers[2].Value = 1;
	}
	else{
		Memory->Registers[2].Value = 0;
	}
}
