// DrawUtils.h: interface for the DrawUtils class.
//
//////////////////////////////////////////////////////////////////////

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

	void Init(CDC *dc);
	void DrawCardAtPoint(CPoint point, Card card, CDC *dc);
	void DrawGameState(CDC *dc, CRect clientRect, const GameState& game, int ignoringRoomCardIndex);

  private:
	CSize cardSize;
	int roomCardMargin;
	CFont cardFont;
	CBitmap suitsSpritesheet;
	CDC suitsMemDC;
	CDC cardCanvasDc;
	void DrawSuitAtPoint(int x, int y, CardSuit suit, CDC *dc);
	void FlipDC180(CDC *pSrcDC, CDC *pDstDC, int width, int height);
};

#endif // !defined(AFX_DRAWUTILS_H__EACA2ACB_396D_4BA1_AAE2_2507686CDD75__INCLUDED_)
