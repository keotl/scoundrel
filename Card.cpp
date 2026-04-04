// Card.cpp: implementation of the Card class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Scoundrel.h"
#include "Card.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Card::Card(int rank, CardSuit suit)
	: rank(rank), suit(suit)
{
}

Card::Card(const Card& other)
	: rank(other.rank), suit(other.suit)
{
}

Card& Card::operator=(const Card& other)
{
	if (this != &other) {
		rank = other.rank;
		suit = other.suit;
	}
	return *this;
}

Card::~Card()
{

}

int Card::Value()
{
	if (rank == 1) {
		return 14;
	}
	return rank;
}
