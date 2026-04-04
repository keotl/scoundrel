// GameState.h: interface for the GameState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMESTATE_H__BFD6B92F_52AC_46E8_964D_4635056ADD9E__INCLUDED_)
#define AFX_GAMESTATE_H__BFD6B92F_52AC_46E8_964D_4635056ADD9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "Card.h"
#include "afxtempl.h"

enum GameError
{
	GAME_ERROR_NONE = 0,
	GAME_ERROR_INVALID_ROOM_SIZE,
	GAME_ERROR_INVALID_CARD_INDEX,
	GAME_ERROR_CANNOT_RUN,
	GAME_ERROR_NO_WEAPON_EQUIPPED,
	GAME_ERROR_NO_ROOM_CARD,
	GAME_ERROR_NOT_A_WEAPON,
	GAME_ERROR_NOT_A_POTION,
	GAME_ERROR_NOT_A_MONSTER,
	GAME_ERROR_EXCEED_WEAPON_DURABILITY
};

enum GameStatus
{
	GAME_STATUS_RUNNING,
	GAME_STATUS_LOST,
	GAME_STATUS_WON
};

class GameState
{
  public:
	GameState();
	virtual ~GameState();
	void InitializeNewGame();

	int DrawRoom();
	int RunAway();
	int Equip(int cardIndexInRoom);
	int DrinkPotion(int cardIndexInRoom);
	int FightBarehanded(int cardIndexInRoom);
	int FightWithWeapon(int cardIndexInRoom);

	GameStatus GetGameStatus();
	int CalculateScore();

	int health;
	// Card arrays. Never deallocate cards, only move them from one array to the next.
	CList<Card *, Card *> remaining;
	CList<Card *, Card *> discarded;
	Card* activeWeapon;
	CList<Card *, Card *> foughtByWeapon;

  private:
	BOOL canRun;
	BOOL canDrink;
	Card* room[4];
};

#endif // !defined(AFX_GAMESTATE_H__BFD6B92F_52AC_46E8_964D_4635056ADD9E__INCLUDED_)
