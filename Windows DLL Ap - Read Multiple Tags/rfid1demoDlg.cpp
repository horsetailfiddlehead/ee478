// rfid1demoDlg.cpp : implementation file
#include "stdafx.h"
#include "rfid1demo.h"
#include "rfid1demoDlg.h"
#include "FTD2XX.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ascitohex(c)    (c <= '9' ? c - '0' : ((c <= 'F') ? c-'A'+10 : c - 'a'+10 ))
#define MYBUFFERSIZE 3000

unsigned char grx[MYBUFFERSIZE];//global receive buffer
int valid_port;
int	process_active;
FT_HANDLE m_ftHandle;

int BatchRunning=0;
CString g_str_array[300];

int data8[8];//array for holding 8 blocks



// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRfid1demoDlg dialog

CRfid1demoDlg::CRfid1demoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRfid1demoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRfid1demoDlg)
	m_NumTags = _T("");
	m_MyText = _T("");
	m_PortStatus = _T("");
	m_NumChars = 0;
	m_Addr = _T("00");
	m_Data = _T("");
	m_BFound = _T("");
	m_PassCount = 0;
	m_UID = _T("");
	m_DSFID = _T("");
	m_AFI = _T("");
	m_TagType = _T("");
	m_MemSize = _T("");
	m_Dsfid2Write = _T("");
	m_AFI2Write = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRfid1demoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRfid1demoDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_LIST1, m_SniffList);
	DDX_Text(pDX, IDC_EDIT_NUMTAGS, m_NumTags);
	DDX_Text(pDX, IDC_EDIT_MYTEXT, m_MyText);
	DDV_MaxChars(pDX, m_MyText, 256);
	DDX_Text(pDX, IDC_EDIT_STATUS, m_PortStatus);
	DDX_Text(pDX, IDC_EDIT_NUM_CHARS, m_NumChars);
	DDX_Text(pDX, IDC_EDIT_BLOCK_ADDR, m_Addr);
	DDX_Text(pDX, IDC_EDIT_BLOCK_32BITDATA2, m_Data);
	DDX_Text(pDX, IDC_EDIT_FOUND_BATCH, m_BFound);
	DDX_Text(pDX, IDC_EDIT_PASS_COUNT, m_PassCount);
	DDX_Text(pDX, IDC_EDIT_UID, m_UID);
	DDX_Text(pDX, IDC_EDIT_DSFID, m_DSFID);
	DDX_Text(pDX, IDC_EDIT_AFI, m_AFI);
	DDX_Text(pDX, IDC_EDIT_TAG_TYPE, m_TagType);
	DDX_Text(pDX, IDC_EDIT_MEM_SIZE, m_MemSize);
	DDX_Text(pDX, IDC_EDIT_DSFID_2WRITE, m_Dsfid2Write);
	DDX_Text(pDX, IDC_EDIT_AFI2WRITE, m_AFI2Write);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRfid1demoDlg, CDialog)
	//{{AFX_MSG_MAP(CRfid1demoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTONREAD, OnButtonread)
	ON_EN_CHANGE(IDC_EDIT_MYTEXT, OnChangeEditMytext)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_TAG, OnButtonWriteTag)
	ON_BN_CLICKED(IDC_BUTTON_TEST_PATTERN, OnButtonTestPattern)
	ON_BN_CLICKED(IDC_BUTTON_PASS_BEEP, OnButtonPassBeep)
	ON_BN_CLICKED(IDC_BUTTON_FAIL_BEEP, OnButtonFailBeep)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_SINGLE, OnButtonWriteSingle)
	ON_BN_CLICKED(IDC_BUTTON_READ_BLOCK, OnButtonReadBlock)
	ON_BN_CLICKED(IDC_BUTTON_STAY_QUIET, OnButtonStayQuiet)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_READ_BATCH, OnButtonReadBatch)
	ON_BN_CLICKED(IDC_BUTTON_STOPBATCH, OnButtonStopbatch)
	ON_BN_CLICKED(IDC_BUTTON_READ_STATUS, OnButtonReadStatus)
	ON_BN_CLICKED(IDC_BUTTON_LOCK_BLOCK, OnButtonLockBlock)
	ON_BN_CLICKED(IDC_BUTTON_DSFUID_WRITE, OnButtonDsfuidWrite)
	ON_BN_CLICKED(IDC_BUTTON_AFI_WRITE, OnButtonAfiWrite)
	ON_BN_CLICKED(IDC_BUTTON_DSFID_LOCK, OnButtonDsfidLock)
	ON_BN_CLICKED(IDC_BUTTON_AFI_LOCK, OnButtonAfiLock)
	ON_BN_CLICKED(IDC_BUTTON_READ_LOCKSTATE, OnButtonReadLockstate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRfid1demoDlg message handlers

void CRfid1demoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRfid1demoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRfid1demoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//**********************************************************************************************
BOOL CRfid1demoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	valid_port=0;
	process_active=0;
	
	OpenPort(1, 1, 1);//reset, beep, notify_open in listbox


	return TRUE;  // return TRUE  unless you set the focus to a control
}


//**********************************************************************************************
int CRfid1demoDlg::OpenPort(int reset, int beep, int notify_open)
{
	DWORD numDevs;
	int res;
	CString sss;

	valid_port=0;//init
	FT_STATUS status = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
	if(status == FT_OK) 
	{
		if((int)numDevs > 0)
		{
			FT_STATUS status = FT_OpenEx((PVOID)(LPCTSTR)"DLP-RFID1", FT_OPEN_BY_DESCRIPTION, &m_ftHandle);
			if(status != FT_OK) 
			{
				AfxMessageBox("Error: Could not open DLP-RFID1 Reader.");				
				return 1;
			}
			else
			{
				FT_Purge(m_ftHandle, FT_PURGE_RX || FT_PURGE_TX); 
				FT_SetTimeouts(m_ftHandle, 100, 100);
				FT_SetBaudRate(m_ftHandle, 115200);

				if(reset)
				{
					//set DTR
					status = FT_SetDtr(m_ftHandle);
					Sleep(20); //wait for cap to charge
					//Clear DTR
					status = FT_ClrDtr(m_ftHandle);
					Sleep(110);//wait for micro to start up
				}

				
				res = PingReader();
				if(res==0)//no error
				{
					if(beep)
						SendBeep1();

					if(notify_open)
					{
						sss.Format("Port Open, RFID1 Ready");
						m_SniffList.InsertString(0,sss);
						UpdateWindow();
					}
				}
				else
				{
					sss.Format("No RFID1 Detected");
					m_SniffList.InsertString(0,sss);
					UpdateWindow();
					//AfxMessageBox("Error: No reader detected.");				
					return 2;
				}

			}
		}
		else
		{
			sss.Format("No RFID1 Detected");
			m_SniffList.InsertString(0,sss);
			UpdateWindow();
			//AfxMessageBox("Error: No reader detected.");				
			return 2;
		}
	}
	else
	{
		sss.Format("No RFID1 Detected");
		m_SniffList.InsertString(0,sss);
		UpdateWindow();
		//AfxMessageBox("Error: No reader detected.");	
		return 3;
	}

	return 0;
}



//*********************************************************************************
void CRfid1demoDlg::SendBeep1() 
{
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;
	unsigned char tx[600], rx[6000];
	CString stra, str;
	
	//build and send the Register Write Request packet
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x39;//9

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x39;//9
		
	tx[pos++] = 0x46;//F
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	
	
	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	Sleep(100);
	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);

}



//**********************************************************************************************
int CRfid1demoDlg::PingReader() 
{
	FT_STATUS status;
	unsigned char tx[200], rx[200];
	CString stra, str, sss;
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;
	int pos;

/*	
	//send a zero to stop the data spewing
	pos=0;
	tx[pos++] = 0x0;
	status = FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	Sleep(150);
	AmountInRxQueue=1;
	while(AmountInRxQueue>0)
	{
		FT_Read(m_ftHandle, rx, 50, &BytesReturned);
		FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	}

	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	Sleep(350);
*/
	//Ping the port and check for correct response
	pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x38;//8

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x46;//F
	tx[pos++] = 0x46;//F
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	status = FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	Sleep(50);
	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	if(AmountInRxQueue>195) AmountInRxQueue = 195;
	FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);

	if(BytesReturned < 3)
	{
		sss.Format("RFID1 not found");
		m_SniffList.InsertString(0,sss);
		UpdateWindow();
		return 1;
	}

	if(BytesReturned > 195)
	{
		sss.Format("RFID1 not found or tag(s) present in field");
		m_SniffList.InsertString(0,sss);
		UpdateWindow();
		return 1;
	}


	for(DWORD x=0; x<BytesReturned; x++)
	{
		stra.Format("%c", rx[x]);
		str+=stra;
	}

	if(str.Find("Dela", 0))
	{
		//pass
		valid_port=1;//this is the only place valid_port can be set
		PurgePort();
	}
	else
	{
		//fail
		sss.Format("RFID1 not found");
		m_SniffList.InsertString(0,sss);
		UpdateWindow();
		return 2;
	}
	
	int ret = PurgePort();
	//if(ret)//if any data is still in the buffer then warn
	//	AfxMessageBox("Warning: extra data in buffer...");

	return 0;
}



