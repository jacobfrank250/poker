#include "stdafx.h"
#include "functions.h"
#include "hand.h"
#include "deck.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

Hand::Hand()
	: hand_vector()
{}

Hand::Hand(const Hand& hand)
	: hand_vector()
{
	vector<Card> other_vector = hand.hand_vector;
	for (Card c : other_vector) {
		this->hand_vector.push_back(c);
	}
}

int Hand::operator= (const Hand& other_hand) {
	if (&other_hand != this) {
		vector<Card> temp_vector;
		vector<Card> other_vector = other_hand.hand_vector;
		for (Card c : other_vector) {
			temp_vector.push_back(c);
		}
		this->hand_vector = temp_vector;
	}
	return success;
}

int Hand::size() const {
	return (this->hand_vector).size();
}

bool Hand::operator== (const Hand& other_hand) const {
	if (this->size() != other_hand.size()) {
		return false;
	}
	bool same = true;
	vector<Card> other_vector = other_hand.hand_vector;
	for (int i = 0; i < this->size(); ++i) {
		Card this_card = (this->hand_vector).at(i);
		Card other_card = other_vector.at(i);
		//Could probably run operator< for cards both ways
		int this_card_rank = enum_rank_ints[this_card.rank];
		int this_card_suit = enum_suit_ints[this_card.suit];
		int other_card_rank = enum_rank_ints[other_card.rank];
		int other_card_suit = enum_suit_ints[other_card.suit];
		if ((this_card_rank != other_card_rank) || (this_card_suit != other_card_suit)) {
			same = false;
		}
	}
	return same;
}

bool Hand::operator< (const Hand& other_hand) const {
	int minSize = this->size();
	if (other_hand.size() < minSize) {
		minSize = other_hand.size();
	}
	bool less = true;
	vector<Card> other_vector = other_hand.hand_vector;
	for (int i = 0; i < minSize; ++i) {
		Card this_card = (this->hand_vector).at(i);
		Card other_card = other_vector.at(i);
		if (this_card < other_card) {
			return true;
		}
		if (other_card < this_card) {
			return false;
		}
	}
	if (other_hand.size() > minSize) {
		return true;
	}
	return false;
}

Card& Hand::operator[](const size_t location) {
	if (location >= 0 && location < hand_vector.size()) {
		return hand_vector.at(location);
	}
	else {
		throw access_out_of_bounds;
	}
}

void Hand::remove_card(const size_t location) {
	if (location >= hand_vector.size() || location < 0) {
		throw erase_out_of_bounds;
	}
	else {
		this->hand_vector.erase(this->hand_vector.begin() + location);
	}
}

string Hand::asString() const {
	string toReturn;
	for (int i = 0; i < this->size(); ++i) {
		Card c = (this->hand_vector).at(i);
		if (i != 0) {
			toReturn += " ";
		}
		//if (c.faceDown) { //IF THE CARD IS FACE DOWN THEN WE PRINT A *
		//	toReturn += "*";
		//}
		//else {
		toReturn += enum_rank_strings[c.rank];
		toReturn += enum_suit_strings[c.suit];
		//}
	}
	return toReturn;
}

ostream& operator<< (ostream& o, const Hand& hand) {
	string hand_string = hand.asString();
	o << hand_string << " " << endl;
	return o;
}

void operator<< (Hand& hand, Deck& deck) {
	vector<Card> dv = deck.deck_vector;
	int dv_size = deck.size();
	Card dv_last = dv.at(dv_size - 1);
	dv.pop_back();
	deck.deck_vector = dv;
	vector<Card> hv = hand.hand_vector;
	hv.push_back(dv_last);
	//sort(hv.begin(), hv.end());
	hand.hand_vector = hv;
}

int handRankInt(char* handRank) {
	if (strcmp(handRank, "Straight Flush") == 0) {
		return 8;
	}
	if (strcmp(handRank, "Four of a Kind") == 0) {
		return 7;
	}
	if (strcmp(handRank, "Full House") == 0) {
		return 6;
	}
	if (strcmp(handRank, "Flush") == 0) {
		return 5;
	}
	if (strcmp(handRank, "Straight") == 0) {
		return 4;
	}
	if (strcmp(handRank, "Three of a Kind") == 0) {
		return 3;
	}
	if (strcmp(handRank, "Two Pairs") == 0) {
		return 2;
	}
	if (strcmp(handRank, "One Pair") == 0) {
		return 1;
	}
	return 0; //No Rank
}

