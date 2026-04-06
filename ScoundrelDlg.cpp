// ScoundrelDlg.cpp : implementation file
//

#include "Card.h"
#include "GameState.h"
#include "afx.h"
#include "afxwin.h"
#include "stdafx.h"
#include "Scoundrel.h"
#include "ScoundrelDlg.h"
#include "DrawUtils.h"
#include "wingdi.h"
#include "winuser.h"

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
	m_hIconSmall = AfxGetApp()->LoadIcon(IDI_ICON1);
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
	SetIcon(m_hIconSmall, FALSE); // Set small icon

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
	CClientDC dc(this);
	drawUtils.DrawHUD(&backgroundDc, game, "");
	if (drawUtils.IsPointInDeckRegion(point))
	{
		int error = game.RunAway();
		if (error)
		{
			HandleGameError(error);
		}
		else
		{
			drawUtils.RepaintRoom(&backgroundDc, game, -1);
			drawUtils.DrawHUD(&backgroundDc, game, "Ran away!");
		}
	}

	int roomCardIndex = drawUtils.GetRoomCardIndexAtPoint(point);
	if (roomCardIndex != -1 && game.room[roomCardIndex] != NULL)
	{
		CRect cardRect = drawUtils.GetRoomCardRect(roomCardIndex);
		draggingCardIndex = roomCardIndex;
		dragX = point.x;
		dragY = point.y;
		dragMouseIntrinsicOffsetX = point.x - cardRect.TopLeft().x;
		dragMouseIntrinsicOffsetY = point.y - cardRect.TopLeft().y;

		drawUtils.TransferRoomCard(&backgroundDc, &foregroundDc, roomCardIndex);
	}
	BlitInMemoryDCs(&dc);

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
	if (draggingCardIndex != -1)
	{
		CClientDC dc(this);
		BOOL actionSuccessful = true;
		int previousHealth = game.health;
		if (drawUtils.IsPointInArmourRegion(point))
		{
			int error = game.Equip(draggingCardIndex);
			if (error)
			{
				HandleGameError(error);
				actionSuccessful = false;
			}
			else
			{
				drawUtils.TransferForegroundCardToArmourRegion(&backgroundDc, &foregroundDc);
				drawUtils.ClearDurabilityRegion(&backgroundDc);
				drawUtils.DrawHUD(&backgroundDc, game, _T("Equipped weapon!"));
			}
		}
		else if (drawUtils.IsPointInDurabilityRegion(point))
		{
			int error = game.FightWithWeapon(draggingCardIndex);
			if (error)
			{
				HandleGameError(error);
				actionSuccessful = false;
			}
			else
			{
				drawUtils.TransferForegroundCardToDurabilityRegion(&backgroundDc, &foregroundDc, game.foughtByWeapon.GetCount() - 1);

				CString msg;
				msg.Format(_T( "Fought with weapon! -%d HP" ), previousHealth - game.health);
				drawUtils.DrawHUD(&backgroundDc, game, msg);
			}
		}
		else if (drawUtils.IsPointInUsePlaceholderRegion(point))
		{
			Card *card = game.room[draggingCardIndex];
			if (card != NULL && (card->suit == SPADE || card->suit == CLUB))
			{
				int error = game.FightBarehanded(draggingCardIndex);
				if (error)
				{
					HandleGameError(error);
					actionSuccessful = false;
				}
				else
				{
					CString msg;
					msg.Format(_T( "Fought barehanded! -%d HP" ), previousHealth - game.health);
					drawUtils.DrawHUD(&backgroundDc, game, msg);
				}
			}
			else if (card != NULL && (card->suit == HEART))
			{
				int error = game.DrinkPotion(draggingCardIndex);
				if (error)
				{
					HandleGameError(error);
					actionSuccessful = false;
				}
				else
				{
					CString msg;
					msg.Format(_T( "Drank potion! +%d HP" ), game.health - previousHealth);
					drawUtils.DrawHUD(&backgroundDc, game, msg);
				}
			}
			else
			{
				actionSuccessful = false;
			}
		}
		else
		{
			actionSuccessful = false;
		}

		if (!actionSuccessful)
		{
			drawUtils.RestoreRoomCardFromForeground(&backgroundDc, &foregroundDc, draggingCardIndex);
		}
		draggingCardIndex = -1;

		if (actionSuccessful)
		{

			GameStatus status = game.GetGameStatus();
			if (status == GAME_STATUS_LOST)
			{
				CString str;
				str.Format(_T( "You lost! Score %d" ), game.CalculateScore());
				MessageBox(str, _T("Scoundrel"), MB_OK);
				game.InitializeNewGame();
				Invalidate(true);
				return;
			}
			else if (status == GAME_STATUS_WON)
			{
				CString str;
				str.Format(_T( "You won! Score %d" ), game.CalculateScore());
				MessageBox(str, _T("Scoundrel"), MB_OK);
				game.InitializeNewGame();
				Invalidate(true);
				return;
			}

			if (game.ShouldDraw())
			{
				// Blit earlier for immediate feedback, since full repaint is quite slow
				BlitInMemoryDCs(&dc);
				game.DrawRoom();
				game.SetCanRun();
				int deckSize = game.remaining.GetCount() == 0 ? 0 : 1 + game.remaining.GetCount() / 10;
				if (deckSize != previousDeckSize)
				{
					previousDeckSize = deckSize;
					drawUtils.RepaintDeck(&backgroundDc, deckSize);
				}
				drawUtils.RepaintRoom(&backgroundDc, game, -1);
			}
		}

		BlitInMemoryDCs(&dc);
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CScoundrelDlg::ShowMessage(CString message)
{
	// MessageBox(message, _T("Scoundrel"), MB_OK | MB_ICONINFORMATION);
	drawUtils.DrawHUD(&backgroundDc, game, message);
}

void CScoundrelDlg::HandleGameError(int error)
{
	CString message;
	switch (error)
	{
	case GAME_ERROR_NONE:
		message = _T("No error");
		break;
	case GAME_ERROR_INVALID_ROOM_SIZE:
		message = _T("Invalid room size");
		break;
	case GAME_ERROR_INVALID_CARD_INDEX:
		message = _T("Invalid card index");
		break;
	case GAME_ERROR_CANNOT_RUN:
		message = _T("Can't escape!");
		break;
	case GAME_ERROR_NO_WEAPON_EQUIPPED:
		message = _T("No weapon equipped");
		break;
	case GAME_ERROR_NO_ROOM_CARD:
		message = _T("No room card at index");
		break;
	case GAME_ERROR_NOT_A_WEAPON:
		message = _T("Not a weapon");
		break;
	case GAME_ERROR_NOT_A_POTION:
		message = _T("Not a potion");
		break;
	case GAME_ERROR_NOT_A_MONSTER:
		message = _T("Not a monster");
		break;
	case GAME_ERROR_EXCEED_WEAPON_DURABILITY:
		message = _T("Exceeds durability!");
		break;
	default:
		message = _T("Unknown error");
		break;
	}
	ShowMessage(message);
}