//**********************************************************************
int CRfid1demoDlg::PurgePort()
{
	int total_bytes_found;
	DWORD AmountInRxQueue, BytesReturned;

	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	total_bytes_found=AmountInRxQueue;//init counter
	while(AmountInRxQueue)
	{
		FT_Read(m_ftHandle, grx, AmountInRxQueue, &BytesReturned);
		Sleep(10);//wait for more to arrive
		FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
		total_bytes_found += AmountInRxQueue;
	}

	for(int x=0; x<MYBUFFERSIZE; x++)
		grx[x]=0xcc;

	return total_bytes_found;
}


//**********************************************************************
DWORD CRfid1demoDlg::FastRead(int count, int timeout)
{
	DWORD AmountInRxQueue, BytesReturned;

	
	if(count>MYBUFFERSIZE)
	{
		KillTimer(100);
		AfxMessageBox("array error 2");
		return 1;
	}

	AmountInRxQueue=0;
	while((timeout--) && (AmountInRxQueue<(DWORD)count))
	{
		Sleep(1);
		FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	}

	if(AmountInRxQueue <= (DWORD)count)
		FT_Read(m_ftHandle, grx, AmountInRxQueue, &BytesReturned);
	else
		FT_Read(m_ftHandle, grx, count, &BytesReturned);

	return AmountInRxQueue;
}

/*

	receive_count=0;
	while((timeout--) && (receive_count<count))
	{
		Sleep(1);
		receive_count = cp->RXCount();
	}

	if(receive_count == count)
	{
		dwBytes = cp->GetBuffer(grx, count, 30); 
		return dwBytes;
	}

	return 0;

*/




//**********************************************************************
void CRfid1demoDlg::SendRegwrtreq(int mode)//0=usermem, 1=UID //15693 Setup
{
	DWORD BytesWritten;
	unsigned char tx[600];
	CString stra, str;

	
	//build and send the Register Write Request packet
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x43;//C

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x32;//2
	tx[pos++] = 0x31;//1

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0

	//0=usermem, 1=UID
	if(mode==0)
		tx[pos++] = 0x30;// 0 for memory read/write
	if(mode==1)
		tx[pos++] = 0x32;// 2 for UID read

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	
	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	int ret = FastRead(53, 50);
}




//**********************************************************************
void CRfid1demoDlg::SendAgcToggle() 
{
	DWORD BytesWritten;
	unsigned char tx[600];
	CString stra, str;
	
	//build and send the Register Write Request packet
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x39;//9

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x46;//F
	tx[pos++] = 0x30;//0
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	
	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	int ret = FastRead(22, 50);

}




//**********************************************************************
void CRfid1demoDlg::SendAmPmToggle() 
{
	DWORD BytesWritten;
	unsigned char tx[600];
	CString stra, str;

	//build and send the Register Write Request packet
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x39;//9

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x46;//F
	tx[pos++] = 0x31;//1
		
	tx[pos++] = 0x46;//F
	tx[pos++] = 0x46;//F

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	
	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	int ret = FastRead(22, 50);
	//if(ret != 22)
	//{
	//	KillTimer(100);
	//	AfxMessageBox("fastread failed21");	
	//}

	
}



//**********************************************************************
void CRfid1demoDlg::OnButton15693invReq() 
{
	DWORD BytesWritten;
	char str_src[3000];
	char str_dest[3000];
	unsigned char tx[3000];
	DWORD ret_bytes, x;
	CString stra, str;

	m_SniffList.ResetContent();
	UpdateWindow();

	int ret = PurgePort();
	//if(ret)
	//	AfxMessageBox("data in buffer 4...");


	//build and send the Register Write Request packet
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x42;//B

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x34;//4
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x36;//6
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0


	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	int res = ret_bytes = FastRead(53, 50);
	//if(res != 53)
	//{
	//	KillTimer(100);
	//	AfxMessageBox("fastread failed4");	
	//}


	m_SniffList.ResetContent();

	for(x=0; x<ret_bytes; x++)
	{
		if(grx[x] == '[')
		{
			x++;
			str.Format(" ");
			while(grx[x] != ',')
			{
				stra.Format("%c", grx[x]);
				x++;
				str+=stra;
			}
		
			if(str.GetLength() >6)//if a UID was found...
			{
				strcpy(str_src, (LPCTSTR)str);

				//byte reverse
				int ptr=15;
				for(int t=0; t<16; t+=2)
				{
					str_dest[t] = str_src[ptr];
					str_dest[t+1] = str_src[ptr+1];
					str_dest[t+2] = NULL;
					ptr-=2;
				}

				CString sss;
				sss.Format("%s", str_dest);
				m_SniffList.InsertString(0,"UID:"+sss);//display the UID
				UpdateWindow();
			}
		}
	}
}





//**********************************************************************
void CRfid1demoDlg::SendRegwrtreqTagit() 
{
	DWORD BytesWritten;
	unsigned char tx[600];
	CString stra, str;
	
//0x30 = 0
//0x39 = 9
//0x41 = A
//0x42 = B
//0x43 = C
//0x44 = D
//0x45 = E
//0x46 = F

	//build and send the Register Write Request packet
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x43;//C

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x32;//2
	tx[pos++] = 0x31;//1

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x33;//3

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0


	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	int ret = FastRead(53, 100);
	//if(ret != 53)
	//{
	//	KillTimer(100);
 	//	AfxMessageBox("fastread failed22");	
	//}

}


/*
//**********************************************************************
void CRfid1demoDlg::SidpollReq() 
{
	DWORD BytesWritten;
	char str_src[3000];
	char str_dest[3000];
	unsigned char tx[3000];
	DWORD ret_bytes, x;
	CString stra, str;

	m_SniffList.ResetContent();
	UpdateWindow();

	int ret = PurgePort();
	//if(ret)
	//	AfxMessageBox("data in buffer 5...");


	//build and send the Register Write Request packet
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x42;//B

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x33;//3
	tx[pos++] = 0x34;//4
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x35;//5
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	int res = ret_bytes = FastRead(53, 50);
	//if(res != 53)
	//{
	//	KillTimer(100);
	//	AfxMessageBox("fastread failed23");	
	//}


	m_SniffList.ResetContent();

	for(x=0; x<ret_bytes; x++)
	{
		if(grx[x] == '[')
		{
			x++;
			str.Format(" ");
			while(grx[x] != ',')
			{
				stra.Format("%c", grx[x]);
				x++;
				str+=stra;
			}
		
			if(str.GetLength() >6)//if a UID was found...
			{
				strcpy(str_src, (LPCTSTR)str);

				//byte reverse
				int ptr=19;
 				for(int t=0; t<20; t+=2)
				{
					str_dest[t] = str_src[ptr];
					str_dest[t+1] = str_src[ptr+1];
					str_dest[t+2] = NULL;
					ptr-=2;
				}

				CString sss;
				sss.Format("%s", str_dest);
				m_SniffList.InsertString(0,"SID:"+sss);//display the UID
				UpdateWindow();
			}
		}
	}
}
*/

//**********************************************************************
DWORD CRfid1demoDlg::GetBufferSlow()
{
	unsigned char temprx[3];
	DWORD AmountInRxQueue, BytesReturned;
	int timeoutcount;

	int found=0, index=0;
	while(!found)//while have not found the ">" character
	{
		timeoutcount=0;
		AmountInRxQueue=0;
		while (AmountInRxQueue==0)//wait for at least one byte in the buffer with timeout
		{
			timeoutcount++;
			if(timeoutcount>250)
			{
				//KillTimer(100);
				//AfxMessageBox("error waiting for one byte");
				break;
			}
			FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
			if(AmountInRxQueue==0)//if no byte received then take a nap and wait for more bytes to arrive
				Sleep(1);
		}

		if(AmountInRxQueue>0)//if one byte is ready to be read
		{
			FT_Read(m_ftHandle, temprx, 1, &BytesReturned);//read in 1 byte at a time

			if(temprx[0] == '>')//if > character received then set the flag and clear the buffer which should be empty
			{
				found=1;
				FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
				FT_Read(m_ftHandle, temprx, AmountInRxQueue, &BytesReturned);//read in 1 byte at a time
			}

			grx[index++] = temprx[0];
			if(index > MYBUFFERSIZE-10)
			{
				found=1;//terminate loop/function
				int ret = PurgePort();
				//if(ret)
				//	AfxMessageBox("data in buffer 6...");

				index=0;
			}
		}
		else//timeout
		{
			PurgePort();//we got a timeout and likely a partial packet so purge and return
			return 0;
		}
	}

	return index;
}




