// DrawUtils.cpp: implementation of the DrawUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "Card.h"
#include "afxwin.h"
#include "stdafx.h"
#include "Scoundrel.h"
#include "wingdi.h"
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

void DrawUtils::Init(CDC *dc, CRect clientRect)
{
	this->bgColor = RGB(0, 130, 0);
	this->darkerBgColor = RGB(0, 100, 0);
	this->clientRect = clientRect;
	this->cardSize = CSize(29, 41);
	this->roomCardMargin = 3;
	this->roomOrigin = clientRect.CenterPoint();
	this->roomOrigin.Offset(-cardSize.cx * 2 - roomCardMargin * 2, 0);
	this->roomOrigin.y = 40;
	this->cardFont.CreateFont(12, 0,
							  0, 0,
							  FW_BOLD,
							  FALSE, FALSE, FALSE, DEFAULT_CHARSET,
							  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							  DEFAULT_QUALITY, DEFAULT_PITCH, _T("Tahoma"));
	this->suitsSpritesheet.LoadBitmap(IDB_BITMAP1);
	this->cardBackSpritesheet.LoadBitmap(IDB_BITMAP2);
	this->suitsMemDC.CreateCompatibleDC(dc);
	this->suitsMemDC.SelectObject(&suitsSpritesheet);
	this->cardBackMemDC.CreateCompatibleDC(dc);
	this->cardBackMemDC.SelectObject(&cardBackSpritesheet);
	this->cardCanvasDc.CreateCompatibleDC(dc);
	this->armourOrigin.x = roomOrigin.x;
	this->armourOrigin.y = roomOrigin.y + 60;
	this->durabilityOrigin.x = armourOrigin.x + cardSize.cx + 8;
	this->durabilityOrigin.y = armourOrigin.y;
	this->deckOrigin = CPoint(4, roomOrigin.y);
	this->usePlaceholderOrigin = CPoint(clientRect.Width() - 4 - cardSize.cx, roomOrigin.y);
	this->armourRegion = CRect(armourOrigin, cardSize);
	this->durabilityRegion = CRect(durabilityOrigin, CSize(cardSize.cx * 3, cardSize.cy));
	this->usePlaceholderRegion = CRect(usePlaceholderOrigin, cardSize);
	this->deckRegion = CRect(deckOrigin, CSize(cardSize.cx + 4, cardSize.cy + 3));
	this->selectedCardBack = 0;
	this->hudRegion = CRect(0, clientRect.BottomRight().y - 20, clientRect.BottomRight().x, clientRect.BottomRight().y);
	this->hudColor = RGB(200, 200, 200);
	this->roomRegion = CRect(roomOrigin, CSize((cardSize.cx + roomCardMargin) * GameState::ROOM_SIZE, cardSize.cy));
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

void DrawUtils::DrawCardAtPoint(CPoint point, const Card *card, CDC *dc)
{
	// Prepare label+suit canvas (10px wide, 18px tall: 10 for rank + 8 for suit)
	int canvasW = 17;
	int canvasH = 20;
	CBitmap canvas;
	canvas.CreateCompatibleBitmap(dc, canvasW, canvasH);
	cardCanvasDc.SelectObject(&canvas);

	// Clear canvas to white
	cardCanvasDc.FillSolidRect(0, 0, canvasW, canvasH, RGB(255, 255, 255));

	// Draw rank label
	CRect labelRect(CPoint(0, 0), CSize(10, 12));
	cardCanvasDc.SetTextColor(RGB((card->suit == HEART || card->suit == DIAMOND) ? 132 : 0, 0, 0));
	cardCanvasDc.SelectObject(&this->cardFont);
	cardCanvasDc.DrawText(card->Label(), labelRect, DT_CENTER);

	// Draw suit next to rank
	cardCanvasDc.BitBlt(10, 2, 7, 8, &this->suitsMemDC, 7 * card->suit, 0, SRCCOPY);
	// Draw suit below rank
	cardCanvasDc.BitBlt(1, 12, 7, 8, &this->suitsMemDC, 7 * card->suit, 0, SRCCOPY);

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

void DrawUtils::DrawGameState(CDC *dc, const GameState &game, int ignoringRoomCardIndex)
{
	CBrush brush;
	CPen pen;

	// Draw background
	brush.CreateSolidBrush(bgColor);
	dc->FillRect(&this->clientRect, &brush);

	// Draw room
	RepaintRoom(dc, game, ignoringRoomCardIndex);

	// Draw Weapon + durability
	if (game.activeWeapon == NULL)
	{
		CRect armourRegion = CRect(armourOrigin, cardSize);
		dc->FillSolidRect(armourRegion, darkerBgColor);
	}
	else
	{
		DrawCardAtPoint(armourOrigin, game.activeWeapon, dc);
	}
	CRect durabilityRegion = CRect(durabilityOrigin, CSize(cardSize.cx * 3, cardSize.cy));
	dc->FillSolidRect(durabilityRegion, darkerBgColor);

	CPoint durabilityCardPos(durabilityOrigin);
	for (POSITION pos = game.foughtByWeapon.GetHeadPosition(); pos != NULL;)
	{
		Card *card = game.foughtByWeapon.GetNext(pos);
		DrawCardAtPoint(durabilityCardPos, card, dc);
		durabilityCardPos.Offset(8, 0);
	}

	// Draw deck
	RepaintDeck(dc, game.remaining.GetCount() == 0 ? 0 : 1 + game.remaining.GetCount() / 10);

	// Draw usage placeholder
	pen.CreatePen(PS_DASH, 1, RGB(10, 10, 10));
	CRect usagePlaceholder(usePlaceholderOrigin, cardSize);
	dc->SelectObject(&pen);
	dc->SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
	dc->Rectangle(usagePlaceholder);

	// Draw HUD
	DrawHUD(dc, game, _T( "" ));
}

int DrawUtils::GetRoomCardIndexAtPoint(const CPoint &point)
{
	if (point.y > roomOrigin.y + cardSize.cy || point.y < roomOrigin.y || point.x < roomOrigin.x || point.x > roomOrigin.x + (cardSize.cx + roomCardMargin) * GameState::ROOM_SIZE)
	{
		return -1;
	}
	return (point.x - roomOrigin.x) / (cardSize.cx + roomCardMargin);
}

CRect DrawUtils::GetRoomCardRect(int roomCardIndex)
{
	CPoint cardPos(roomOrigin);
	cardPos.Offset((cardSize.cx + roomCardMargin) * roomCardIndex, 0);
	CRect result(cardPos, cardSize);
	return result;
}

void DrawUtils::TransferRoomCard(CDC *backgroundDc, CDC *foregroundDc, int roomCardIndex)
{
	CRect cardRect = GetRoomCardRect(roomCardIndex);
	foregroundDc->BitBlt(0, 0, cardSize.cx, cardSize.cy, backgroundDc, cardRect.TopLeft().x, cardRect.TopLeft().y, SRCCOPY);
	backgroundDc->FillSolidRect(cardRect, bgColor);
}

void DrawUtils::DrawCardBackAtPoint(CPoint point, CDC *dc)
{
	dc->BitBlt(point.x, point.y, cardSize.cx, cardSize.cy, &cardBackMemDC, 0, 0, SRCCOPY);
}

BOOL DrawUtils::IsPointInDeckRegion(const CPoint &point)
{
	return deckRegion.TopLeft().x <= point.x && deckRegion.TopLeft().y <= point.y && point.x <= deckRegion.BottomRight().x && point.y <= deckRegion.BottomRight().y;
}

BOOL DrawUtils::IsPointInArmourRegion(const CPoint &point)
{
	return armourRegion.TopLeft().x <= point.x && armourRegion.TopLeft().y <= point.y && point.x <= armourRegion.BottomRight().x && point.y <= armourRegion.BottomRight().y;
}

BOOL DrawUtils::IsPointInDurabilityRegion(const CPoint &point)
{
	return durabilityRegion.TopLeft().x <= point.x && durabilityRegion.TopLeft().y <= point.y && point.x <= durabilityRegion.BottomRight().x && point.y <= durabilityRegion.BottomRight().y;
}

BOOL DrawUtils::IsPointInUsePlaceholderRegion(const CPoint &point)
{
	return usePlaceholderRegion.TopLeft().x <= point.x && usePlaceholderRegion.TopLeft().y <= point.y && point.x <= usePlaceholderRegion.BottomRight().x && point.y <= usePlaceholderRegion.BottomRight().y;
}

void DrawUtils::RestoreRoomCardFromForeground(CDC *backgroundDc, CDC *foregroundDc, int roomCardIndex)
{
	CRect cardRect = GetRoomCardRect(roomCardIndex);
	backgroundDc->BitBlt(cardRect.TopLeft().x, cardRect.TopLeft().y, cardSize.cx, cardSize.cy, foregroundDc, 0, 0, SRCCOPY);
	// backgroundDc->FillSolidRect(cardRect, bgColor);
}

void DrawUtils::TransferForegroundCardToArmourRegion(CDC *backgroundDc, CDC *foregroundDc)
{
	backgroundDc->BitBlt(armourOrigin.x, armourOrigin.y, cardSize.cx, cardSize.cy, foregroundDc, 0, 0, SRCCOPY);
}
void DrawUtils::TransferForegroundCardToDurabilityRegion(CDC *backgroundDc, CDC *foregroundDc, int indexInDurability)
{
	backgroundDc->BitBlt(durabilityOrigin.x + indexInDurability * 8, durabilityOrigin.y, cardSize.cx, cardSize.cy, foregroundDc, 0, 0, SRCCOPY);
}

void DrawUtils::ClearDurabilityRegion(CDC *backgroundDc)
{
	CRect durabilityRegion = CRect(durabilityOrigin, CSize(cardSize.cx * 3, cardSize.cy));
	backgroundDc->FillSolidRect(durabilityRegion, darkerBgColor);
}

void DrawUtils::DrawHUD(CDC *backgroundDc, const GameState &game, CString actionMessage)
{
	backgroundDc->FillSolidRect(hudRegion, hudColor);
	CRect healthRect(hudRegion.left + 2, hudRegion.top + 2, hudRegion.right - 2, hudRegion.bottom - 2);
	CString str;
	str.Format(_T( "Health: %d" ), game.health);
	backgroundDc->DrawText(str, healthRect, DT_LEFT);
	backgroundDc->DrawText(actionMessage, healthRect, DT_RIGHT);
}

void DrawUtils::RepaintRoom(CDC *dc, const GameState &game, int ignoringRoomCardIndex)
{
	dc->FillSolidRect(&roomRegion, bgColor);
	for (int i = 0; i < GameState::ROOM_SIZE; i++)
	{
		CPoint cardPos(roomOrigin);
		cardPos.Offset((cardSize.cx + roomCardMargin) * i, 0);

		if (game.room[i] != NULL && i != ignoringRoomCardIndex)
		{
			DrawCardAtPoint(cardPos, game.room[i], dc);
		}
	}
}

void DrawUtils::RepaintDeck(CDC *backgroundDc, int size)
{
	CPen pen;
	backgroundDc->FillSolidRect(deckRegion, bgColor);
	CPoint deckCardOrigin(deckOrigin);
	if (size == 0)
	{
		// Empty deck placeholder
		pen.CreatePen(PS_DASH, 1, RGB(10, 10, 10));
		backgroundDc->SelectObject(&pen);
		backgroundDc->SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
		backgroundDc->Rectangle(CRect(deckOrigin, cardSize));
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
		backgroundDc->SelectObject(&pen);
		CPoint deckCenter = CRect(deckOrigin, cardSize).CenterPoint();
		backgroundDc->Ellipse(CRect(deckCenter.x - 10, deckCenter.y - 10, deckCenter.x + 10, deckCenter.y + 10));
		pen.DeleteObject();
	}
	else
	{
		for (int i = 0; i < min(size, 3); i++)
		{
			DrawCardBackAtPoint(deckCardOrigin, backgroundDc);
			deckCardOrigin.Offset(2, 1);
		}
	}
}
