// ScoundrelDlg.cpp : implementation file
//

#include "Card.h"
#include "afxwin.h"
#include "stdafx.h"
#include "Scoundrel.h"
#include "ScoundrelDlg.h"
#include "DrawUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScoundrelDlg dialog

CScoundrelDlg::CScoundrelDlg(CWnd *pParent /*=NULL*/)
	: CDialog(CScoundrelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScoundrelDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScoundrelDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScoundrelDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScoundrelDlg, CDialog)
	//{{AFX_MSG_MAP(CScoundrelDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScoundrelDlg message handlers

BOOL CScoundrelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);	 // Set big icon
	SetIcon(m_hIcon, FALSE); // Set small icon

	CenterWindow(GetDesktopWindow()); // center to the hpc screen

	// Setup a random seed for rand()
	srand((unsigned int) GetTickCount());

	// TODO: Add extra initialization here
	CClientDC dc(this);
	this->drawUtils.Init(&dc);
	this->game.InitializeNewGame();
	this->draggingCardIndex = -1;

	return TRUE; // return TRUE  unless you set the focus to a control
}

void CScoundrelDlg::OnPaint()
{
	CPaintDC dc(this);
	CRect clientRect;
	GetClientRect(&clientRect);

	drawUtils.DrawGameState(&dc, clientRect, game, draggingCardIndex);

	// Do not call CDialog::OnPaint() for painting messages
}

void CScoundrelDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);

	Card card(3, HEART);
	this->drawUtils.DrawCardAtPoint(point, card, &dc);

	CDialog::OnLButtonDown(nFlags, point);
}

void CScoundrelDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMouseMove(nFlags, point);
}

void CScoundrelDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnLButtonDblClk(nFlags, point);
}
