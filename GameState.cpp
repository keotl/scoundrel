// GameState.cpp: implementation of the GameState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Scoundrel.h"
#include "Card.h"
#include "GameState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static const int MAX_HEALTH = 20;

const int GameState::ROOM_SIZE = 4;

GameState::GameState()
{
	for (int i = 0; i < ROOM_SIZE; i++)
	{
		room[i] = NULL;
	}
}

GameState::~GameState()
{
}

static Card allCards[] = {
	Card(1, SPADE),
	Card(2, SPADE),
	Card(3, SPADE),
	Card(4, SPADE),
	Card(5, SPADE),
	Card(6, SPADE),
	Card(7, SPADE),
	Card(8, SPADE),
	Card(9, SPADE),
	Card(10, SPADE),
	Card(11, SPADE),
	Card(12, SPADE),
	Card(13, SPADE),
	Card(1, CLUB),
	Card(2, CLUB),
	Card(3, CLUB),
	Card(4, CLUB),
	Card(5, CLUB),
	Card(6, CLUB),
	Card(7, CLUB),
	Card(8, CLUB),
	Card(9, CLUB),
	Card(10, CLUB),
	Card(11, CLUB),
	Card(12, CLUB),
	Card(13, CLUB),
	// No face cards/Aces for red cards
	Card(2, DIAMOND),
	Card(3, DIAMOND),
	Card(4, DIAMOND),
	Card(5, DIAMOND),
	Card(6, DIAMOND),
	Card(7, DIAMOND),
	Card(8, DIAMOND),
	Card(9, DIAMOND),
	Card(10, DIAMOND),
	Card(2, HEART),
	Card(3, HEART),
	Card(4, HEART),
	Card(5, HEART),
	Card(6, HEART),
	Card(7, HEART),
	Card(8, HEART),
	Card(9, HEART),
	Card(10, HEART),
};

static const int DECK_SIZE = 52 - 4 * 2;

void GameState::InitializeNewGame()
{
	canRun = false;
	health = MAX_HEALTH;
	remaining.RemoveAll();
	discarded.RemoveAll();
	activeWeapon = NULL;
	foughtByWeapon.RemoveAll();

	int i;
	for (i = 0; i < ROOM_SIZE; i++)
	{
		room[i] = NULL;
	}

	// Prepare temp array for shuffle
	Card *cardArray[DECK_SIZE];
	for (i = 0; i < DECK_SIZE; i++)
	{
		cardArray[i] = &allCards[i];
	}

	// Shuffle deck
	for (i = DECK_SIZE - 1; i > 0; i--)
	{
		int j = rand() % (i + 1);
		Card *temp = cardArray[j];
		cardArray[j] = cardArray[i];
		cardArray[i] = temp;
	}

	for (i = 0; i < DECK_SIZE; i++)
	{
		remaining.AddTail(cardArray[i]);
	}
	DrawRoom();
}

int GameState::DrawRoom()
{
	canDrink = true;
	int nonEmptyIndex = -1;
	int i;
	for (i = 0; i < ROOM_SIZE; i++)
	{
		if (room[i] != NULL)
		{
			nonEmptyIndex = i;
		}
	}

	if (nonEmptyIndex > 0)
	{
		room[0] = room[nonEmptyIndex];
		room[nonEmptyIndex] = NULL;
	}

	int emptySlots = 0;
	for (i = 0; i < ROOM_SIZE; i++)
	{
		if (room[i] == NULL)
		{
			emptySlots++;
		}
	}

	int cardsToDraw = min(emptySlots, remaining.GetCount());

	for (i = 0; i < cardsToDraw; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (room[j] == NULL)
			{
				room[j] = remaining.RemoveHead();
				break;
			}
		}
	}

	return GAME_ERROR_NONE;
}

int GameState::RunAway()
{
	if (!canRun)
	{
		return GAME_ERROR_CANNOT_RUN;
	}
	canRun = false;
	int i;
	for (i = 0; i < ROOM_SIZE; i++)
	{
		if (room[i] != NULL)
		{
			remaining.AddTail(room[i]);
			room[i] = NULL;
		}
	}
	return DrawRoom();
}

int GameState::Equip(int cardIndexInRoom)
{
	if (cardIndexInRoom < 0 || cardIndexInRoom >= ROOM_SIZE || room[cardIndexInRoom] == NULL)
	{
		return GAME_ERROR_INVALID_CARD_INDEX;
	}
	if (room[cardIndexInRoom]->suit != DIAMOND)
	{
		return GAME_ERROR_NOT_A_WEAPON;
	}
	if (activeWeapon)
	{
		discarded.AddTail(activeWeapon);
		while (foughtByWeapon.GetCount() > 0)
		{
			discarded.AddTail(foughtByWeapon.RemoveHead());
		}
	}
	activeWeapon = room[cardIndexInRoom];
	room[cardIndexInRoom] = NULL;

	return GAME_ERROR_NONE;
}

