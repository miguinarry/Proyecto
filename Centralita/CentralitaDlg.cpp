
// CentralitaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Centralita.h"
#include "CentralitaDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Thread functions declaration
UINT AccionamientosSend(LPVOID Param);
UINT MotorSend(LPVOID Param);
UINT LucesSend(LPVOID Param);

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


// CCentralitaDlg dialog



CCentralitaDlg::CCentralitaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCentralitaDlg::IDD, pParent)
	, m_luces_ip(_T(""))
	, m_accionamientos_ip(_T(""))
	, m_accionamientos_port(0)
	, m_luces_port(0)
	, m_motor_ip(_T(""))
	, m_motor_port(0)
	, m_polling_time(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCentralitaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_LUCES_PORT, m_luces_port);
	DDX_Text(pDX, IDC_LUCES_IP, m_luces_ip);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_ACCIONAMIENTOS_IP, m_accionamientos_ip);
	DDX_Text(pDX, IDC_ACCIONAMIENTOS_PORT, m_accionamientos_port);
	DDX_Text(pDX, IDC_LUCES_PORT, m_luces_port);
	DDX_Text(pDX, IDC_MOTOR_IP, m_motor_ip);
	DDX_Text(pDX, IDC_MOTOR_PORT, m_motor_port);
	DDX_Text(pDX, IDC_POLLING_TIME, m_polling_time);
	DDX_Control(pDX, IDC_POLLING_STARTSTOP, m_polling);
}

BEGIN_MESSAGE_MAP(CCentralitaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_EXIT, &CCentralitaDlg::OnFileExit)
	ON_BN_CLICKED(IDC_CLEAR, &CCentralitaDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_POLLING_STARTSTOP, &CCentralitaDlg::OnBnClickedPollingStartstop)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCentralitaDlg message handlers

BOOL CCentralitaDlg::OnInitDialog()
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
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//									INICIALIZACION
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	// Set leds
	m_motor_led.SubclassDlgItem(IDC_MOTOR_LED, this);
	m_accionamientos_led.SubclassDlgItem(IDC_ACCIONAMIENTOS_LED, this);
	m_accionamientos_izq.SubclassDlgItem(IDC_ACCIONAMIENTOS_IZQ, this);
	m_accionamientos_der.SubclassDlgItem(IDC_ACCIONAMIENTOS_DER, this);
	m_accionamientos_freno.SubclassDlgItem(IDC_ACCIONAMIENTOS_FRENO, this);
	m_luces_led.SubclassDlgItem(IDC_LUCES_LED, this);
	m_motor_led.color = m_motor_led.rojo;
	m_accionamientos_led.color = m_accionamientos_led.rojo;
	m_luces_led.color = m_luces_led.rojo;
	// Set gauge
	m_motor_temperatura.SetMaxValue(300);
	m_motor_rpm.SetMaxValue(7000);	
	m_motor_temperatura.SubclassDlgItem(IDC_MOTOR_TEMPERATURA, this);
	m_motor_rpm.SubclassDlgItem(IDC_MOTOR_RPM, this);
	// Set the IP field
	m_accionamientos_ip = "127.0.0.1";
	m_luces_ip = "127.0.0.1";
	m_motor_ip = "127.0.0.1";
	// Set the PORT field
	m_motor_port = 502;
	m_accionamientos_port = 503;
	m_luces_port = 504;
	// Set polling TIME
	m_polling_time = 500;
	ShowLog("Centralita Iniciada");
	// Start Memory
	Memory = new CMyMemory();
	Memory->Reset();
	// Start Modbus
	ModBusAccionamientos = new ModBus(Memory);
	ModBusMotor = new ModBus(Memory);
	ModBusLuces = new ModBus(Memory);
	UpdateData(false);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCentralitaDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCentralitaDlg::OnPaint()
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
HCURSOR CCentralitaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCentralitaDlg::OnFileExit()
{
	// TODO: Add your command handler code here
	CDialogEx::OnCancel();
}


void CCentralitaDlg::OnBnClickedClear()
{
	// TODO: Add your control notification handler code here
	m_list.ResetContent();
}


void CCentralitaDlg::ShowLog(CString msg, bool NewLine){
	if (NewLine) {
		m_list.AddString("¯¯¯¯¯¯¯¯¯"); m_list.AddString(" "); m_list.AddString("_________");
	}
	// Funcion que saca un string por el log con el timestamp
	COleDateTime dateTest = COleDateTime::GetCurrentTime();
	CString cs; cs.Format("%02d:%02d:%02d  |  %s", dateTest.GetHour(), dateTest.GetMinute(), dateTest.GetSecond(), msg);
	m_list.AddString(cs);
	// Mover barra de desplazamiento al final
	m_list.SetCurSel(m_list.GetCount() - 1);
}

