// Scoundrel.h : main header file for the SCOUNDREL application

//



#if !defined(AFX_SCOUNDREL_H__555422F2_8F7C_495A_87B0_444E1433BFF1__INCLUDED_)

#define AFX_SCOUNDREL_H__555422F2_8F7C_495A_87B0_444E1433BFF1__INCLUDED_



#if _MSC_VER >= 1000

#pragma once

#endif // _MSC_VER >= 1000



#ifndef __AFXWIN_H__

	#error include 'stdafx.h' before including this file for PCH

#endif



#include "resource.h"		// main symbols



/////////////////////////////////////////////////////////////////////////////

// CScoundrelApp:

// See Scoundrel.cpp for the implementation of this class

//



class CScoundrelApp : public CWinApp

{

public:

	CScoundrelApp();



// Overrides

	// ClassWizard generated virtual function overrides

	//{{AFX_VIRTUAL(CScoundrelApp)

	public:

	virtual BOOL InitInstance();

	//}}AFX_VIRTUAL



// Implementation



	//{{AFX_MSG(CScoundrelApp)

	afx_msg void OnPaint();

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};





/////////////////////////////////////////////////////////////////////////////



//{{AFX_INSERT_LOCATION}}

// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_SCOUNDREL_H__555422F2_8F7C_495A_87B0_444E1433BFF1__INCLUDED_)