//**********************************************************************
void CRfid1demoDlg::OnButtonStart() 
{
	SetTimer(100, 250, NULL);
}
//**********************************************************************
void CRfid1demoDlg::OnButtonStop() 
{
	KillTimer(100);
}


//**********************************************************************
void CRfid1demoDlg::OnTimer(UINT nIDEvent) 
{
	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}

	if(process_active)
		return;
	process_active=1;

	ReadBoth();
	
	process_active=0;

	CDialog::OnTimer(nIDEvent);
}






//**********************************************************************
void CRfid1demoDlg::ReadBoth() 
{
	DWORD BytesWritten;
	char str_src[3000];
	char str_dest[3000];
	unsigned char tx[3000];
	DWORD ret_bytes, x;
	CString stra, str;
	int TotNumTags;



	SendRegwrtreq(1);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();

	int ret = PurgePort();
	//if(ret)
	//	AfxMessageBox("data in buffer 7...");

	//build and send the Register Write Request packet
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x42;//B

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x34;//4
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x36;//6
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	
	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	ret_bytes = GetBufferSlow();

	m_SniffList.ResetContent();
	UpdateWindow();
	TotNumTags=0;

	for(x=0; x<ret_bytes; x++)
	{
		if(grx[x] == '[')
		{
			x++;
			str.Format(" ");
			int indx=0;
			while(grx[x] != ',')
			{
				stra.Format("%c", grx[x]);
				x++;
				indx++;
				str+=stra;
				if(indx>35)
				{
					str.Format(" ");
					break;
				}
			}
		
			if(str.GetLength() >6)//if a UID was found...
			{
				strcpy(str_src, (LPCTSTR)str);

				//byte reverse
				int ptr=15;
				for(int t=0; t<16; t+=2)
				{
					str_dest[t] = str_src[ptr];
					str_dest[t+1] = str_src[ptr+1];
					str_dest[t+2] = NULL;
					ptr-=2;
				}

				CString sss;
				sss.Format("%s", str_dest);
				m_SniffList.InsertString(0,"UID:"+sss);//display the UID
				TotNumTags++;
			}
		}
	}


	ret = PurgePort();
	//if(ret)
	//	AfxMessageBox("data in buffer 7...");

	//*********************************************
	SendRegwrtreqTagit();
	SendAgcToggle();
	SendAmPmToggle();

	ret = PurgePort();
	//if(ret)
	//	AfxMessageBox("data in buffer 8...");

	//build and send the Register Write Request packet
	pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x42;//B

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x33;//3
	tx[pos++] = 0x34;//4
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x35;//5
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	ret_bytes = GetBufferSlow();

	for(x=0; x<ret_bytes; x++)
	{
		if(grx[x] == '[')
		{
			x++;
			str.Format(" ");
			int indx=0;
			while(grx[x] != ',')
			{
				stra.Format("%c", grx[x]);
				x++;
				indx++;
				str+=stra;
				if(indx>35)
				{
					str.Format(" ");
					break;
				}
			}
		
			if(str.GetLength() >6)//if a UID was found...
			{
				strcpy(str_src, (LPCTSTR)str);

				//byte reverse
				int ptr=19;
				for(int t=0; t<20; t+=2)
				{
					str_dest[t] = str_src[ptr];
					str_dest[t+1] = str_src[ptr+1];
					str_dest[t+2] = NULL;
					ptr-=2;
				}

				CString sss;
				sss.Format("%s", str_dest);
				int y = m_SniffList.GetCount();
				m_SniffList.InsertString(y,"SID:"+sss);//display the UID
				TotNumTags++;
				UpdateWindow();
			}
		}
	}

	UpdateData(TRUE);
	m_NumTags.Format("%d tags", TotNumTags);
	UpdateData(FALSE);
	UpdateWindow();

	if(TotNumTags==0)
	{
		CString sss;
		sss.Format("NO      TAGS      FOUND");
		m_SniffList.InsertString(0,sss);
		m_SniffList.InsertString(0,sss);
		m_SniffList.InsertString(0,sss);
		m_SniffList.InsertString(0,sss);
		m_SniffList.InsertString(0,sss);
		UpdateWindow();
	}


	Sleep(40);//wait for any stragglers

	ret = PurgePort();
	//if(ret>1)
	//	AfxMessageBox("data in buffer 9...");
}




//*****************************************************************************
DWORD CRfid1demoDlg::hex2dec(char *addr)
{
	DWORD a, b, c, d, e, f, g, h;

	if(strlen(addr) == 1)
		return ascitohex(*addr);

	if(strlen(addr) == 2)
	{
		a = ascitohex(*addr) << 4;
		addr++;
		b = ascitohex(*addr);
		return a | b;
	}

	if(strlen(addr) == 3)
	{
		a = ascitohex(*addr) << 8;
		addr++;
		b = ascitohex(*addr) << 4;
		addr++;
		c = ascitohex(*addr);
		return a | b | c;
	}

	if(strlen(addr) == 4)
	{
		a = ascitohex(*addr) << 12;
		addr++;
		b = ascitohex(*addr) << 8;
		addr++;
		c = ascitohex(*addr) << 4;
		addr++;
		d = ascitohex(*addr);
		return a | b | c | d;
	}

	if(strlen(addr) == 5)
	{
		a = ascitohex(*addr) << 16;
		addr++;
		b = ascitohex(*addr) << 12;
		addr++;
		c = ascitohex(*addr) << 8;
		addr++;
		d = ascitohex(*addr) << 4;
		addr++;
		e = ascitohex(*addr);
		return a | b | c | d | e;
	}

	if(strlen(addr) == 6)
	{
		a = ascitohex(*addr) << 20;
		addr++;
		b = ascitohex(*addr) << 16;
		addr++;
		c = ascitohex(*addr) << 12;
		addr++;
		d = ascitohex(*addr) << 8;
		addr++;
		e = ascitohex(*addr) << 4;
		addr++;
		f = ascitohex(*addr);
		return a | b | c | d | e | f;
	}

	if(strlen(addr) == 7)
	{
		a = ascitohex(*addr) << 24;
		addr++;
		b = ascitohex(*addr) << 20;
		addr++;
		c = ascitohex(*addr) << 16;
		addr++;
		d = ascitohex(*addr) << 12;
		addr++;
		e = ascitohex(*addr) << 8;
		addr++;
		f = ascitohex(*addr) << 4;
		addr++;
		g = ascitohex(*addr);
		return a | b | c | d | e | f | g;
	}
	
	if(strlen(addr) == 8)
	{
		DWORD x;

		x=0;
		a = ascitohex(*addr) << 28;
		addr++;
		x |= a;
		b = ascitohex(*addr) << 24;
		addr++;
		x |= b;
		c = ascitohex(*addr) << 20;
		addr++;
		x |= c;
		d = ascitohex(*addr) << 16;
		addr++;
		x |= d;
		e = ascitohex(*addr) << 12;
		addr++;
		x |= e;
		f = ascitohex(*addr) << 8;
		addr++;
		x |= f;
		g = ascitohex(*addr) << 4;
		addr++;
		x |= g;
		h = ascitohex(*addr);
		x |= h;

		//x = a | b | c | d | e | f | g | h; 

		return x;
	}

	return 0;
}

//**********************************************************************
void CRfid1demoDlg::Read8Blocks(int section) //read 8 4-byte blocks starting at section
{
	char str_src[3000];
	char str_dest[3000];
	DWORD x;
	unsigned char tx[1000];//, rx[1000];//, strr[300];
	DWORD ret_bytes;
	CString stra, str;
	DWORD BytesWritten;

	for(x=0; x<2000; x++)
		grx[x] = 0;

	for(x=0; x<3000; x++)
	{
		str_src[x] = 0;
		str_dest[x] = 0;
	}

	//build and send the Read block command
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x43;//C

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x38;//8
		
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x32;//2
	tx[pos++] = 0x33;//3

	if(section==0)//block 0-7
	{	
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
	}
	if(section==1)//block 8-15
	{	
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x38;//8
	}
	if(section==2)//block 16-23
	{	
		tx[pos++] = 0x31;//1
		tx[pos++] = 0x30;//0
	}
	if(section==3)//block 24-31
	{	
		tx[pos++] = 0x31;//1
		tx[pos++] = 0x38;//8
	}

	if(section==4)//block 32-39
	{	
		tx[pos++] = 0x32;//2
		tx[pos++] = 0x30;//0
	}
	if(section==5)//block 40-47
	{	
		tx[pos++] = 0x32;//2
		tx[pos++] = 0x38;//8
	}
	if(section==6)//block 48-55
	{	
		tx[pos++] = 0x33;//3
		tx[pos++] = 0x30;//0
	}
	if(section==7)//block 56-63
	{	
		tx[pos++] = 0x33;//3
		tx[pos++] = 0x38;//8
	}
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x37;//7
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	PurgePort();
	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	ret_bytes = FastRead(127, 100);

	if(ret_bytes != 127)
	{
		//AfxMessageBox("error1");
		str.Format("(No Tag Found)");
		UpdateData(TRUE);
		m_MyText+=str;
		UpdateData(FALSE);
		UpdateWindow();

	   	//Initialize port ********************
		//set DTR
		FT_SetDtr(m_ftHandle);
		Sleep(20); //wait for cap to charge
		//Clear DTR
		FT_ClrDtr(m_ftHandle);
		Sleep(110);//wait for micro to start up

		PingReader();
		PurgePort();
		PingReader();
		PurgePort();

		return;
	}


	pos=0;

	while(grx[pos++] != '[')//find starting brace
	{
		if((DWORD)pos>ret_bytes)
		{
			AfxMessageBox("error2");
			return;
		}	
	}

	pos+=4;
	stra.Format("");
	for(x=0; x<8; x++)
	{
		int count=8;
		str.Format("");
		while(count--)
		{
			stra.Format("%c", grx[pos++]);
			str+=stra;
		}
	
		pos+=2;
	
		data8[x] = 0;//init
		if(str.GetLength() ==8)//if a read was performed
		{
			strcpy(str_src, (LPCTSTR)str);
			//byte reverse
			int ptr=6;
			for(int t=0; t<8; t+=2)
			{
				str_dest[t] = str_src[ptr];
				str_dest[t+1] = str_src[ptr+1];
				str_dest[t+2] = NULL;
				ptr-=2;
			}
			data8[x] = hex2dec(str_dest);
		}
	}

	return;
}



