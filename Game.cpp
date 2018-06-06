#include "stdafx.h"
#include "deck.h"
#include "functions.h"
#include "game.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <memory>

using namespace std;

shared_ptr<Game> Game::ptr;


shared_ptr<Game> Game::instance() {
	if (!ptr) {
		throw instance_not_available;
	}
	shared_ptr<Game> ptr_copy(ptr);
	return ptr_copy;
}

void Game::start_game(const string& str) {
	std::string game_type = str;
	if (ptr) {
		throw game_already_started;
	}
	/*else if (str.find("FiveCardDraw") >= 12) {
		throw unknown_game;
	}*/
	else if (game_type == "FiveCardDraw") {
		ptr = std::static_pointer_cast<Game>(std::make_shared<FiveCardDraw>());
	}
	else if (game_type == "SevenCardStud") {
		ptr = std::static_pointer_cast<Game>(std::make_shared<SevenCardStud>());
	}
	else {
		ptr = std::static_pointer_cast<Game>(std::make_shared<FiveCardDraw>());
	}
}

void Game::stop_game() {
	if (!ptr) {
		throw no_game_in_progress;
	}
	ptr.reset();
}

shared_ptr<Player> Game::find_player(const string& str) {
	for (shared_ptr<Player> p : players) {
		if (p->name == str) {
			return p;
		}
	}
	shared_ptr<Player> not_found;
	return not_found;
}

void Game::add_player(const string& str) {
	shared_ptr<Player> player_found = find_player(str);
	if (player_found) {
		throw already_playing;
	}
	/*
	else if ((player_found->chips)<=0) { //NEW: player with 0 chips quits the game, then tries to rejoin the game, required to reset their chip count to 20 and keep playing, or not rejoin the game.
		cout << "You don't have any chips left. In order to keep playing you must reset your chips to 20. Would you like to do that? Please enter 'yes' or 'no'." << endl;
		string responseChips;
		cin >> responseChips;
		if (responseChips == "yes" || responseChips == "Yes") {
			player_found->chips = 20;
			players.push_back(make_shared<Player>(str));

		}
		else if (responseChips == "no" || responseChips == "No") {
			cout << "Fine, you won't join the game then" << endl;
			throw not_enough_chips;
		}
	}*/
	else {
		players.push_back(make_shared<Player>(str));
	}
}

int Game::size() const {
	return (this->players).size();
}

void Game::remove_player(const string& str) {
	for (std::vector<std::shared_ptr<Player>>::iterator i = players.begin(); i != players.end(); ++i) {
		if ((*i)->name == str) {
			players.erase(i);
			return;
		}
	}
}

bool Game::compareHand(std::shared_ptr<Player> p1, std::shared_ptr<Player> p2) {
	if (!p1) {
		return false;
	}
	if (!p2) {
		return true;
	}
	return poker_rank(p1->hand, p2->hand);
}

bool Game::players_same_hands(shared_ptr<Player> p1, shared_ptr<Player> p2) {
	if (!p1) {
		return false;
	}
	if (!p2) {
		return true;
	}
	return equivalent_hands(p1->hand, p2->hand);
}
//Game::~Game() {
//	//Doesn't do anything
//}