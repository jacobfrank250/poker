#include "stdafx.h"
#include "functions.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

Card::Card(card_suit s, card_rank r) :
	suit(s), rank(r), faceDown(false) {}

card_suit getSuit(int sInt) {
	switch (sInt) {
	case 0: return clubs;
	case 1: return diamonds;
	case 2: return hearts;
	case 3: return spades;
	default: return invalidSuit;
	}
}

card_rank getRank(int rInt) {
	switch (rInt) {
	case 2: return two;
	case 3: return three;
	case 4: return four;
	case 5: return five;
	case 6: return six;
	case 7: return seven;
	case 8: return eight;
	case 9: return nine;
	case 10: return ten; //so that ten can be represented by a char
	case 11: return jack;
	case 12: return queen;
	case 13: return king;
	case 14: return ace;
	default: return invalidRank;
	}
}

const char* enum_suit_strings[] = { "C", "D", "H", "S", "invalidSuit", 0 };

const int enum_suit_ints[] = { 1, 2, 3, 4, 0 };

const char* enum_rank_strings[] = { "2", "3", "4", "5", "6", "7", "8", "9",
"10", "J", "Q", "K", "A", "invalidRank", 0 };

const int enum_rank_ints[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0 }; //Will this work?

bool Card::operator< (const Card & card) const {
	card_suit cs = card.suit;
	card_rank cr = card.rank;
	return ((enum_rank_ints[rank] < enum_rank_ints[cr]) || ((enum_rank_ints[rank] == enum_rank_ints[cr]) && (enum_suit_ints[suit] < enum_suit_ints[cs])));
}

Card parseString(string card_string) {
	char rChar;
	char sChar;
	if (card_string.at(0) == '1') { //to check if rank is  10
		rChar = '0'; //uses '0' instead of '10' because var is a char
		sChar = card_string.at(2);
	}
	else {
		rChar = card_string.at(0);
		sChar = card_string.at(1);
	}
	card_suit s = getSuit(sChar);
	card_rank r = getRank(rChar);
	Card card = Card(s, r);
	return card;
}

char* handRank(const vector<Card> & hand) {
	//Get all cards of sorted hand
	Card c1 = hand.at(0);
	Card c2 = hand.at(1);
	Card c3 = hand.at(2);
	Card c4 = hand.at(3);
	Card c5 = hand.at(4);
	//Get rank of each card in int form
	int c1R = enum_rank_ints[c1.rank];
	int c2R = enum_rank_ints[c2.rank];
	int c3R = enum_rank_ints[c3.rank];
	int c4R = enum_rank_ints[c4.rank];
	int c5R = enum_rank_ints[c5.rank];
	//Get suit of each card in string form
	const char* c1S = enum_suit_strings[c1.suit];
	const char* c2S = enum_suit_strings[c2.suit];
	const char* c3S = enum_suit_strings[c3.suit];
	const char* c4S = enum_suit_strings[c4.suit];
	const char* c5S = enum_suit_strings[c5.suit];

	//5 of a Kind -> No Rank as this is for one deck
	if (c1R == c5R) {
		return "No Rank";
	}
	//4 of a Kind
	else if ((c1R == c4R) || (c2R == c5R)) {
		return "Four of a Kind";
	}
	//Full House
	else if (((c1R == c3R) && (c4R == c5R)) || ((c3R == c5R) && (c1R == c2R))) {
		return "Full House";
	}
	//Flush
	else if (((c1S == c2S) && (c1S == c3S) && (c1S == c4S) && (c1S == c5S)) &&
		!((c1R + 4 == c5R) || (c1R == 2 && c2R == 3 && c3R == 4 && c4R == 5 && c5R == 14))
		) {
		return "Flush";
	}
	//3 of a Kind
	else if ((c1R == c3R) ||
		((c1R == c2R) && (c1R == c4R)) ||
		((c1R == c2R) && (c1R == c5R)) ||
		((c1R == c3R) && (c1R == c4R)) ||
		((c1R == c3R) && (c1R == c5R)) ||
		((c1R == c4R) && (c1R == c5R)) ||
		(c2R == c4R) ||
		((c2R == c3R) && (c2R == c5R)) ||
		((c2R == c4R) && (c2R == c5R)) ||
		(c3R == c5R)
		) {
		return "Three of a Kind";
	}
	//Two Pairs
	else if (((c1R == c2R) && (c3R == c4R)) || ((c1R == c2R) && (c4R == c5R)) || ((c2R == c3R) && (c4R == c5R))) {
		return "Two Pairs";
	}
	//One Pair
	else if ((c1R == c2R) || (c2R == c3R) || (c3R == c4R) || (c4R == c5R)) {
		return "One Pair";
	}
	//Straight Flush
	else if (((c1S == c2S) && (c1S == c3S) && (c1S == c4S) && (c1S == c5S)) &&
		((c1R + 4 == c5R) || (c1R == 2 && c2R == 3 && c3R == 4 && c4R == 5 && c5R == 14))
		) {
		return "Straight Flush";
	}
	//Straight
	else if ((c1R + 4 == c5R) || (c1R == 2 && c2R == 3 && c3R == 4 && c4R == 5 && c5R == 14)) {
		return "Straight";
	}
	//No Rank
	else {
		return "No Rank";
	}
}

int printHand(const vector<Card> & cards) {
	int size = cards.size() / 5;
	for (int i = 0; i < size; ++i) {
		vector<Card> hand;
		//push back each group of five cards into hand vector
		hand.push_back(cards.at(5 * i + 0));
		hand.push_back(cards.at(5 * i + 1));
		hand.push_back(cards.at(5 * i + 2));
		hand.push_back(cards.at(5 * i + 3));
		hand.push_back(cards.at(5 * i + 4));

		sort(hand.begin(), hand.end()); //sort vector using overloaded < operator
		char* hand_rank = handRank(hand); //call handRank on hand to determine what rank the hand is
		try {
			cout << hand_rank << endl; //print rank of hand
		}
		catch (int e) { //if there is some error above, print_error is returned
			cout << "Print Error: " << e << endl;
			return print_error;
		}
	}
	return success;
}

int printCards(const vector<Card> & cards) {
	cout << "Try Print" << endl;
	try {
		for (Card c : cards) {
			//use enum_rank_strings so that enum text is printed instead of enum numbers
			cout << enum_rank_strings[c.rank] << " of " << enum_suit_strings[c.suit] << endl;
		}
		return success;
	}
	catch (int e) { //if there is some error above, print_error is returned
		cout << "Print Error: " << e << endl;
		return print_error;
	}
}

int usageMessage(const char* programName, const char * usageMessage) {
	// print out a helpful usage message telling the user how to run the program correctly
	cout << usageMessage << endl;
	return usage_error;
}