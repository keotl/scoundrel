// DrawUtils.h: interface for the DrawUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "windef.h"
#if !defined(AFX_DRAWUTILS_H__EACA2ACB_396D_4BA1_AAE2_2507686CDD75__INCLUDED_)
#define AFX_DRAWUTILS_H__EACA2ACB_396D_4BA1_AAE2_2507686CDD75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "Card.h"
#include "GameState.h"

class DrawUtils
{
  public:
	DrawUtils();
	virtual ~DrawUtils();

	void Init(CDC *dc, CRect clientRect);
	void DrawCardAtPoint(CPoint point, const Card *card, CDC *dc);
	void DrawCardBackAtPoint(CPoint point, CDC *dc);
	void DrawGameState(CDC *dc, const GameState &game, int ignoringRoomCardIndex);
	int GetRoomCardIndexAtPoint(const CPoint &point);
	void TransferRoomCard(CDC *backgroundDc, CDC *foregroundDc, int roomCardIndex);
	void RestoreRoomCardFromForeground(CDC *backgroundDc, CDC *foregroundDc, int roomCardIndex);
	void TransferForegroundCardToArmourRegion(CDC *backgroundDc, CDC *foregroundDc);
	void TransferForegroundCardToDurabilityRegion(CDC *backgroundDc, CDC *foregroundDc, int indexInDurability);
	void ClearDurabilityRegion(CDC *backgroundDc);
	CRect GetRoomCardRect(int roomCardIndex);
	BOOL IsPointInArmourRegion(const CPoint &point);
	BOOL IsPointInDurabilityRegion(const CPoint &point);
	BOOL IsPointInUsePlaceholderRegion(const CPoint &point);
	BOOL IsPointInDeckRegion(const CPoint &point);
	void DrawHUD(CDC*backgroundDc, const GameState &game, CString actionMessage);
	void RepaintRoom(CDC*backgroundDc, const GameState & game, int ignoringRoomCardIndex);
	void RepaintDeck(CDC*backgroundDc, int size);

	CSize cardSize;
	CPoint roomOrigin;
	CRect clientRect;

  private:
	int roomCardMargin;
	CFont cardFont;
	CBitmap suitsSpritesheet;
	CBitmap cardBackSpritesheet;
	CDC suitsMemDC;
	CDC cardBackMemDC;
	CDC cardCanvasDc;
	void DrawSuitAtPoint(int x, int y, CardSuit suit, CDC *dc);
	void FlipDC180(CDC *pSrcDC, CDC *pDstDC, int width, int height);
	COLORREF bgColor;
	COLORREF darkerBgColor;
	CPoint armourOrigin;
	CPoint durabilityOrigin;
	CPoint deckOrigin;
	CPoint usePlaceholderOrigin;
	int selectedCardBack;
	CRect deckRegion;
	CRect armourRegion;
	CRect durabilityRegion;
	CRect usePlaceholderRegion;
	CRect hudRegion;
	COLORREF hudColor;
	CRect roomRegion;
};

#endif // !defined(AFX_DRAWUTILS_H__EACA2ACB_396D_4BA1_AAE2_2507686CDD75__INCLUDED_)
