#ifndef hand_h
#define hand_h
#include "stdafx.h"
#include "functions.h"
#include "deck.h"
#include <string>
#include <vector>

using namespace std;

class Deck;

class Hand {
public:
	Hand();
	Hand(const Hand& hand);
	// Can use virtual destructor because there is no special destructing that needs to be done here
	int operator= (const Hand& other_hand);
	int size() const;
	bool operator== (const Hand& other_hand) const;
	bool operator< (const Hand& other_hand) const;
	string asString() const;
	Card& operator[](size_t location);
	void remove_card(size_t location);
	friend ostream& operator<< (ostream& o, const Hand& hand);
	friend void operator<< (Hand& hand, Deck& deck);
	friend bool poker_rank(const Hand& hand1, const Hand& hand2);
	friend bool equivalent_hands(const Hand& hand1, const Hand& hand2);
	Hand sevenChooseFive();
private:
	vector<Card> hand_vector;
};

ostream& operator<< (ostream& o, const Hand& hand);
void operator<< (Hand& hand, Deck& deck); //void because doesn't need to return anything
int handRankInt(char* handRank);
bool poker_rank(const Hand& hand1, const Hand& hand2);
bool equivalent_hands(const Hand& hand1, const Hand& hand2);

#endif;