int GameState::DrinkPotion(int cardIndexInRoom)
{
	if (cardIndexInRoom < 0 || cardIndexInRoom >= ROOM_SIZE || room[cardIndexInRoom] == NULL)
	{
		return GAME_ERROR_INVALID_CARD_INDEX;
	}
	if (room[cardIndexInRoom]->suit != HEART)
	{
		return GAME_ERROR_NOT_A_POTION;
	}

	if (canDrink)
	{
		health = min(health + room[cardIndexInRoom]->Value(), MAX_HEALTH);
	}

	canDrink = false;
	discarded.AddTail(room[cardIndexInRoom]);
	room[cardIndexInRoom] = NULL;
	return GAME_ERROR_NONE;
}

int GameState::FightBarehanded(int cardIndexInRoom)
{
	if (cardIndexInRoom < 0 || cardIndexInRoom >= ROOM_SIZE || room[cardIndexInRoom] == NULL)
	{
		return GAME_ERROR_INVALID_CARD_INDEX;
	}
	Card *monster = room[cardIndexInRoom];
	if (monster->suit != SPADE && monster->suit != CLUB)
	{
		return GAME_ERROR_NOT_A_MONSTER;
	}

	health -= monster->Value();
	discarded.AddTail(monster);
	room[cardIndexInRoom] = NULL;
	return GAME_ERROR_NONE;
}

int GameState::FightWithWeapon(int cardIndexInRoom)
{
	if (cardIndexInRoom < 0 || cardIndexInRoom >= ROOM_SIZE || room[cardIndexInRoom] == NULL)
	{
		return GAME_ERROR_INVALID_CARD_INDEX;
	}
	if (activeWeapon == NULL)
	{
		return GAME_ERROR_NO_WEAPON_EQUIPPED;
	}
	Card *monster = room[cardIndexInRoom];
	if (monster->suit != SPADE && monster->suit != CLUB)
	{
		return GAME_ERROR_NOT_A_MONSTER;
	}

	int durability = 15;
	if (foughtByWeapon.GetCount() > 0)
	{
		durability = foughtByWeapon.GetTail()->Value();
	}
	if (monster->Value() >= durability)
	{
		return GAME_ERROR_EXCEED_WEAPON_DURABILITY;
	}

	health -= max(0, monster->Value() - activeWeapon->Value());
	foughtByWeapon.AddTail(monster);
	room[cardIndexInRoom] = NULL;
	return GAME_ERROR_NONE;
}

GameStatus GameState::GetGameStatus()
{
	if (health <= 0)
	{
		return GAME_STATUS_LOST;
	}
	Card *remainingPotion = NULL;
	int remainingCardsInRoom = 0;
	for (int i = 0; i < 4; i++)
	{
		if (room[i] != NULL)
		{
			remainingCardsInRoom++;
			if (room[i]->suit == HEART)
			{
				remainingPotion = room[i];
			}
		}
	}
	if (
		(remaining.GetCount() == 0 && remainingCardsInRoom == 1 && remainingPotion != NULL) ||
		(remaining.GetCount() == 0 && remainingCardsInRoom == 0))
	{
		return GAME_STATUS_WON;
	}

	return GAME_STATUS_RUNNING;
}

int GameState::CalculateScore()
{
	Card *remainingPotion = NULL;
	int remainingMonsterHealth = 0;
	for (int i = 0; i < 4; i++)
	{
		if (room[i] != NULL)
		{
			if (room[i]->suit == HEART && remainingPotion == NULL)
			{
				remainingPotion = room[i];
			}
			else if (room[i]->suit == SPADE || room[i]->suit == CLUB)
			{
				remainingMonsterHealth += room[i]->Value();
			}
		}
	}
	POSITION pos = remaining.GetHeadPosition();
	while (pos != NULL)
	{
		Card *card = remaining.GetNext(pos);
		if (card->suit == SPADE || card->suit == CLUB)
		{
			remainingMonsterHealth += card->Value();
		}
	}

	if (remainingMonsterHealth)
	{
		return -remainingMonsterHealth;
	}
	else if (remainingPotion)
	{
		return health + remainingPotion->Value();
	}

	return health;
}
