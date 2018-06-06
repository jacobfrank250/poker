#ifndef functions_H
#define functions_H

#include <string>
#include <vector>

using namespace std;

enum card_suit { clubs, diamonds, hearts, spades, invalidSuit };

extern const char* enum_suit_strings[];

extern const int enum_suit_ints[];

enum card_rank { two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace, invalidRank };

extern const char* enum_rank_strings[];

extern const int enum_rank_ints[];

card_suit getSuit(int sInt);

card_rank getRank(int rInt);

struct Card {
	Card(card_suit s, card_rank r);
	bool operator<  (const Card &) const;
	card_suit suit;
	card_rank rank;
	bool faceDown; //NEW FOR SEVEN CARD LARRY
};

enum outcome {
	success = 0,
	file_open_error = 1,
	none_well_formed = 2,
	print_error = 3,
	usage_error = 4,
	no_arguments = 5,
	just_shuffle = 6,
	need_shuffle = 7,
	too_many_arguments = 8,
	instance_not_available = 9,
	game_already_started = 10,
	unknown_game = 11,
	no_game_in_progress = 12,
	already_playing = 13,
	not_enough_cards = 14,
	access_out_of_bounds = 15,
	erase_out_of_bounds = 16,
	too_few_arguments = 17,
	not_enough_chips = 18 //New
};

Card parseString(string card_string);

char* handRank(const vector<Card> & hand);

int printHand(const vector<Card> & cards);

int printCards(const vector<Card> & cards);

int usageMessage(const char* programName, const char * usageMessage);

#endif