bool poker_rank(const Hand& hand1, const Hand& hand2) {
	int hand1_rank = handRankInt(handRank(hand1.hand_vector));
	int hand2_rank = handRankInt(handRank(hand2.hand_vector));
	vector<Card> hv1 = hand1.hand_vector;
	vector<Card> hv2 = hand2.hand_vector;
	int hv1cr1 = enum_rank_ints[(hv1.at(0)).rank];
	int hv1cr2 = enum_rank_ints[(hv1.at(1)).rank];
	int hv1cr3 = enum_rank_ints[(hv1.at(2)).rank];
	int hv1cr4 = enum_rank_ints[(hv1.at(3)).rank];
	int hv1cr5 = enum_rank_ints[(hv1.at(4)).rank];
	int hv2cr1 = enum_rank_ints[(hv2.at(0)).rank];
	int hv2cr2 = enum_rank_ints[(hv2.at(1)).rank];
	int hv2cr3 = enum_rank_ints[(hv2.at(2)).rank];
	int hv2cr4 = enum_rank_ints[(hv2.at(3)).rank];
	int hv2cr5 = enum_rank_ints[(hv2.at(4)).rank];
	if (hand1_rank == hand2_rank) {
		if (hand1_rank == 5 || hand1_rank == 0) { //No Rank or Flush
			for (int i = 4; i >= 0; --i) {
				if (hv2.at(i) < hv1.at(i)) {
					return true;
				}
				else if (hv1.at(i) < hv2.at(i)) {
					return false;
				}
			}
			return false;
		}
		else if (hand1_rank == 8 || hand1_rank == 4) { //straight flush or straight
			return hv1cr5 > hv2cr5; //compare highest cards
		}
		else if (hand1_rank == 7 || hand1_rank == 6) { //four of a kind or full house
													   //compare ranks of 3rd ranking cards since those are always in 4 of a kind or full house
			return hv1cr3 > hv2cr3;
		}
		else if (hand1_rank == 3) { //three of a kind
			int hv1check = hv1cr1;
			int hv2check = hv2cr1;
			if (hv1cr2 == hv1cr4 || (hv1cr2 == hv1cr3 && hv1cr2 == hv1cr5) || (hv1cr2 == hv1cr4 && hv1cr2 == hv1cr5)) {
				hv1check = hv1cr2;
			}
			else if (hv1cr3 == hv1cr5) {
				hv1check = hv1cr3;
			}
			if (hv2cr2 == hv2cr4 || (hv2cr2 == hv2cr3 && hv2cr2 == hv2cr5) || (hv2cr2 == hv2cr4 && hv2cr2 == hv2cr5)) {
				hv2check = hv2cr2;
			}
			else if (hv2cr3 == hv2cr5) {
				hv2check = hv2cr3;
			}
			return hv1check > hv2check;
		}
		else if (hand1_rank == 2) { //two pairs
			int hv1checkBig = hv1cr4;
			int hv1checkSmall = hv1cr1;
			int hv2checkBig = hv2cr4;
			int hv2checkSmall = hv2cr1;
			if (hv1cr1 == hv1cr2 && hv1cr3 == hv1cr4) {
				hv1checkSmall = hv1cr1;
				hv1checkBig = hv1cr3;
			}
			else if (hv1cr1 == hv1cr2 && hv1cr4 == hv1cr5) {
				hv1checkSmall = hv1cr1;
				hv1checkBig = hv1cr5;
			}
			else if (hv1cr2 == hv1cr3 && hv1cr4 == hv1cr5) {
				hv1checkSmall = hv1cr2;
				hv1checkBig = hv1cr5;
			}
			if (hv2cr1 == hv2cr2 && hv2cr3 == hv2cr4) {
				hv2checkSmall = hv2cr1;
				hv2checkBig = hv2cr3;
			}
			else if (hv2cr1 == hv2cr2 && hv2cr4 == hv2cr5) {
				hv2checkSmall = hv2cr1;
				hv2checkBig = hv2cr5;
			}
			else if (hv2cr2 == hv2cr3 && hv2cr4 == hv2cr5) {
				hv2checkSmall = hv2cr2;
				hv2checkBig = hv2cr5;
			}
			if (hv1checkBig != hv2checkBig) {
				return hv1checkBig > hv2checkBig;
			}
			return hv1checkSmall > hv2checkSmall;
		}
		else if (hand1_rank == 1) { //one pair
			int hv1check = hv1cr2;
			int hv2check = hv2cr2;
			if (hv1cr3 == hv1cr4 || hv1cr4 == hv1cr5) {
				hv1check = hv1cr4;
			}
			if (hv2cr3 == hv2cr4 || hv2cr4 == hv2cr5) {
				hv2check = hv2cr4;
			}
			return hv1check > hv2check;
		}
	}
	return hand1_rank > hand2_rank;
}

