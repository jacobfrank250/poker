#include "stdafx.h"
#include "deck.h"
#include "functions.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <iterator>

using namespace std;

Deck::Deck()
	: deck_vector()
{}

Deck::Deck(char * filename)
	: deck_vector()
{
	int load_status = load(filename);
	if (load_status != success) {
		cout << "File could not be opened" << endl;
		throw load_status;
	}
}

int Deck::load(char * filename) {
	ifstream in(filename);
	if (in.is_open()) {
		string localLine;
		while (getline(in, localLine)) {
			istringstream iss(localLine);
			bool empty = true; //keeps track of empty lines, allowowing program to skip lines without printing out "too many" or "too few"
			string cardString;
			while (iss >> cardString) {
				if (cardString.length() == 2 || cardString.length() == 3) { //make sure string is right length
					empty = false;
					Card card = parseString(cardString);
					if (card.suit != invalidSuit && card.rank != invalidRank) { //make sure ranks are between 2-Ace and suits one of the four
						deck_vector.push_back(card); //push back string to temporary vector hand
					}
					else { cout << "String not well-formed" << endl; }
				}
				else { cout << "String not well-formed" << endl; }
			}
		}
		return success;
	}
	return file_open_error;
}

int Deck::shuffle() {
	unsigned seed = (int)std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(deck_vector.begin(), deck_vector.end(), std::default_random_engine(seed));
	return success;
}

int Deck::size() const {
	return deck_vector.size();
}

int Deck::add_card(Card c) {
	deck_vector.push_back(c);
	return success;
}

ostream& operator<< (ostream& o, const Deck& deck) {
	for (Card c : deck.deck_vector) {
		//use enum_rank_strings so that enum text is printed instead of enum numbers
		o << enum_rank_strings[c.rank] << " of " << enum_suit_strings[c.suit] << endl;
	}
	return o;
}

void Deck::getCardsFromDeck(Deck& other_deck) {
	std::copy(other_deck.deck_vector.begin(), other_deck.deck_vector.end(), std::back_inserter(this->deck_vector));
	other_deck.deck_vector.erase(other_deck.deck_vector.begin(), other_deck.deck_vector.end());
}