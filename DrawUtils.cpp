// DrawUtils.cpp: implementation of the DrawUtils class.
//
//////////////////////////////////////////////////////////////////////

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

DrawUtils::DrawUtils() {
  this->cardSize = CSize(29, 41);
  this->cardFont.CreateFont(12, 0,   // height, width
                            0, 0,    // escapement, orientation
                            FW_BOLD, // weight
                            FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                            DEFAULT_QUALITY, DEFAULT_PITCH, _T("Tahoma"));
  this->suitsSpritesheet.LoadBitmap(IDB_BITMAP1);
}

DrawUtils::~DrawUtils() {}

void DrawUtils::DrawCardAtPoint(CPoint point, CDC *dc) {
  CRect cardRect(point, this->cardSize);

  CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
  CBrush brush(RGB(255, 255, 255));

  CPen *pOldPen = dc->SelectObject(&pen);
  CBrush *pOldBrush = dc->SelectObject(&brush);

  dc->Rectangle(cardRect);

  CRect labelRect(CPoint(point.x + 2, point.y + 2), CSize(10, 20));
  dc->SetTextColor(RGB(0, 0, 0));

  dc->SelectObject(&this->cardFont);

  dc->DrawText("8", labelRect, DT_CENTER);

  dc->SelectObject(pOldPen);
  dc->SelectObject(pOldBrush);
}