//**************************************************************************************
void CRfid1demoDlg::OnButtonread() 
{
	int pos;
	char b1, b2, b3, b4;
	CString str;
	DWORD data;

	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}


	PurgePort();

	UpdateData(TRUE);
	m_MyText = _T("");
	m_PortStatus = _T("Reading Tag...");
	m_TagType.Format(" ");
	UpdateData(FALSE);
	UpdateWindow();

	//determine the type of tag
	CString single = ReadBothReturnOne();
	if(single.GetLength() < 5)
	{
		m_SniffList.InsertString(0, "Tag not foud.");
		UpdateWindow();
		return;
	}

	m_SniffList.InsertString(0, single);
	UpdateWindow();

	CString type;
	type = single.Mid(4, 2);
	int dtype = hex2dec((char *)(LPCTSTR)type);	
	dtype &= 0xFE; //clear the lsb

	UpdateData(TRUE);
	if(dtype == 0x00)//HF-I Plus Inlay
		m_TagType.Format("HF-I Plus Inlay");
	if(dtype == 0x80)//HF-I Plus Chip
		m_TagType.Format("HF-I Plus Chip");
	if(dtype == 0xC0)//Standard Chip / Inlay
		m_TagType.Format("Standard Chip / Inlay");
	if(dtype == 0xC4)//Pro Chip / Inlay
		m_TagType.Format("Pro Chip / Inlay");
	UpdateData(FALSE);
	UpdateWindow();


	SendRegwrtreq(0);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();

	if( (dtype == 0x00) || (dtype == 0x80))//HF-I Plus 
	{
		//change Static text to read 256 Max characters
		GetDlgItem(IDC_STATIC_MAX_COUNT)->SetWindowText("Max:256");

		m_Progress.SetRange(0, 100);
		m_Progress.SetPos(0);

		for(int x=0; x<8; x++)
		{
			pos = (int)((float)x/(float)7.0*100.00);
			m_Progress.SetPos(pos);

			for(int y=0; y<8; y++)
				data8[y]=0;

			Read8Blocks(x);//data stored in data8[x]
			for(y=0; y<8; y++)
			{
				b1 = (data8[y]>>24) & 0x000000FF;
				b2 = (data8[y]>>16) & 0x000000FF;
				b3 = (data8[y]>>8) & 0x000000FF;
				b4 = data8[y] & 0x000000FF;
				str.Format("%c%c%c%c", b1, b2, b3, b4);
				
				UpdateData(TRUE);
				m_MyText+=str;
				UpdateData(FALSE);
				UpdateWindow();
			}
		}
	}
   else//Pro/Standard Read (one block at a time)
   {
		//change Static text to read 32 Max characters
		GetDlgItem(IDC_STATIC_MAX_COUNT)->SetWindowText("Max:32");

	   
	    m_Progress.SetRange(0, 7);
		m_Progress.SetPos(0);

		int error_code;
		for(int loop=0; loop<8; loop++)
		{
			m_Progress.SetPos(loop);
			data = ReadSingleBlock(loop, &error_code);

			b1 = (char)(data>>24) & 0x000000FF;
			b2 = (char)(data>>16) & 0x000000FF;
			b3 = (char)(data>>8) & 0x000000FF;
			b4 = (char)data & 0x000000FF;
			str.Format("%c%c%c%c", b1, b2, b3, b4);
			
			UpdateData(TRUE);
			m_MyText+=str;
			UpdateData(FALSE);
			UpdateWindow();
		}

   }

	OnChangeEditMytext();	
}




//********************************************************************************
void CRfid1demoDlg::OnChangeEditMytext() 
{
	UpdateData(TRUE);
	m_NumChars = m_MyText.GetLength();
	m_PortStatus = _T("Editing...");
	UpdateData(FALSE);
	UpdateWindow();
}




//********************************************************************************
void CRfid1demoDlg::OnButtonClear() 
{
	UpdateData(TRUE);
	m_MyText = _T("");
	UpdateData(FALSE);
	UpdateWindow();
	OnChangeEditMytext();
}




//********************************************************************************
void CRfid1demoDlg::OnButtonWriteTag() 
{
	unsigned char tx[100], rx[100], strj[10], stry[5]; 
	CString strr;
	CString stra, str, pos_str;
	int address, pos;
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;


	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}


	UpdateData(TRUE);
	m_PortStatus = _T("Writing Tag...");
	UpdateData(FALSE);
	UpdateWindow();

	
	int uu = m_MyText.GetLength() % 4;
	if(uu ==1) 
		m_MyText += _T("   ");
	if(uu ==2) 
		m_MyText += _T("  ");
	if(uu ==3) 
		m_MyText += _T(" ");

	while(m_MyText.GetLength() <=256)//file out the text with whitespace
		m_MyText += _T("    ");


	SendRegwrtreq(0);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();

	m_Progress.SetRange(0, 100);
	m_Progress.SetPos(0);


	for(address=0; address<m_MyText.GetLength()/4; address++)
	{
		pos = (int)((float)address/(float)64.0*100.00);
		m_Progress.SetPos(pos);

		sprintf((char *)strj, "%02.2X", address);

		int b1 = strj[0];
		int b2 = strj[1];

	
		strr = m_MyText.Mid(address*4, 4);
		

		sprintf((char *)strj, "%s", (LPCTSTR)strr);//This

		sprintf((char *)stry, "%02.2X", strj[0]);
		int b3 = stry[0];
		int b4 = stry[1];

		sprintf((char *)stry, "%02.2X", strj[1]);
		int b5 = stry[0];
		int b6 = stry[1];
		
		sprintf((char *)stry, "%02.2X", strj[2]);
		int b7 = stry[0];
		int b8 = stry[1];
		
		sprintf((char *)stry, "%02.2X", strj[3]);
		int b9 = stry[0];
		int b10 = stry[1];


		//build and send the Read block command
		int pos=0;
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x31;//1
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x46;//F

		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x33;//3
		
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x34;//4
		tx[pos++] = 0x31;//1
		tx[pos++] = 0x38;//8
			
		tx[pos++] = 0x34;//4
		tx[pos++] = 0x30;//0

		tx[pos++] = 0x32;//2
		tx[pos++] = 0x31;//1
		tx[pos++] = b1;//addr
		tx[pos++] = b2;//addr
		
		tx[pos++] = b9;//data
		tx[pos++] = b10;//data
		tx[pos++] = b7;//data
		tx[pos++] = b8;//data
		tx[pos++] = b5;//data
		tx[pos++] = b6;//data
		tx[pos++] = b3;//data
		tx[pos++] = b4;//data

		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
	
		FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
		Sleep(50);
		FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
		FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);
	}

	
	UpdateData(TRUE);
	m_PortStatus = _T("Done.");;
	UpdateData(FALSE);
	UpdateWindow();
}




