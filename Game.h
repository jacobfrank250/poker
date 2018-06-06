#ifndef game_h
#define game_h
#include "stdafx.h"
#include "functions.h"
#include "player.h"
#include "deck.h"
#include <string>
#include <vector>
#include <iostream>
#include <memory>

using namespace std;
class SevenCardStud;
class Game {
protected:
	static shared_ptr<Game> ptr;
	Deck main_deck;
	vector<shared_ptr<Player>> players;
public:
	static shared_ptr<Game> instance();
	static void start_game(const string& str);
	static void stop_game();
	void add_player(const string& str);
	shared_ptr<Player> find_player(const string& str);
	int size() const;
	//virtual ~Game();
	//Don't need to include virtual destructor b/c of shared_ptrs
	virtual int before_turn(Player& p) = 0;
	virtual int turn5(Player& p) = 0;
	virtual int turn7(Player& p, int turn) = 0;
	virtual int after_turn(Player& p) = 0;
	virtual int before_round() = 0;
	virtual int round() = 0;
	virtual int after_round() = 0;
	virtual void betting_phase(Player& p) = 0;
	virtual int betting_round() = 0;
	void remove_player(const string& str);
	//void add_to_pot(Player& p, unsigned int amount);
	static bool compareHand(std::shared_ptr<Player> p1, std::shared_ptr<Player> p2);
	static bool players_same_hands(shared_ptr<Player> p1, shared_ptr<Player> p2);
};

#endif;
