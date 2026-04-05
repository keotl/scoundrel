// ScoundrelDlg.cpp : implementation file
//

#include "Card.h"
#include "afxwin.h"
#include "stdafx.h"
#include "Scoundrel.h"
#include "ScoundrelDlg.h"
#include "DrawUtils.h"
#include "wingdi.h"

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
	ON_WM_LBUTTONUP()
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
	srand((unsigned int)GetTickCount());

	CRect clientRect;
	GetClientRect(&clientRect);
	CClientDC dc(this);
	this->drawUtils.Init(&dc, clientRect);
	this->game.InitializeNewGame();
	this->draggingCardIndex = -1;
	this->backgroundBitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());
	this->backgroundDc.CreateCompatibleDC(&dc);
	this->backgroundDc.SelectObject(&backgroundBitmap);
	this->foregroundBitmap.CreateCompatibleBitmap(&dc, drawUtils.cardSize.cx, drawUtils.cardSize.cy);
	this->foregroundDc.CreateCompatibleDC(&dc);
	this->foregroundDc.SelectObject(&foregroundBitmap);
	this->fbBitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());
	this->fbDc.CreateCompatibleDC(&dc);
	this->fbDc.SelectObject(&fbBitmap);

	return TRUE; // return TRUE  unless you set the focus to a control
}

void CScoundrelDlg::PaintForegroundDC()
{
	if (draggingCardIndex != -1 && game.room[draggingCardIndex] != NULL)
	{
		drawUtils.DrawCardAtPoint(CPoint(0, 0), game.room[draggingCardIndex], &foregroundDc);
	}
}

void CScoundrelDlg::PaintInMemoryDCs()
{
	drawUtils.DrawGameState(&backgroundDc, game, draggingCardIndex);
	PaintForegroundDC();
}

void CScoundrelDlg::BlitInMemoryDCs(CDC *pDC)
{
	if (draggingCardIndex == -1 || game.room[draggingCardIndex] == NULL)
	{
		// Nothing to render in foreground, blit background directly to dc
		pDC->BitBlt(0, 0, drawUtils.clientRect.Width(), drawUtils.clientRect.Height(), &backgroundDc, 0, 0, SRCCOPY);
		return;
	}

	fbDc.BitBlt(0, 0, drawUtils.clientRect.Width(), drawUtils.clientRect.Height(), &backgroundDc, 0, 0, SRCCOPY);

	fbDc.BitBlt(dragX - dragMouseIntrinsicOffsetX, dragY - dragMouseIntrinsicOffsetY, drawUtils.cardSize.cx, drawUtils.cardSize.cy, &foregroundDc, 0, 0, SRCCOPY);

	pDC->BitBlt(0, 0, drawUtils.clientRect.Width(), drawUtils.clientRect.Height(), &fbDc, 0, 0, SRCCOPY);
}

void CScoundrelDlg::OnPaint()
{
	CPaintDC dc(this);
	PaintInMemoryDCs();
	BlitInMemoryDCs(&dc);

	// Do not call CDialog::OnPaint() for painting messages
}

void CScoundrelDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	int roomCardIndex = drawUtils.GetRoomCardIndexAtPoint(point);
	if (roomCardIndex != -1 && game.room[roomCardIndex] != NULL)
	{
		CRect cardRect = drawUtils.GetRoomCardRect(roomCardIndex);
		draggingCardIndex = roomCardIndex;
		dragX = point.x;
		dragY = point.y;
		dragMouseIntrinsicOffsetX = point.x - cardRect.TopLeft().x;
		dragMouseIntrinsicOffsetY = point.y - cardRect.TopLeft().y;

		CClientDC dc(this);
		drawUtils.TransferRoomCard(&backgroundDc, &foregroundDc, roomCardIndex);
		BlitInMemoryDCs(&dc);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CScoundrelDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (draggingCardIndex != -1)
	{
		int newX = point.x / 4 * 4;
		int newY = point.y / 4 * 4;
		if (newX == dragX && newY == dragY)
			return;

		dragX = newX;
		dragY = newY;
		CClientDC dc(this);
		BlitInMemoryDCs(&dc);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CScoundrelDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO -   - keotl 2026-04-05
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CScoundrelDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	draggingCardIndex = -1;
	CClientDC dc(this);
	PaintInMemoryDCs();
	BlitInMemoryDCs(&dc);
	// TODO - trigger action based on drop-off point  - keotl 2026-04-05

	CDialog::OnLButtonUp(nFlags, point);
}
