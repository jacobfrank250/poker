#ifndef FIVECARDDRAW_h
#define FIVECARDDRAW_h

#include "stdafx.h"
#include "functions.h"
#include "deck.h"
#include "hand.h"
#include "player.h"
#include "game.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

class FiveCardDraw : public Game {
protected:
	size_t dealer; //start at 0
	Deck discardDeck; //under the assumption that the deck constructor initializes empty deck
	int commonChipPot; //NEW: we were not specifically asked to make it protected/member variable but i think this is the best thing to do
	unsigned int current_bet; //NEW: this will record the current bet for the phase

public:
	FiveCardDraw();
	virtual int before_turn(Player& p);
	virtual int turn5(Player& p);
	virtual int turn7(Player& p, int turn);
	virtual int after_turn(Player& p);
	virtual int before_round();
	virtual int round();
	virtual int after_round();
	virtual void betting_phase(Player& p);
	virtual int betting_round();
};

#endif