void CCentralitaDlg::UpdateValuesFromMemory(){
	// freno
	if (Memory->Registers[0].Value == 1){ m_accionamientos_freno.color = m_accionamientos_freno.rojo; }
	else{ m_accionamientos_freno.color = m_accionamientos_freno.gris; }
	// Intermitente izquierdo
	if (Memory->Registers[1].Value == 1) {
		m_accionamientos_izq.IsOn = true;
	}
	else {
		m_accionamientos_izq.IsOn = false;
	}
	// Intermitente derecho
	if (Memory->Registers[2].Value == 1){
		m_accionamientos_der.IsOn = true;
	}
	else{
		m_accionamientos_der.IsOn = false;
	}
	// Sensor de temperatura
	m_motor_temperatura.nivel = Memory->Registers[3].Value;
	// Sensor de RPM
	m_motor_rpm.nivel = Memory->Registers[4].Value;
	Invalidate(0);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//***                                         TIMERS                                              ***//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
void CCentralitaDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString cs;

	switch (nIDEvent)
	{
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	case ID_TIMER_INTERMITENTES:
		if (LightsOn){
			LightsOn = false;
			if (m_accionamientos_izq.IsOn) m_accionamientos_izq.color = m_accionamientos_izq.ambar;
			if (m_accionamientos_der.IsOn) m_accionamientos_der.color = m_accionamientos_der.ambar;
		}
		else{
			LightsOn = true;
			m_accionamientos_izq.color = m_accionamientos_izq.gris;
			m_accionamientos_der.color = m_accionamientos_der.gris;
		}
		Invalidate(false);
		break;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	case ID_TIMER_POLLING:
		//// ACCIONAMIENTOS => Read
		if (SocketAccionamientos->IsConnected)
			THAccionamientos = AfxBeginThread(AccionamientosSend, this);

		//// MOTOR => READ
		if (SocketMotor->IsConnected)
			THMotor = AfxBeginThread(MotorSend, this);

		//// LUCES => WRITE
		if (SocketLuces->IsConnected)
			THLuces = AfxBeginThread(LucesSend, this);

		break;
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	case ID_TIMER_RECONNECT:


		// Reconnect ALMACENAMIENTOS
		if (!SocketAccionamientos->IsConnected){
			if (SocketAccionamientos->ReConnect == 0){
				SetTimer(ID_TIMER_RECONNECT, RECONNECT_TIME, NULL);
				ShowLog("   - Accionamientos Desconectado");
				// Deactivate Leds
				m_accionamientos_led.color = m_accionamientos_led.rojo;
				Invalidate(0);
				// Reset socket
				delete[] SocketAccionamientos;
				SocketAccionamientos = new CMySocket(this);
				SocketAccionamientos->Create();
			}

			if (!SocketAccionamientos->Connect(m_accionamientos_ip, m_accionamientos_port)){
				if (SocketAccionamientos->ReConnect < NUMBER_RECONNECTIOS){
					cs.Format("   - Intento %d de reconexión del socket de Accionamientos", ++SocketAccionamientos->ReConnect);
					ShowLog(cs);
				}
				if (SocketAccionamientos->ReConnect == NUMBER_RECONNECTIOS){
					SocketAccionamientos->ReConnect++;
					ShowLog("   - Reconexión del socket de Accionamientos en segundo plano...");
					KillTimer(ID_TIMER_RECONNECT);
					SetTimer(ID_TIMER_RECONNECT, RECONNECT_TIME_2PLANO, NULL);
				}
			}
			else{
				// Start connection flag
				SocketAccionamientos->IsConnected = true;
				SocketAccionamientos->ReConnect = 0;
				// Activate Leds
				m_accionamientos_led.color = m_accionamientos_led.verde;
				Invalidate(0);
				// Assign ID to the socket
				SocketAccionamientos->ID = TARGET_ACCIONAMIENTOS;
				// Modbus
				ModBusAccionamientos->SetClient();
				ModBusAccionamientos->SetTarget(TARGET_ACCIONAMIENTOS);
				ShowLog("   - Accionamientos Reconectado");
			}
		}


		// Reconnect LUCES
		if (!SocketLuces->IsConnected){
			if (SocketLuces->ReConnect == 0){
				SetTimer(ID_TIMER_RECONNECT, RECONNECT_TIME, NULL);
				ShowLog("   - Luces Desconectado");
				// Deactivate Leds
				m_luces_led.color = m_luces_led.rojo;
				Invalidate(0);
				// Reset socket
				delete[] SocketLuces;
				SocketLuces = new CMySocket(this);
				SocketLuces->Create();
			}
			if (!SocketLuces->Connect(m_luces_ip, m_luces_port)){
				if (SocketLuces->ReConnect < NUMBER_RECONNECTIOS){
					cs.Format("   - Intento %d de reconexión del socket de Luces", ++SocketLuces->ReConnect);
					ShowLog(cs);
				}
				if (SocketLuces->ReConnect == NUMBER_RECONNECTIOS) {
					SocketLuces->ReConnect++;
					ShowLog("   - Reconexión del socket de Luces en segundo plano...");
					KillTimer(ID_TIMER_RECONNECT);
					SetTimer(ID_TIMER_RECONNECT, RECONNECT_TIME_2PLANO, NULL);
				}
			}
			else{
				// Start connection flag
				SocketLuces->IsConnected = true;
				SocketLuces->ReConnect = 0;
				// Activate Leds
				m_luces_led.color = m_luces_led.verde;
				Invalidate(0);
				// Assign ID to the socket
				SocketLuces->ID = TARGET_LUCES;
				// Modbus
				ModBusLuces->SetClient();
				ModBusLuces->SetTarget(TARGET_LUCES);
				ShowLog("   - Luces Reconectado");
			}
		}


		// Reconnect MOTOR
		if (!SocketMotor->IsConnected){
			if (SocketMotor->ReConnect == 0){
				SetTimer(ID_TIMER_RECONNECT, RECONNECT_TIME, NULL);
				ShowLog("   - Motor Desconectado");
				// Deactivate Leds
				m_motor_led.color = m_motor_led.rojo;
				Invalidate(0);
				// Reset socket
				delete[] SocketMotor;
				SocketMotor = new CMySocket(this);
				SocketMotor->Create();
			}
			if (!SocketMotor->Connect(m_motor_ip, m_motor_port)){
				if (SocketMotor->ReConnect < NUMBER_RECONNECTIOS) {
					cs.Format("   - Intento %d de reconexión del socket de Motor", ++SocketMotor->ReConnect);
					ShowLog(cs);
				}
				if (SocketMotor->ReConnect == NUMBER_RECONNECTIOS) {
					SocketMotor->ReConnect++;
					ShowLog("   - Reconexión del socket de Motor en segundo plano...");
					KillTimer(ID_TIMER_RECONNECT);
					SetTimer(ID_TIMER_RECONNECT, RECONNECT_TIME_2PLANO, NULL);
				}
			}
			else{
				// Start connection flag
				SocketMotor->IsConnected = true;
				SocketMotor->ReConnect = 0;
				// Activate Leds
				m_motor_led.color = m_motor_led.verde;
				Invalidate(0);
				// Assign ID to the socket
				SocketMotor->ID = TARGET_MOTOR;
				// Modbus
				ModBusMotor->SetClient();
				ModBusMotor->SetTarget(TARGET_MOTOR);
				ShowLog("   - Motor Reconectado");
			}
		}

		if (SocketAccionamientos->IsConnected && SocketLuces->IsConnected && SocketMotor->IsConnected){
			KillTimer(ID_TIMER_RECONNECT);
		}
		break;

	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//***                                         STARTSTOP                                           ***//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
void CCentralitaDlg::OnBnClickedPollingStartstop()
{
	// Get actual value of the variables
	UpdateData(true);

	switch (m_polling.GetCheck())
	{
	case BST_CHECKED:
		m_polling.SetWindowText(_T("Stop"));
		ShowLog("***    Polling Iniciado    ***", true);
	
		////ACCIONAMIENTOS
		// Create sockets
		SocketAccionamientos = new CMySocket(this);
		if (!SocketAccionamientos->Create()){
			ShowLog(" => ERROR en la creación del socket de Accionamientos");
		}
		if (!SocketAccionamientos->Connect(m_accionamientos_ip, m_accionamientos_port)){
			ShowLog(" => ERROR en la conexión del socket de Accionamientos");
		}
		else{
			// Start connection flag
			SocketAccionamientos->IsConnected = true;
			// Activate Leds
			m_accionamientos_led.color = m_accionamientos_led.verde;
			Invalidate(0);
			// Assign ID to the socket
			SocketAccionamientos->ID = TARGET_ACCIONAMIENTOS;
			// Modbus
			ModBusAccionamientos->SetClient();
			ModBusAccionamientos->SetTarget(TARGET_ACCIONAMIENTOS);
			ShowLog(" Accionamientos conectado");
		}
		
		////MOTOR
		// Create sockets
		SocketMotor = new CMySocket(this);
		if (!SocketMotor->Create()){
			ShowLog(" => ERROR en la creación del socket de Motor");
		}
		if (!SocketMotor->Connect(m_motor_ip, m_motor_port)){
			ShowLog(" => ERROR en la conexión del socket de Motor");
		}
		else{
			// Start connection flag
			SocketMotor->IsConnected = true;
			// Activate Leds
			m_motor_led.color = m_motor_led.verde;
			Invalidate(0);
			// Assign ID to the socket
			SocketMotor->ID = TARGET_MOTOR;
			// Modbus
			ModBusMotor->SetClient();
			ModBusMotor->SetTarget(TARGET_MOTOR);
			ShowLog(" Motor conectado");
		}

		////LUCES
		// Create sockets
		SocketLuces = new CMySocket(this);
		if (!SocketLuces->Create()){
			ShowLog(" => ERROR en la creación del socket de Luces");
		}
		if (!SocketLuces->Connect(m_luces_ip, m_luces_port)){
			ShowLog(" => ERROR en la conexión del socket de Luces");
		}
		else{
			// Start connection flag
			SocketLuces->IsConnected = true;
			// Activate Leds
			m_luces_led.color = m_luces_led.verde;
			Invalidate(0);
			// Assign ID to the socket
			SocketLuces->ID = TARGET_LUCES;
			// Modbus
			ModBusLuces->SetClient();
			ModBusLuces->SetTarget(TARGET_LUCES);
			ShowLog(" Luces conectado");
		}

		// Start timers
		SetTimer(ID_TIMER_RECONNECT, RECONNECT_TIME, NULL);
		SetTimer(ID_TIMER_POLLING, m_polling_time, NULL);
		SetTimer(ID_TIMER_INTERMITENTES, 400, NULL);
		break;

	case BST_UNCHECKED:
		m_polling.SetWindowText(_T("Start"));

		// Stop timers
		KillTimer(ID_TIMER_POLLING);
		KillTimer(ID_TIMER_RECONNECT);

		//// ACCIONAMIENTOS
		try{
			// Deactivate Leds
			m_accionamientos_led.color = m_accionamientos_led.rojo;
			Invalidate(0);

			// Delete socket for accionamientos
			SocketAccionamientos->Close();
			delete[] SocketAccionamientos;
		}
		catch (...){}

		//// MOTOR
		try{
			// Deactivate Leds
			m_motor_led.color = m_motor_led.rojo;
			Invalidate(0);

			// Close socket for accionamientos
			SocketMotor->Close();
			delete[] SocketMotor;
		}
		catch (...){}

		//// Luces
		try{
			// Deactivate Leds
			m_luces_led.color = m_luces_led.rojo;
			Invalidate(0);

			// Delete socket for accionamientos
			SocketLuces->Close();
			delete[] SocketLuces;
		}
		catch (...){}

		

		ShowLog("***    Polling Terminado    ***");
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//***                                         THREADS                                             ***//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

UINT AccionamientosSend(LPVOID Param){
	CCentralitaDlg *pDlg = (CCentralitaDlg*)Param;

	unsigned char Buffer[12];
	//Prepare ModBus Request
	pDlg->ModBusAccionamientos->SendRead(Buffer, 400, 3);
	// Send Message
	pDlg->SocketAccionamientos->Send(Buffer, 12);

	return 0;
}

UINT LucesSend(LPVOID Param){
	CCentralitaDlg *pDlg = (CCentralitaDlg*)Param;

	unsigned char Buffer[12];
	//Prepare ModBus Request: Freno
	pDlg->ModBusLuces->SendWrite(Buffer, 500, pDlg->Memory->Registers[0].Value);
	pDlg->SocketLuces->Send(Buffer, 12);
	//Prepare ModBus Request: Intermitente Izquierdo
	pDlg->ModBusLuces->SendWrite(Buffer, 501, pDlg->Memory->Registers[1].Value);
	pDlg->SocketLuces->Send(Buffer, 12);
	pDlg->ModBusLuces->SendWrite(Buffer, 503, pDlg->Memory->Registers[1].Value);
	pDlg->SocketLuces->Send(Buffer, 12);
	//Prepare ModBus Request: Intermitente Derecho
	pDlg->ModBusLuces->SendWrite(Buffer, 502, pDlg->Memory->Registers[2].Value);
	pDlg->SocketLuces->Send(Buffer, 12);
	pDlg->ModBusLuces->SendWrite(Buffer, 504, pDlg->Memory->Registers[2].Value);
	pDlg->SocketLuces->Send(Buffer, 12);

	return 0;
}

UINT MotorSend(LPVOID Param){
	CCentralitaDlg *pDlg = (CCentralitaDlg*)Param;

	unsigned char Buffer[12];
	//Prepare ModBus Request
	pDlg->ModBusMotor->SendRead(Buffer, 400, 2);
	// Send Message
	pDlg->SocketMotor->Send(Buffer, 12);

	return 0;
}