//********************************************************************************
void CRfid1demoDlg::OnButtonTestPattern() 
{
	unsigned char tx[100], rx[100], strj[10]; 
	CString strr;
	CString stra, str, pos_str;
	int address, pos;
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;

	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}


	UpdateData(TRUE);
	m_PortStatus = _T("Writing Test Pattern...");
	UpdateData(FALSE);
	UpdateWindow();


	SendRegwrtreq(0);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();

	m_Progress.SetRange(0, 100);
	m_Progress.SetPos(0);


	for(address=0; address<256; address+=4)
	{
		pos = (int)((float)address/(float)256.0*100.00);
		m_Progress.SetPos(pos);

		sprintf((char *)strj, "%02.2X", address/4);

		int b1 = strj[0];
		int b2 = strj[1];


		//data =  hex2dec((char *)(LPCTSTR)m_Data);	
		sprintf((char *)strj, "%02.2X%02.2X%02.2X%02.2X", address, address+1, address+2, address+3);
		int b3 = strj[0];
		int b4 = strj[1];
		int b5 = strj[2];
		int b6 = strj[3];
		int b7 = strj[4];
		int b8 = strj[5];
		int b9 = strj[6];
		int b10 = strj[7];



		//build and send the Read block command
		int pos=0;
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x31;//1
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x46;//F

		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x33;//3
		
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x34;//4
		tx[pos++] = 0x31;//1
		tx[pos++] = 0x38;//8
			
		tx[pos++] = 0x34;//4
		tx[pos++] = 0x30;//0

		tx[pos++] = 0x32;//2
		tx[pos++] = 0x31;//1
		tx[pos++] = b1;//addr
		tx[pos++] = b2;//addr
		
		tx[pos++] = b9;//data
		tx[pos++] = b10;//data
		tx[pos++] = b7;//data
		tx[pos++] = b8;//data
		tx[pos++] = b5;//data
		tx[pos++] = b6;//data
		tx[pos++] = b3;//data
		tx[pos++] = b4;//data

		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		
		FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
		Sleep(50);
		FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
		FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);
	}




	UpdateData(TRUE);
	m_PortStatus = _T("Done.");;
	UpdateData(FALSE);
	UpdateWindow();
}


//**************************************************************************************
void CRfid1demoDlg::OnButtonPassBeep() 
{
	unsigned char tx[600], rx[600];
	CString stra, str;
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;


	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}

	
	//build and send the Register Write Request packet
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x39;//9

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x39;//9
		
	tx[pos++] = 0x46;//F
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	Sleep(50);
	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);
}




//***************************************************************************
void CRfid1demoDlg::OnButtonFailBeep() 
{
	unsigned char tx[600];
	unsigned char rx[600];
	CString stra, str;
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;

	

	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}

	//build and send the Register Write Request packet
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x39;//9

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x32;//2
	tx[pos++] = 0x30;//0
		
	tx[pos++] = 0x46;//F
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	Sleep(50);
	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);
}


//**************************************************************************
void CRfid1demoDlg::WriteSingleBlock(DWORD address, int data) 
{
	unsigned char tx[100], rx[1000], strr[30];
	CString stra, str;
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;

	SendRegwrtreq(0);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();

	sprintf((char *)strr, "%02.2X", address);
	int b1 = strr[0];
	int b2 = strr[1];

	sprintf((char *)strr, "%08.8X", data);
	int b3 = strr[0];
	int b4 = strr[1];
	int b5 = strr[2];
	int b6 = strr[3];
	int b7 = strr[4];
	int b8 = strr[5];
	int b9 = strr[6];
	int b10 = strr[7];


	//build and send the Read block command
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x46;//F

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x38;//8
		
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x32;//2
	tx[pos++] = 0x31;//1
	tx[pos++] = b1;//addr
	tx[pos++] = b2;//addr
	
	tx[pos++] = b9;//data
	tx[pos++] = b10;//data
	tx[pos++] = b7;//data
	tx[pos++] = b8;//data
	tx[pos++] = b5;//data
	tx[pos++] = b6;//data
	tx[pos++] = b3;//data
	tx[pos++] = b4;//data

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	Sleep(50);
	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);

}


//**************************************************************************
void CRfid1demoDlg::OnButtonWriteSingle() 
{
	CString stra, str;
	int address, data;

	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}

	UpdateData(TRUE);
	if(m_Addr.GetLength() > 2)
	{
		AfxMessageBox("Invalid Entry.  Range: 00-FF");
		return;
	}

	if(m_Data.GetLength() != 8)
	{
		AfxMessageBox("Invalid Entry.  Range: 00000000-FFFFFFFF");
		return;
	}

	address = hex2dec((char *)(LPCTSTR)m_Addr);	
	data =  hex2dec((char *)(LPCTSTR)m_Data);	

	WriteSingleBlock(address, data);
}




//************************************************************************************
DWORD CRfid1demoDlg::ReadSingleBlock(int addr, int *error) //return hex value
{
	char str_src[3000];
	char str_dest[3000];
	DWORD x;
	unsigned char tx[100], rx[100], strr[3];
	CString stra, str;
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;


	*error=0;//init to no error

	SendRegwrtreq(0);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();

	sprintf((char *)strr, "%02.2X", addr);

	int b1 = strr[0];//F
	int b2 = strr[1];//E



	//build and send the Read block command
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x42;//B

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x38;//8
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x32;//2
	tx[pos++] = 0x30;//0
	tx[pos++] = b1;//addr
	tx[pos++] = b2;//addr
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	
	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	Sleep(100);
	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);

	for(x=0; x<BytesReturned; x++)
	{
		if(rx[x] == '[')
		{
			x++;
			str.Format(" ");
			while(rx[x] != ']')
			{
				stra.Format("%c", rx[x]);
				x++;
				str+=stra;
			}
		
			if(str.GetLength() >10)//if a read was performed
			{
				strcpy(str_src, (LPCTSTR)str);
				//byte reverse
				int ptr=9;
				for(int t=0; t<8; t+=2)
				{
					str_dest[t] = str_src[ptr];
					str_dest[t+1] = str_src[ptr+1];
					str_dest[t+2] = NULL;
					ptr-=2;
				}

				DWORD x = hex2dec(str_dest);
				return x;
			}
			else
			{
				*error=1;//no data read
				return 0;
			}
		}
	}

	return 0;//default return
}



//************************************************************************************
void CRfid1demoDlg::OnButtonReadBlock() 
{
	CString stra, str;
	int address;
	int error_code;


	UpdateData(TRUE);
	m_Data = _T("");
	UpdateData(FALSE);
	UpdateWindow();


	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}

	if(m_Addr.GetLength() > 2)
	{
		AfxMessageBox("Invalid Entry.  Range: 00-FF");
		return;
	}

	address = hex2dec((char *)(LPCTSTR)m_Addr);	

	DWORD data = ReadSingleBlock(address, &error_code);
	if(error_code)
	{
		AfxMessageBox("ERROR: Invalid data returned.");
		return;
	}

	UpdateData(TRUE);
	m_Data.Format("%08.8X", data);
	UpdateData(FALSE);
	UpdateWindow();
}



//***********************************************************************************
void CRfid1demoDlg::OnButtonStayQuiet() 
{
	unsigned char rx[1000], tx[1000], strr[300];
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;
	int nIndex, n;

	CString str, str2;
	KillTimer(100);


	SendRegwrtreq(1);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();


	//int i = m_SniffList.GetCount();
	nIndex = m_SniffList.GetCurSel();
	if(nIndex < 0)
	{
		AfxMessageBox("No tag selected.");
		return;
	}

    n = m_SniffList.GetTextLen( nIndex );
    m_SniffList.GetText( nIndex, str.GetBuffer(n) );
    str.ReleaseBuffer();
    str2.Format(_T("%s"), str.GetBuffer(0));//save the selected string
	str2.Delete(0,4);//chop off the header

//  E007 0000 23F4 6572
//	7265 F423 0000 07E0 **

//	E007 0000 2404 06AC 
//	0112000304 182002 AC060424000007E0 0000

	//and write it

	sprintf((char *)strr, "%s", (LPCTSTR)str2);
	int b0 = strr[0];
	int b1 = strr[1];
	int b2 = strr[2];
	int b3 = strr[3];
	int b4 = strr[4];
	int b5 = strr[5];
	int b6 = strr[6];
	int b7 = strr[7];
	int b8 = strr[8];
	int b9 = strr[9];
	int b10 = strr[10];
	int b11 = strr[11];
	int b12 = strr[12];
	int b13 = strr[13];
	int b14 = strr[14];
	int b15 = strr[15];


	//build and send the Read block command
	int pos=0;
//	0112000304 182002 AC060424000007E0 0000
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x32;//2
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4

	tx[pos++] = 0x31;//1
	tx[pos++] = 0x38;//8		
	tx[pos++] = 0x32;//2
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x32;//2
	
	tx[pos++] = b14;//data
	tx[pos++] = b15;//data
	tx[pos++] = b12;//data
	tx[pos++] = b13;//data
	tx[pos++] = b10;//data
	tx[pos++] = b11;//data
	tx[pos++] = b8;//data
	tx[pos++] = b9;//data
	tx[pos++] = b6;//data
	tx[pos++] = b7;//data
	tx[pos++] = b4;//data
	tx[pos++] = b5;//data
	tx[pos++] = b2;//data
	tx[pos++] = b3;//data
	tx[pos++] = b0;//data
	tx[pos++] = b1;//data

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	Sleep(50);
	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);

}




