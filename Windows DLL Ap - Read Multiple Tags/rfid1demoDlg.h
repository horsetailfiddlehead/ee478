// rfid1demoDlg.h : header file
#if !defined(AFX_RFID1DEMODLG_H__4272D17D_50C3_48CB_9F61_7993F94F94F9__INCLUDED_)
#define AFX_RFID1DEMODLG_H__4272D17D_50C3_48CB_9F61_7993F94F94F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




// CRfid1demoDlg dialog
class CRfid1demoDlg : public CDialog
{
// Construction
public:
	CRfid1demoDlg(CWnd* pParent = NULL);	// standard constructor
	int PurgePort();

	DWORD hex2dec(char *addr);
	DWORD GetBufferSlow();
	DWORD FastRead(int count, int timeout);

	void OnButton15693invReq(); 
	void CRfid1demoDlg::SendRegwrtreq(int mode);//0=usermem, 1=UID //15693 Setup
	void SendRegwrtreqTagit();
	void SidpollReq();
	void SendAgcToggle();
	void SendAmPmToggle(); 
	void ReadBoth();
	int  OpenPort(int reset, int beep, int notify_open);
	int  PingReader(); 
	void SendBeep1();
	void Read8Blocks(int section);
	CString ReadBothReturnOne(); 
	void SilenceTag(CString str2); 
	int ReadAndSaveTags(); 
	DWORD ReadSingleBlock(int addr, int *error);
	void WriteSingleBlock(DWORD address, int data); 






// Dialog Data
	//{{AFX_DATA(CRfid1demoDlg)
	enum { IDD = IDD_RFID1DEMO_DIALOG };
	CProgressCtrl	m_Progress;
	CListBox	m_SniffList;
	CString	m_NumTags;
	CString	m_MyText;
	CString	m_PortStatus;
	int		m_NumChars;
	CString	m_Addr;
	CString	m_Data;
	CString	m_BFound;
	int		m_PassCount;
	CString	m_UID;
	CString	m_DSFID;
	CString	m_AFI;
	CString	m_TagType;
	CString	m_MemSize;
	CString	m_Dsfid2Write;
	CString	m_AFI2Write;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRfid1demoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRfid1demoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonread();
	afx_msg void OnChangeEditMytext();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonWriteTag();
	afx_msg void OnButtonTestPattern();
	afx_msg void OnButtonPassBeep();
	afx_msg void OnButtonFailBeep();
	afx_msg void OnButtonWriteSingle();
	afx_msg void OnButtonReadBlock();
	afx_msg void OnButtonStayQuiet();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonReadBatch();
	afx_msg void OnButtonStopbatch();
	afx_msg void OnButtonReadStatus();
	afx_msg void OnButtonLockBlock();
	afx_msg void OnButtonDsfuidWrite();
	afx_msg void OnButtonAfiWrite();
	afx_msg void OnButtonDsfidLock();
	afx_msg void OnButtonAfiLock();
	afx_msg void OnButtonReadLockstate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RFID1DEMODLG_H__4272D17D_50C3_48CB_9F61_7993F94F94F9__INCLUDED_)
