// ScoundrelDlg.h : header file
//

#if !defined(AFX_SCOUNDRELDLG_H__798E72F5_DBC5_44A6_AA4E_0CA6F9C4F130__INCLUDED_)
#define AFX_SCOUNDRELDLG_H__798E72F5_DBC5_44A6_AA4E_0CA6F9C4F130__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CScoundrelDlg dialog
#include "DrawUtils.h"
#include "GameState.h"

class CScoundrelDlg : public CDialog
{
// Construction
public:
	CScoundrelDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CScoundrelDlg)
	enum { IDD = IDD_SCOUNDREL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScoundrelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CScoundrelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	DrawUtils drawUtils;
	GameState game;
	int draggingCardIndex;
	int dragX;
	int dragY;
	CBitmap backgroundBitmap;
	CDC backgroundDc; // Everything, except card being dragged
	CBitmap foregroundBitmap;
	CDC foregroundDc; // Card being dragged around
	void PaintInMemoryDCs();
	void BlitInMemoryDCs(CDC *pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCOUNDRELDLG_H__798E72F5_DBC5_44A6_AA4E_0CA6F9C4F130__INCLUDED_)