//***********************************************************************************
void CRfid1demoDlg::OnButtonReset() 
{
	unsigned char rx[1000], tx[1000];
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;

	//build and send the disable command
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x39;//9
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	tx[pos++] = 0x46;//f
	tx[pos++] = 0x46;//f
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	Sleep(50);
	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);

//	Sleep(110);//wait for micro to start up

	//set DTR
	FT_SetDtr(m_ftHandle);
	Sleep(20); //wait for cap to charge
	//Clear DTR
	FT_ClrDtr(m_ftHandle);
	Sleep(110);//wait for micro to start up

	int ret = PingReader();
	if(ret)
		AfxMessageBox("Ping failed.");

}




//**********************************************************************
CString CRfid1demoDlg::ReadBothReturnOne() 
{
	DWORD BytesWritten;
	char str_src[3000];
	char str_dest[3000];
	unsigned char tx[3000];
	DWORD ret_bytes, x;
	CString stra, str;
	CString sss;
	int TotNumTags;

	SendRegwrtreq(1);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();

	int ret = PurgePort();

	//build and send the Register Write Request packet
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x42;//B

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x34;//4
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x36;//6
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	
	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	ret_bytes = GetBufferSlow();

	m_SniffList.ResetContent();
	UpdateWindow();
	TotNumTags=0;

	for(x=0; x<ret_bytes; x++)
	{
		if(grx[x] == '[')
		{
			x++;
			str.Format(" ");
			int indx=0;
			while(grx[x] != ',')
			{
				stra.Format("%c", grx[x]);
				x++;
				indx++;
				str+=stra;
				if(indx>35)
				{
					str.Format(" ");
					break;
				}
			}
		
			if(str.GetLength() >6)//if a UID was found...
			{
				strcpy(str_src, (LPCTSTR)str);

				//byte reverse
				int ptr=15;
				for(int t=0; t<16; t+=2)
				{
					str_dest[t] = str_src[ptr];
					str_dest[t+1] = str_src[ptr+1];
					str_dest[t+2] = NULL;
					ptr-=2;
				}

				sss.Format("%s", str_dest);
				return sss;
			}
		}
	}


	ret = PurgePort();

	//*********************************************
	SendRegwrtreqTagit();
	SendAgcToggle();
	SendAmPmToggle();

	ret = PurgePort();

	//build and send the Register Write Request packet
	pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x42;//B

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x33;//3
	tx[pos++] = 0x34;//4
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x35;//5
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	ret_bytes = GetBufferSlow();

	for(x=0; x<ret_bytes; x++)
	{
		if(grx[x] == '[')
		{
			x++;
			str.Format(" ");
			int indx=0;
			while(grx[x] != ',')
			{
				stra.Format("%c", grx[x]);
				x++;
				indx++;
				str+=stra;
				if(indx>35)
				{
					str.Format(" ");
					break;
				}
			}
		
			if(str.GetLength() >6)//if a UID was found...
			{
				strcpy(str_src, (LPCTSTR)str);

				//byte reverse
				int ptr=19;
				for(int t=0; t<20; t+=2)
				{
					str_dest[t] = str_src[ptr];
					str_dest[t+1] = str_src[ptr+1];
					str_dest[t+2] = NULL;
					ptr-=2;
				}

				sss.Format("%s", str_dest);
				return sss;
			}
		}
	}

	Sleep(40);//wait for any stragglers

	ret = PurgePort();

	sss.Format(" ");
	return sss;
}







//***********************************************************************************
void CRfid1demoDlg::SilenceTag(CString str2) 
{
	DWORD BytesWritten;
	unsigned char tx[1000], strr[300];

	SendRegwrtreq(1);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();


	sprintf((char *)strr, "%s", (LPCTSTR)str2);
	int b0 = strr[0];
	int b1 = strr[1];
	int b2 = strr[2];
	int b3 = strr[3];
	int b4 = strr[4];
	int b5 = strr[5];
	int b6 = strr[6];
	int b7 = strr[7];
	int b8 = strr[8];
	int b9 = strr[9];
	int b10 = strr[10];
	int b11 = strr[11];
	int b12 = strr[12];
	int b13 = strr[13];
	int b14 = strr[14];
	int b15 = strr[15];


	//build and send the command
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x32;//2
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4

	tx[pos++] = 0x31;//1
	tx[pos++] = 0x38;//8		
	tx[pos++] = 0x32;//2
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x32;//2
	
	tx[pos++] = b14;//data
	tx[pos++] = b15;//data
	tx[pos++] = b12;//data
	tx[pos++] = b13;//data
	tx[pos++] = b10;//data
	tx[pos++] = b11;//data
	tx[pos++] = b8;//data
	tx[pos++] = b9;//data
	tx[pos++] = b6;//data
	tx[pos++] = b7;//data
	tx[pos++] = b4;//data
	tx[pos++] = b5;//data
	tx[pos++] = b2;//data
	tx[pos++] = b3;//data
	tx[pos++] = b0;//data
	tx[pos++] = b1;//data

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	int ret = FastRead(63, 50);

}





//**********************************************************************
int CRfid1demoDlg::ReadAndSaveTags() 
{
	DWORD BytesWritten;
	char str_src[3000];
	char str_dest[3000];
	unsigned char tx[3000];
	DWORD ret_bytes, x;
	CString stra, str;
	int TotNumTags;




	SendRegwrtreq(1);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();

	int ret = PurgePort();

	//build and send the Register Write Request packet
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x42;//B

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x34;//4
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x36;//6
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	
	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	ret_bytes = GetBufferSlow();

	TotNumTags=0;

	for(x=0; x<ret_bytes; x++)
	{
		if(grx[x] == '[')
		{
			x++;
			str.Format(" ");
			int indx=0;
			while(grx[x] != ',')
			{
				stra.Format("%c", grx[x]);
				x++;
				indx++;
				str+=stra;
				if(indx>35)
				{
					str.Format(" ");
					break;
				}
			}
		
			if(str.GetLength() >6)//if a UID was found...
			{
				strcpy(str_src, (LPCTSTR)str);

				//byte reverse
				int ptr=15;
				for(int t=0; t<16; t+=2)
				{
					str_dest[t] = str_src[ptr];
					str_dest[t+1] = str_src[ptr+1];
					str_dest[t+2] = NULL;
					ptr-=2;
				}

				CString sss;
				sss.Format("%s", str_dest);
				
				//save to array
				g_str_array[TotNumTags] = sss;
				TotNumTags++;
			}
		}
	}


	ret = PurgePort();

	//*********************************************
	SendRegwrtreqTagit();
	SendAgcToggle();
	SendAmPmToggle();

	ret = PurgePort();

	//build and send the Register Write Request packet
	pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x42;//B

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x33;//3
	tx[pos++] = 0x34;//4
		
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x35;//5
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	ret_bytes = GetBufferSlow();

	for(x=0; x<ret_bytes; x++)
	{
		if(grx[x] == '[')
		{
			x++;
			str.Format(" ");
			int indx=0;
			while(grx[x] != ',')
			{
				stra.Format("%c", grx[x]);
				x++;
				indx++;
				str+=stra;
				if(indx>35)
				{
					str.Format(" ");
					break;
				}
			}
		
			if(str.GetLength() >6)//if a UID was found...
			{
				strcpy(str_src, (LPCTSTR)str);

				//byte reverse
				int ptr=19;
				for(int t=0; t<20; t+=2)
				{
					str_dest[t] = str_src[ptr];
					str_dest[t+1] = str_src[ptr+1];
					str_dest[t+2] = NULL;
					ptr-=2;
				}

				CString sss;
				sss.Format("%s", str_dest);

				//save to array
				g_str_array[TotNumTags] = sss;

				TotNumTags++;				
			}
		}
	}

	Sleep(40);//wait for any stragglers

	ret = PurgePort();

	return TotNumTags;
}




//****************************************************************************************
void CRfid1demoDlg::OnButtonReadBatch() 
{
	CString str;
	int count;

	UpdateData(TRUE);
	m_BFound.Format(" ");
	UpdateData(FALSE);
	UpdateWindow();
	count=0;

	m_SniffList.ResetContent();
	UpdateWindow();


	BatchRunning=1;
	while(BatchRunning)
	{
		int numtags = ReadAndSaveTags();

		for(int i=0; i<numtags; i++)
		{
			SilenceTag(g_str_array[i]);

			m_SniffList.InsertString(0,g_str_array[i]);
			UpdateWindow();
		}

		UpdateData(TRUE);
		count+=numtags;
		m_BFound.Format("%d found", count);
		UpdateData(FALSE);
		UpdateWindow();

		//Sleep(1);
		//message pump
		MSG message;
		if(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) 
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}

		UpdateData(TRUE);
		m_PassCount++;
		UpdateData(FALSE);
		UpdateWindow();
	}


}



//****************************************************************************************
void CRfid1demoDlg::OnButtonStopbatch() 
{
	BatchRunning=0;
}

