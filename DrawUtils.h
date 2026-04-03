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

class DrawUtils
{
  public:
	void DrawCardAtPoint(CPoint point, Card card, CDC *dc);
	void Init(CDC *dc);
	DrawUtils();
	virtual ~DrawUtils();

  private:
	CFont cardFont;
	CBitmap suitsSpritesheet;
	CSize cardSize;
	CDC suitsMemDC;
	CDC cardCanvasDc;
	void DrawSuitAtPoint(int x, int y, CardSuit suit, CDC *dc);
	void FlipDC180(CDC *pSrcDC, CDC *pDstDC, int width, int height);
};

#endif // !defined(AFX_DRAWUTILS_H__EACA2ACB_396D_4BA1_AAE2_2507686CDD75__INCLUDED_)