bool equivalent_hands(const Hand& hand1, const Hand& hand2) {
	vector<Card> hv1 = hand1.hand_vector;
	sort(hv1.begin(), hv1.end());
	vector<Card> hv2 = hand2.hand_vector;
	sort(hv2.begin(), hv2.end());
	int hv1cr1 = enum_rank_ints[(hv1.at(0)).rank];
	int hv1cr2 = enum_rank_ints[(hv1.at(1)).rank];
	int hv1cr3 = enum_rank_ints[(hv1.at(2)).rank];
	int hv1cr4 = enum_rank_ints[(hv1.at(3)).rank];
	int hv1cr5 = enum_rank_ints[(hv1.at(4)).rank];
	int hv2cr1 = enum_rank_ints[(hv2.at(0)).rank];
	int hv2cr2 = enum_rank_ints[(hv2.at(1)).rank];
	int hv2cr3 = enum_rank_ints[(hv2.at(2)).rank];
	int hv2cr4 = enum_rank_ints[(hv2.at(3)).rank];
	int hv2cr5 = enum_rank_ints[(hv2.at(4)).rank];
	if (hv1cr1 != hv2cr1 || hv1cr2 != hv2cr2 || hv1cr3 != hv2cr3 || hv1cr4 != hv2cr4 || hv1cr5 != hv2cr5) {
		return false;
	}
	return true;
}

//THIS IS MADE FOR SEVEN CARD STUD
Hand Hand::sevenChooseFive() {
	std::cout << "first line of sevenChooseFive" << std::endl;
	sort((this->hand_vector).begin(), (this->hand_vector).end());
	std::cout << "sorted the hand" << std::endl;
	Hand original_hand = *this;
	Hand best_hand;
	cout << "original hand: " << endl;
	cout << original_hand;
	for (int i = 0; i < 6; ++i) {
		std::cout << "entered first 4loop" << std::endl;
		for (int j = i + 1; j < 7; ++j) {
			std::cout << "entered second 4 loop" << endl;
			cout << "i: " << i << " j: " << j << endl;
			int card_index1 = 0;
			if (card_index1 == i || card_index1 == j) {
				++card_index1;
				if (card_index1 == j) {
					++card_index1;
				}
			}
			int card_index2 = card_index1 + 1;
			if (card_index2 == i || card_index2 == j) {
				++card_index2;
				if (card_index2 == j) {
					++card_index2;
				}
			}
			int card_index3 = card_index2 + 1;
			if (card_index3 == i || card_index3 == j) {
				++card_index3;
				if (card_index3 == j) {
					++card_index3;
				}
			}
			int card_index4 = card_index3 + 1;
			if (card_index3 == i || card_index4 == j) {
				++card_index4;
				if (card_index4 == j) {
					++card_index4;
				}
			}
			int card_index5 = card_index4 + 1;
			if (card_index5 == i || card_index5 == j) {
				++card_index5;
				if (card_index5 == j) {
					++card_index5;
				}
			}
			cout << card_index1 << card_index2 << card_index3 << card_index4 << card_index5 << endl;
			Card card1 = (this->hand_vector)[card_index1];
			Card card2 = (this->hand_vector)[card_index2];
			Card card3 = (this->hand_vector)[card_index3];
			Card card4 = (this->hand_vector)[card_index4];
			Card card5 = (this->hand_vector)[card_index5];
			cout << "CARDS MADE BRAH" << endl;
			Hand tempHand;
			tempHand.hand_vector.push_back(card1);
			tempHand.hand_vector.push_back(card2);
			tempHand.hand_vector.push_back(card3);
			tempHand.hand_vector.push_back(card4);
			tempHand.hand_vector.push_back(card5);
			cout << "temp hand made" << endl;
			cout << tempHand;
			if (i == 0 && j == 1) {
				cout << "INITIAL HAND!" << endl;
				best_hand = tempHand;
			}
			else if (poker_rank(tempHand, best_hand)) {
				cout << "iff yee?" << endl;
				best_hand = tempHand;
				cout << "iff yee" << endl;
			}
			cout << "current besty" << endl;
			cout << best_hand << endl;
		}
	}
	cout << "original hand: " << endl;
	cout << original_hand;
	std::cout << "best hand: "<< std::endl;
	std::cout<<best_hand << endl;
	return best_hand;
}