//0x30 = 0
//0x39 = 9
//0x41 = A
//0x42 = B
//0x43 = C
//0x44 = D
//0x45 = E
//0x46 = F



//****************************************************************************************
void CRfid1demoDlg::OnButtonReadStatus() 
{
	char str_src[3000];
	char str_dest[3000];
	unsigned char tx[100], rx[400];
	CString stra, str;
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;


	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}


	UpdateData(TRUE);
	m_UID.Format(" ");
	m_DSFID.Format(" ");
	m_AFI.Format(" ");
	m_TagType.Format(" ");
	m_MemSize.Format(" ");
	UpdateData(FALSE);
	UpdateWindow();

	m_SniffList.ResetContent();
	UpdateWindow();

	//determine the type of tag
	CString single = ReadBothReturnOne();
	if(single.GetLength() < 5)
	{
		//AfxMessageBox("Tag not found.");
		m_SniffList.InsertString(0, "Tag not foud.");
		UpdateWindow();
		return;
	}

	m_SniffList.InsertString(0, single);
	UpdateWindow();

	CString type;
	type = single.Mid(4, 2);
	int dtype = hex2dec((char *)(LPCTSTR)type);	
	dtype &= 0xFE; //clear the lsb
	
	UpdateData(TRUE);
	if(dtype == 0x00)//HF-I Plus Inlay
		m_TagType.Format("HF-I Plus Inlay");
	if(dtype == 0x80)//HF-I Plus Chip
		m_TagType.Format("HF-I Plus Chip");
	if(dtype == 0xC0)//Standard Chip / Inlay
		m_TagType.Format("Standard Chip / Inlay");
	if(dtype == 0xC4)//Pro Chip / Inlay
		m_TagType.Format("Pro Chip / Inlay");
	UpdateData(FALSE);
	UpdateWindow();


	if( (dtype == 0xC4) || (dtype == 0xC0) )//HF-I Standard/Pro
	{

		UpdateData(TRUE);

		m_UID = single;
		
		int error;
		DWORD data = ReadSingleBlock(0x0A, &error); //read AFI
		if(error)
			m_AFI.Format("ERROR");
		   else	
		   {
			   data &= 0xFF;
				m_AFI.Format("%02.2X", data);
		   }


		m_MemSize.Format("N/A");
		m_DSFID.Format("N/A");
		UpdateData(FALSE);
		UpdateWindow();
	}


	if( (dtype == 0x00) || (dtype == 0x80) )//HF-I Plus Inlay/chip
	{
		SendRegwrtreq(0);//0=usermem, 1=UID  //15693 setup
		SendAgcToggle();
		SendAmPmToggle();
		//Request system info: 010A00030418002B0000
		//build and send the Read block command
		int pos=0;
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x31;//1
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x41;//A

		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x33;//3
		
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x34;//4
		tx[pos++] = 0x31;//1
		tx[pos++] = 0x38;//8
			
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0

		tx[pos++] = 0x32;//2
		tx[pos++] = 0x42;//B
		
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
		Sleep(100);
		FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
		FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);

		str.Format("%s", rx);
		if(str.GetLength() > 5)
		{
			pos = str.Find("[", 0);
			str = str.Mid(pos+1);
			pos = str.Find("]", 0);
			str = str.Left(pos);
			//m_SniffList.InsertString(0, str);
			UpdateWindow();

			str = str.Mid(4);

			UpdateData(TRUE);
			//Display UID
			m_UID = str.Left(16);
			strcpy(str_src, (LPCTSTR)m_UID);
			//byte reverse
			int ptr=14;
			for(int t=0; t<16; t+=2)
			{
				str_dest[t] = str_src[ptr];
				str_dest[t+1] = str_src[ptr+1];
				str_dest[t+2] = NULL;
				ptr-=2;
			}
			m_UID.Format("%s", str_dest);

			//display DSFID & AFI
			m_DSFID.Format("%s", str.Mid(16, 2) );
			m_AFI.Format("%s", str.Mid(18, 2) );
			m_MemSize.Format("%s", str.Mid(20, 4) );
			UpdateData(FALSE);
			UpdateWindow();
		}
	}

}


//0x30 = 0
//0x39 = 9
//0x41 = A
//0x42 = B
//0x43 = C
//0x44 = D
//0x45 = E
//0x46 = F


//****************************************************************************************
void CRfid1demoDlg::OnButtonLockBlock() 
{
	unsigned char tx[100], rx[1000], strr[30];
	CString stra, str;
	int address;
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;


	
	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}

	SendRegwrtreq(0);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();

	UpdateData(TRUE);
	if(m_Addr.GetLength() > 2)
	{
		AfxMessageBox("Invalid Entry.  Range: 00-FF");
		return;
	}

	address = hex2dec((char *)(LPCTSTR)m_Addr);	
	sprintf((char *)strr, "%02.2X", address);
	int b1 = strr[0];
	int b2 = strr[1];


	//build and send the Lock block command
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x42;//B

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x38;//8
		
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x32;//2
	tx[pos++] = 0x32;//2
	tx[pos++] = b1;//addr
	tx[pos++] = b2;//addr

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	Sleep(50);
	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);	
}


//****************************************************************************************
void CRfid1demoDlg::OnButtonDsfuidWrite() 
{
	unsigned char tx[100], rx[1000], strr[30];
	CString stra, str;
	int data;
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;
	
	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}

	SendRegwrtreq(0);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();

	UpdateData(TRUE);
	if(m_Dsfid2Write.GetLength() > 2)
	{
		AfxMessageBox("Invalid Entry.  Range: 00-FF");
		return;
	}

	data= hex2dec((char *)(LPCTSTR)m_Dsfid2Write);	
	sprintf((char *)strr, "%02.2X", data);
	int b1 = strr[0];
	int b2 = strr[1];


	//build and send the DSFID Write command
	//010B000304184029670000
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x42;//B

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x38;//8
		
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x32;//2
	tx[pos++] = 0x39;//9
	tx[pos++] = b1;//data
	tx[pos++] = b2;//data
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	Sleep(50);
	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);
}





//****************************************************************************************
void CRfid1demoDlg::OnButtonAfiWrite() 
{
	unsigned char tx[100], rx[1000], strr[30];
	CString stra, str;
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;

	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}

	UpdateData(TRUE);
	m_UID.Format(" ");
	m_DSFID.Format(" ");
	m_AFI.Format(" ");
	m_TagType.Format(" ");
	m_MemSize.Format(" ");
	UpdateData(FALSE);
	UpdateWindow();


	m_SniffList.ResetContent();
	UpdateWindow();

	//determine the type of tag
	CString single = ReadBothReturnOne();
	if(single.GetLength() < 5)
	{
		m_SniffList.InsertString(0, "Tag not foud.");
		UpdateWindow();
		return;
	}

	m_SniffList.InsertString(0, single);
	UpdateWindow();

	CString type;
	type = single.Mid(4, 2);
	int dtype = hex2dec((char *)(LPCTSTR)type);	
	dtype &= 0xFE; //clear the lsb
	
	UpdateData(TRUE);
	if(dtype == 0x00)//HF-I Plus Inlay
		m_TagType.Format("HF-I Plus Inlay");
	if(dtype == 0x80)//HF-I Plus Chip
		m_TagType.Format("HF-I Plus Chip");
	if(dtype == 0xC0)//Standard Chip / Inlay
		m_TagType.Format("Standard Chip / Inlay");
	if(dtype == 0xC4)//Pro Chip / Inlay
		m_TagType.Format("Pro Chip / Inlay");
	UpdateData(FALSE);
	UpdateWindow();

	//m_AFI2Write

	UpdateData(TRUE);
	if(m_AFI2Write.GetLength() > 2)
	{
		AfxMessageBox("Invalid Entry.  Range: 00-FF");
		return;
	}
	int data =  hex2dec((char *)(LPCTSTR)m_AFI2Write);	


	if( (dtype == 0xC4) || (dtype == 0xC0) )//HF-I Standard/Pro
	{
		WriteSingleBlock(0x0A, data);
	}

	if( (dtype == 0x00) || (dtype == 0x80) )//HF-I Plus Inlay/chip
	{
		sprintf((char *)strr, "%02.2X", data);
		int b1 = strr[0];
		int b2 = strr[1];

		SendRegwrtreq(0);//0=usermem, 1=UID  //15693 setup
		SendAgcToggle();
		SendAmPmToggle();
		//write AFI of 56: 010B 000304 184027 560000
		//build and send the Write command
		int pos=0;
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x31;//1
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x42;//B

		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x33;//3
		
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x34;//4
		tx[pos++] = 0x31;//1
		tx[pos++] = 0x38;//8
			
		tx[pos++] = 0x34;//4
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x32;//2
		tx[pos++] = 0x37;//7

		tx[pos++] = b1;//data
		tx[pos++] = b2;//data
		
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
		Sleep(100);
		FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
		FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);
	}



}




