// rfid1demo.h : main header file for the RFID1DEMO application
//

#if !defined(AFX_RFID1DEMO_H__0E59BD5D_1E59_450F_9E35_473223B5F035__INCLUDED_)
#define AFX_RFID1DEMO_H__0E59BD5D_1E59_450F_9E35_473223B5F035__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRfid1demoApp:
// See rfid1demo.cpp for the implementation of this class
//

class CRfid1demoApp : public CWinApp
{
public:
	CRfid1demoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRfid1demoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRfid1demoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RFID1DEMO_H__0E59BD5D_1E59_450F_9E35_473223B5F035__INCLUDED_)
