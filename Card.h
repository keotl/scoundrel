// Card.h: interface for the Card class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARD_H__C31D4891_8D2D_4287_BFC0_0E5EAA6C3B4E__INCLUDED_)
#define AFX_CARD_H__C31D4891_8D2D_4287_BFC0_0E5EAA6C3B4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum CardSuit {
 SPADE = 0,
DIAMOND = 1,
  HEART = 2,
  CLUB = 3
};

class Card  
{
public:
	Card(int rank, CardSuit suit );
	virtual ~Card();

};

#endif // !defined(AFX_CARD_H__C31D4891_8D2D_4287_BFC0_0E5EAA6C3B4E__INCLUDED_)
