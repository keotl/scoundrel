// DrawUtils.cpp: implementation of the DrawUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "afxwin.h"
#include "stdafx.h"
#include "Scoundrel.h"
#include "DrawUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DrawUtils::DrawUtils()
{
}

void DrawUtils::Init(CDC *dc)
{
	this->cardSize = CSize(29, 41);
	this->roomCardMargin = 4;
	this->cardFont.CreateFont(12, 0,
							  0, 0,
							  FW_BOLD,
							  FALSE, FALSE, FALSE, DEFAULT_CHARSET,
							  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							  DEFAULT_QUALITY, DEFAULT_PITCH, _T("Tahoma"));
	this->suitsSpritesheet.LoadBitmap(IDB_BITMAP1);
	this->suitsMemDC.CreateCompatibleDC(dc);
	this->suitsMemDC.SelectObject(&suitsSpritesheet);
	this->cardCanvasDc.CreateCompatibleDC(dc);
}

DrawUtils::~DrawUtils()
{
}

void DrawUtils::DrawSuitAtPoint(int x, int y, CardSuit suit, CDC *dc)
{
	dc->BitBlt(x, y, 7, 8, &this->suitsMemDC, 0, 8 * suit, SRCCOPY);
}

void DrawUtils::FlipDC180(CDC *pSrcDC, CDC *pDstDC, int width, int height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			COLORREF pixel = pSrcDC->GetPixel(x, y);
			pDstDC->SetPixel(width - 1 - x, height - 1 - y, pixel);
		}
	}
}

void DrawUtils::DrawCardAtPoint(CPoint point, Card card, CDC *dc)
{
	// Prepare label+suit canvas (10px wide, 18px tall: 10 for rank + 8 for suit)
	int canvasW = 17;
	int canvasH = 18;
	CBitmap canvas;
	canvas.CreateCompatibleBitmap(dc, canvasW, canvasH);
	cardCanvasDc.SelectObject(&canvas);

	// Clear canvas to white
	cardCanvasDc.FillSolidRect(0, 0, canvasW, canvasH, RGB(255, 255, 255));

	// TODO - select suit color  - keotl 2026-04-03
	// Draw rank label
	CRect labelRect(CPoint(0, 0), CSize(10, 10));
	cardCanvasDc.SetTextColor(RGB(0, 0, 0));
	cardCanvasDc.SelectObject(&this->cardFont);
	CString rankStr;
	rankStr.Format(_T("%d"), card.rank);
	cardCanvasDc.DrawText(rankStr, labelRect, DT_CENTER);

	// Draw suit next to rank
	cardCanvasDc.BitBlt(10, 1, 7, 8, &this->suitsMemDC, 7 * card.suit, 0, SRCCOPY);
	// Draw suit below rank
	cardCanvasDc.BitBlt(1, 10, 7, 8, &this->suitsMemDC, 7 * card.suit, 0, SRCCOPY);

	// Draw card outline
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush brush(RGB(255, 255, 255));
	CPen *pOldPen = dc->SelectObject(&pen);
	CBrush *pOldBrush = dc->SelectObject(&brush);
	CRect cardRect(point, this->cardSize);
	dc->Rectangle(cardRect);

	// Blit label+suit to top-left corner
	dc->BitBlt(point.x + 1, point.y + 1, canvasW, canvasH, &cardCanvasDc, 0, 0, SRCCOPY);

	// Flip 180 and blit to bottom-right corner
	CDC flippedDC;
	CBitmap flippedBmp;
	flippedDC.CreateCompatibleDC(dc);
	flippedBmp.CreateCompatibleBitmap(dc, canvasW, canvasH);
	flippedDC.SelectObject(&flippedBmp);
	FlipDC180(&cardCanvasDc, &flippedDC, canvasW, canvasH);
	dc->BitBlt(point.x + cardSize.cx - canvasW - 1, point.y + cardSize.cy - canvasH - 1,
			   canvasW, canvasH, &flippedDC, 0, 0, SRCCOPY);

	dc->SelectObject(pOldPen);
	dc->SelectObject(pOldBrush);
}

void DrawUtils::DrawGameState(CDC *dc, CRect clientRect, const GameState &game, int ignoringRoomCardIndex)
{

	// Draw background
	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 130, 0));
	dc->FillRect(&clientRect, &brush);

	// Draw room
	CPoint roomOrigin(clientRect.CenterPoint());
	roomOrigin.Offset(-cardSize.cx * 2 - roomCardMargin * 2, 0);
	roomOrigin.y = 40;

	for (int i = 0; i < GameState::ROOM_SIZE; i++)
	{
		CPoint cardPos(roomOrigin);
		cardPos.Offset((cardSize.cx + roomCardMargin) * i, 0);

		if (game.room[i] != NULL)
		{
			DrawCardAtPoint(cardPos, *game.room[i], dc);
		}
	}

	// Draw Weapon + durability

	// Draw discarded
}
