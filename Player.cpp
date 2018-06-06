#include "stdafx.h"
#include "functions.h"
#include "deck.h"
#include "hand.h"
#include "player.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Player::Player(std::string n) {
	name = n;
	handsWon = 0;
	handsLost = 0;
	chips = 20; //NEW
	chips_bet = 0; //NEW: this is chips bet by the player in the round
	move = "";
	hand = Hand();
	string fileName = n + ".txt";
	string line;
	ifstream ifs(fileName);
	bool wExtracted = false;
	bool lExtracted = false;
	bool chipsExtracted = false;

	if (ifs.is_open()) {
		while (getline(ifs, line)) {
			char p = line.at(0);
			string number = "";
			if (line.length() > 1) {
				for (unsigned int i = 1; i < line.length(); i++) {
					char temp_num = line.at(i);
					number = number + temp_num;
				}
			}
			if (p == 'W') {
				handsWon = stoi(number);
				wExtracted = true;
			}
			else if (p == 'L') {
				handsLost = stoi(number);
				lExtracted = true;
			}
			else if (p == 'C') { //NEW
				chips = stoi(number);
				chipsExtracted = true;
			}
		}
		if (!wExtracted || !lExtracted) {
			handsWon = 0;
			handsLost = 0;
		}
	}
}

ostream& operator<< (ostream& out, const Player& p) {
	out << "Player: " << p.name << " Wins: " << p.handsWon << " Losses: " << p.handsLost << " Chips: "<<p.chips; //new
	return out;
}