//****************************************************************************************
void CRfid1demoDlg::OnButtonDsfidLock() 
{
	unsigned char tx[100], rx[1000];
	CString stra, str;

	DWORD BytesWritten, AmountInRxQueue, BytesReturned;
	
	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}

	SendRegwrtreq(0);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();

	//build and send the DSFID Lock command
	//010A00030418402A0000
	int pos=0;
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x31;//1
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x41;//A

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	tx[pos++] = 0x30;//0
	tx[pos++] = 0x33;//3
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x34;//4

	tx[pos++] = 0x31;//1
	tx[pos++] = 0x38;//8
	tx[pos++] = 0x34;//4
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x32;//2
	tx[pos++] = 0x41;//A
	
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0
	tx[pos++] = 0x30;//0

	FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
	Sleep(50);
	FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
	FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);
	
}
//0x30 = 0
//0x39 = 9
//0x41 = A
//0x42 = B
//0x43 = C
//0x44 = D
//0x45 = E
//0x46 = F
//****************************************************************************************
void CRfid1demoDlg::OnButtonAfiLock() 
{
	unsigned char tx[100], rx[1000], strr[30];
	CString stra, str;
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;

	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}

	UpdateData(TRUE);
	m_UID.Format(" ");
	m_DSFID.Format(" ");
	m_AFI.Format(" ");
	m_TagType.Format(" ");
	m_MemSize.Format(" ");
	UpdateData(FALSE);
	UpdateWindow();


	m_SniffList.ResetContent();
	UpdateWindow();

	//determine the type of tag
	CString single = ReadBothReturnOne();
	if(single.GetLength() < 5)
	{
		m_SniffList.InsertString(0, "Tag not foud.");
		UpdateWindow();
		return;
	}

	m_SniffList.InsertString(0, single);
	UpdateWindow();

	CString type;
	type = single.Mid(4, 2);
	int dtype = hex2dec((char *)(LPCTSTR)type);	
	dtype &= 0xFE; //clear the lsb
	
	UpdateData(TRUE);
	if(dtype == 0x00)//HF-I Plus Inlay
		m_TagType.Format("HF-I Plus Inlay");
	if(dtype == 0x80)//HF-I Plus Chip
		m_TagType.Format("HF-I Plus Chip");
	if(dtype == 0xC0)//Standard Chip / Inlay
		m_TagType.Format("Standard Chip / Inlay");
	if(dtype == 0xC4)//Pro Chip / Inlay
		m_TagType.Format("Pro Chip / Inlay");
	UpdateData(FALSE);
	UpdateWindow();




	if( (dtype == 0xC4) || (dtype == 0xC0) )//HF-I Standard/Pro
	{
		int address = 0x0A;	
		sprintf((char *)strr, "%02.2X", address);
		int b1 = strr[0];
		int b2 = strr[1];


		//build and send the Lock block command
		int pos=0;
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x31;//1
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x42;//B

		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x33;//3
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x34;//4

		tx[pos++] = 0x31;//1
		tx[pos++] = 0x38;//8
		tx[pos++] = 0x34;//4
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x32;//2
		tx[pos++] = 0x32;//2

		tx[pos++] = b1;//addr
		tx[pos++] = b2;//addr

		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0

		FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
		Sleep(50);
		FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
		FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);	
	}

	if( (dtype == 0x00) || (dtype == 0x80) )//HF-I Plus Inlay/chip
	{
		SendRegwrtreq(0);//0=usermem, 1=UID  //15693 setup
		SendAgcToggle();
		SendAmPmToggle();
		//write AFI lock: 010A 00 0304 184028 0000
		int pos=0;
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x31;//1
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x41;//A

		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x33;//3
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x34;//4

		tx[pos++] = 0x31;//1
		tx[pos++] = 0x38;//8
		tx[pos++] = 0x34;//4
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x32;//2
		tx[pos++] = 0x38;//8
		
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		tx[pos++] = 0x30;//0
		FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
		Sleep(100);
		FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
		FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);
	}


}





//****************************************************************************************
void CRfid1demoDlg::OnButtonReadLockstate() 
{
	CString stra, str, str2;
	unsigned char tx[100], rx[1000];
	DWORD BytesWritten, AmountInRxQueue, BytesReturned;

	m_SniffList.ResetContent();
	m_SniffList.AddString("Reading...");
	m_SniffList.SetCurSel(0);
	UpdateWindow();


	if(valid_port==0)
	{
		KillTimer(100);
		AfxMessageBox("RFID1 Reader not found");
		return;
	}

	//determine the type of tag
	CString single = ReadBothReturnOne();
	if(single.GetLength() < 5)
	{
		//AfxMessageBox("Tag not found.");
		m_SniffList.InsertString(0, "Tag not foud.");
		UpdateWindow();
		return;
	}


	CString type;
	type = single.Mid(4, 2);
	int dtype = hex2dec((char *)(LPCTSTR)type);	
	dtype &= 0xFE; //clear the lsb

	
	UpdateData(TRUE);
	if(dtype == 0x00)//HF-I Plus Inlay
		m_TagType.Format("HF-I Plus Inlay");
	if(dtype == 0x80)//HF-I Plus Chip
		m_TagType.Format("HF-I Plus Chip");
	if(dtype == 0xC0)//Standard Chip / Inlay
		m_TagType.Format("Standard Chip / Inlay");
	if(dtype == 0xC4)//Pro Chip / Inlay
		m_TagType.Format("Pro Chip / Inlay");
	UpdateData(FALSE);
	UpdateWindow();

	PurgePort();

	SendRegwrtreq(0);//0=usermem, 1=UID  //15693 setup
	SendAgcToggle();
	SendAmPmToggle();


	if( (dtype == 0x00) || (dtype == 0x80) )//HF-I Plus Inlay/chip
	{
		//010C 000304 18002C 00 3F 0000
		m_SniffList.ResetContent();
		int index=0;
		for(int loop=0; loop<8; loop++)
		{
			int pos=0;
			tx[pos++] = 0x30;//0
			tx[pos++] = 0x31;//1
			tx[pos++] = 0x30;//0
			tx[pos++] = 0x43;//C

			tx[pos++] = 0x30;//0
			tx[pos++] = 0x30;//0
			tx[pos++] = 0x30;//0
			tx[pos++] = 0x33;//3
			tx[pos++] = 0x30;//0
			tx[pos++] = 0x34;//4

			tx[pos++] = 0x31;//1
			tx[pos++] = 0x38;//8
			tx[pos++] = 0x30;//0
			tx[pos++] = 0x30;//0
			tx[pos++] = 0x32;//2
			tx[pos++] = 0x43;//C

			//add start here...
			if(loop==0)
			{
				tx[pos++] = 0x30;//0
				tx[pos++] = 0x30;//0
			}
			if(loop==1)
			{
				tx[pos++] = 0x30;//0
				tx[pos++] = 0x38;//8
			}
			if(loop==2)
			{
				tx[pos++] = 0x31;//1
				tx[pos++] = 0x30;//0
			}
			if(loop==3)
			{
				tx[pos++] = 0x31;//1
				tx[pos++] = 0x38;//8
			}
			if(loop==4)
			{
				tx[pos++] = 0x32;//2
				tx[pos++] = 0x30;//0
			}
			if(loop==5)
			{
				tx[pos++] = 0x32;//2
				tx[pos++] = 0x38;//8
			}
			if(loop==6)
			{
				tx[pos++] = 0x33;//3
				tx[pos++] = 0x30;//0
			}
			if(loop==7)
			{
				tx[pos++] = 0x33;//3
				tx[pos++] = 0x38;//8
			}

			
			//add end here...
			tx[pos++] = 0x30;//0
			tx[pos++] = 0x37;//7

			tx[pos++] = 0x30;//0
			tx[pos++] = 0x30;//0
			tx[pos++] = 0x30;//0
			tx[pos++] = 0x30;//0

			FT_Write(m_ftHandle, tx, pos, &BytesWritten); 
			Sleep(150);
			FT_GetQueueStatus(m_ftHandle, &AmountInRxQueue);
			FT_Read(m_ftHandle, rx, AmountInRxQueue, &BytesReturned);


			for(DWORD x=0; x<BytesReturned; x++)
			{
				stra.Format("%c", rx[x]);
				str+=stra;
			}
			pos = str.Find("[", 0);
			str = str.Mid(pos+3);
				
			//load m_LockRead with data
			for(x=0; x<8; x++)
			{
				str2 = str.Left(2);		

				if(str2.Find("01", 0) >= 0)
					stra.Format("%02.2X: Locked", index);
				else
					stra.Format("%02.2X: Unlocked", index);
				m_SniffList.AddString(stra);

				str = str.Mid(2);
				index++;
			}
			UpdateWindow();


		}

		m_SniffList.SetCurSel(0);
	}
   else//Pro/Standard Read (one block at a time)
   {
		AfxMessageBox("Only HF-I Plus tags support this function.");
		return;
   }
	
}
