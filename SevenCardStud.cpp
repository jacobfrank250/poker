#include "stdafx.h"
#include "deck.h"
#include "functions.h"
#include "hand.h"
#include "game.h"
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

SevenCardStud::SevenCardStud()
	: dealer(0), commonChipPot(0), current_bet(0) //commonChipPot & current_bet are NEW
{
	for (int i = 2; i <= 14; i++) {
		for (int j = 0; j < 4; j++) {
			card_rank c_rank = getRank(i);
			card_suit c_suit = getSuit(j);
			Card c = Card(c_suit, c_rank);
			(this->main_deck).add_card(c);
		}
	}
}


bool bet_on_table = false;
//NEW: BETTING PHASE
void SevenCardStud::betting_phase(Player &p) {
	bool bet_on_table = false; //TEMP
	bool validResponse_CB = false;
	bool validResponse_FCR = false;
	//bool playerDone = false;
	cout << endl;
	cout << p << endl;
	cout << p.hand;
	for (size_t j = 0; j < (this->players).size(); ++j) {
		if (players[j]->move == "bet" || players[j]->move == "raise") {
			bet_on_table = true;
		}
	}
	//cout << "Bet On Table: " << bet_on_table << endl;

	if (p.chips == 0) {
		cout << "This player has no more chips, let em be let em be, let em be let em be sing a song of freedom let em be bum bum bum" << endl;
		p.move = "no money pls";
	}
	else if (!bet_on_table) { // players may either check or bet 1-2 chips
		while (!validResponse_CB) {
			cout << p.name << ": Would you like to 'check' or 'bet'?" << endl;
			string checkOrBet;
			cin >> checkOrBet;
			if (checkOrBet == "check" || checkOrBet == "Check") {
				validResponse_CB = true;
				p.move = "check";
				//playerDone = true;
				//return;
			}
			else if (checkOrBet == "bet" || checkOrBet == "Bet") {
				validResponse_CB = true;
				bool validBet = false;
				while (!validBet) {
					cout << p.name << ": Would you like to bet '1' or '2' chips?" << endl;
					string betAmount;
					cin >> betAmount;
					if (stoi(betAmount) == 1) {
						validBet = true;
						p.move = "bet";
						--p.chips; //take chip from player's chips
						++commonChipPot; //add that chip to POT
						++p.chips_bet; //to keep track of how many chips were bet

						bet_on_table = true; //WILL MATTER WHEN WE MAKE GLOBAL ?!?!?! XD
						++current_bet;
					}
					else if (stoi(betAmount) == 2) { //they waged a bet of 2
						cout << "bet amount: " << betAmount << endl;
						if (p.chips == 1) { //the player has exactly one chip so we will say invalid bet which will prompt them again to bet or check
							cout << "You only have one chip so you cannot bet 2" << endl;
							//validBet = false;
						}
						else { //the player has 2 or more chips so they can bet 2 chips
							validBet = true;
							p.move = "bet";
							p.chips -= 2; //takes 2 chips from player
							commonChipPot += 2; //adds those chips to chipPotle
							p.chips_bet += 2; //keep track of current chips bet in round
							bet_on_table = true;
							current_bet += 2;
						}
					}
					else { // the user entered a bet amount other than '1' or '2' so it's invalid
						cout << "Invalid Bet Amount" << endl;
						//validBet = false;
					}
				}
			}
			else {
				cout << "invalid response!" << endl;
				//validResponse_CB = false;
			}
		}
	}
	else { //There is a bet placed on the table so the player can either FOLD, CALL, or RAISE
		while (!validResponse_FCR) {
			cout << p.name << ": Would you like to 'fold', 'call', or 'raise'?" << endl; //Add call amount?
			string FoldCallOrRaise;
			cin >> FoldCallOrRaise;
			if (FoldCallOrRaise == "fold" || FoldCallOrRaise == "Fold") {
				validResponse_FCR = true;
				p.move = "fold";
				//playerDone = true;
				//commonChipPot += p.chips_bet;
				//p.folded = true; //????
				return;
			}
			else if (FoldCallOrRaise == "call" || FoldCallOrRaise == "Call") {
				validResponse_FCR = true;
				p.move = "call";
				//check if player has enough money
				unsigned int call_amount = current_bet - p.chips_bet;
				if (p.chips < call_amount) {
					commonChipPot += p.chips;
					p.chips_bet += p.chips;
					p.chips = 0; //dangerous move here hard coding it to zero my b guys. CHECK ME OUT WHEN DEBUGGING
					std::cout << "You don't have enough money to call, you're going ALL IN" << std::endl;
					//playerDone = true;
					return;
				}
				else if (p.chips == call_amount) { //player has same amount of chips as current bet
					commonChipPot += call_amount;
					p.chips_bet += call_amount;
					p.chips -= call_amount; //this should be zero as they will have bet the last of their chips
					cout << "You're going ALL IN BUDDY!" << endl;
				}
				else { //They have more money than current bet
					commonChipPot += call_amount;
					p.chips -= call_amount;
					p.chips_bet += call_amount;
				}
			}
			else if (FoldCallOrRaise == "raise" || FoldCallOrRaise == "Raise") {
				if (p.chips <= current_bet - p.chips_bet) { //If the player doesnt have more than the current bet they cannot raise
					cout << "Sorry you actually don't have enough chips to raise-- either fold or call" << endl;
					//validResponse_FCR = false;
				}
				else if (p.chips == current_bet - p.chips_bet + 1) { //in this case the player can only raise by one
					cout << "You can only raise by 1 chip...raising by one chip" << endl;
					--p.chips; //take chip from p.chips
					++commonChipPot; //add it to commonChipotle
					++p.chips_bet;
					++current_bet;
					validResponse_FCR = true;
					p.move = "raise";
				}
				else {
					cout << "How many chips would you like to raise? '1' or '2'?" << endl;
					string raiseAmount;
					cin >> raiseAmount;
					if (stoi(raiseAmount) == 1) {
						--p.chips; //take chip from player's chips
						++commonChipPot; //add that chip to potle
						++p.chips_bet;
						++current_bet;
						validResponse_FCR = true;
						p.move = "raise";
					}
					else if (stoi(raiseAmount) == 2) {
						p.chips -= 2; //take 2 chips from player's chips
						commonChipPot += 2; //add them to chipPotle
						p.chips_bet += 2;
						current_bet += 2;
						validResponse_FCR = true;
						p.move = "raise";
					}
					else {
						cout << "invalid response. Raise by a value of '1' or '2'" << endl;
						//validResponse_FCR = false;
					}
				}
			}
			else {
				cout << "invalid response" << endl;
				validResponse_FCR = false;
			}
		}
	}
}

int SevenCardStud::betting_round() {
	int start = (this->dealer) + 1;
	int num_players = (this->players).size();
	if ((this->dealer) == num_players - 1) {
		start = 0;
	}
	//for (size_t j = 0; j < (this->players).size(); ++j) {
	size_t j = 0;
	int last_to_raise = -1; //to keep track of index of last person who raised
	int number_folded = 0;
	bool round_over = false;
	while (!round_over) {
		cout << "Current Chip Pot: " << commonChipPot << endl;
		/*for (size_t j = 0; j < (this->players).size(); ++j) {
		cout << players[j]->name << " move is: " << players[j]->move << endl;
		}*/
		int position = (start + j) % (this->players).size();
		if (number_folded == num_players - 1) { //if everyone folded
			players.at(position)->chips += commonChipPot; //award them entire pot
			++players.at(position)->handsWon; //increment win count
			round_over = true; //end round
		}
		/*cout << "Last to Raise: " << last_to_raise << endl;
		cout << "Position: " << position << endl;
		cout << "Start position: " << (start) % (this->players).size() << endl;
		cout << "Previous move: " << players.at(position)->move << endl;*/
		if (position == last_to_raise) { //if everyone called
			round_over = true;
			cout << "EVERYONE CALLED!" << endl;
		}
		//if everyone checked
		else if (last_to_raise == -1 && position == ((start) % (this->players).size()) && players.at(position)->move == "check") {
			round_over = true;
		}
		else if (players.at(position)->move != "fold") {
			betting_phase(*players.at(position));
			string player_move = players.at(position)->move;
			if (player_move == "bet" || player_move == "raise") {
				last_to_raise = position;
			}
			if (player_move == "fold") {
				++number_folded;
			}
		}
		++j;
	}
	cout << "BETTING ROUND OVER!!!" << endl;
	//after_round();
	//clear p.move for all p if they didnt fold
	for (size_t i = 0; i < players.size(); ++i) {
		if (players.at(i)->move != "fold") {
			players.at(i)->move = "";
		}
	}
	return success;
}

int SevenCardStud::before_turn(Player & p) {
	//	int cardsToDiscard = 0;
	//	cout << endl;
	//	cout << p << endl;
	//	cout << p.hand;
	//	bool validResponse = false;
	//	while (!validResponse) {
	//		cout << "How many cards would you like to discard?" << endl;
	//		string input;
	//		cin >> input;
	//		try {
	//			if (0 <= stoi(input) && stoi(input) <= 5) {
	//				validResponse = true;
	//				cardsToDiscard = stoi(input);
	//			}
	//			else {
	//				cout << "Please enter a number from 0 to 5" << endl;
	//			}
	//		}
	//		catch (std::invalid_argument& e) {
	//			// if no conversion could be performed
	//			cout << "Please enter a numerical answer (0, 1, 2, 3, 4, 5) Error: " << e.what() << endl;
	//		}
	//		catch (std::out_of_range& e) {
	//			// if the converted value would fall out of the range of the result type
	//			// or if the underlying function (std::strtol or std::strtoull) sets errno
	//			// to ERANGE.
	//			cout << "Number is out of range of int capacity. Error: " << e.what() << endl;
	//		}
	//	}
	//	while (cardsToDiscard > 0) {
	//		cout << "Which card index would you like to discard?" << endl;
	//		cout << p.hand;
	//		string discard;
	//		cin >> discard;
	//		try {
	//			Card toDiscard = p.hand[stoi(discard)];
	//			discardDeck.add_card(toDiscard);
	//			p.hand.remove_card(stoi(discard));
	//			cardsToDiscard--;
	//		}
	//		catch (std::invalid_argument& e) {
	//			// if no conversion could be performed
	//			cout << "Please enter a numerical answer (0, 1, 2, ...). Error: " << e.what() << endl;
	//		}
	//		catch (std::out_of_range& e) {
	//			// if the converted value would fall out of the range of the result type
	//			// or if the underlying function (std::strtol or std::strtoull) sets errno
	//			// to ERANGE.
	//			cout << "Number is out of range of int capacity. Error: " << e.what() << endl;
	//		}
	//		catch (int e) {
	//			switch (e) {
	//			case erase_out_of_bounds:
	//				std::cout << "tried erasing out of bounds!" << std::endl;
	//				break;
	//			case access_out_of_bounds:
	//				std::cout << "tried accessing out of bounds!" << std::endl;
	//				break;
	//			default:
	//				std::cout << "try again" << std::endl;
	//			}
	//		}
	//		catch (...) {
	//			cout << "try again" << endl;
	//		}
	//	}
	return success;
}

int SevenCardStud::turn7(Player& p, int turn) {
	//int cards_needed = 5 - p.hand.size();
	//while (cards_needed > 0) {
	int main_deck_size = (this->main_deck).size(); //store main deck size
	if (main_deck_size == 0) { //check if main deck is empty
		if ((this->discardDeck).size() == 0) { //check if discard deck is empty
			cout << "not enough cards" << endl;
			return not_enough_cards;
		}
		(this->discardDeck).shuffle(); //shuffle discard deck
		p.hand << (this->discardDeck); //deal cards from discard deck
	}
	p.hand << (this->main_deck); //deal cards from main deck
	if (turn == 4) { //TURN IS 4 BECAUSE THEY ARE DELT 4 CARDS IN A GAME AND THE LAST ONE IS FACE DOWN
		p.hand[6].faceDown = true;
	}
	//	--cards_needed;
	//}


	return success;
}

int SevenCardStud::after_turn(Player& p) {
	for (size_t i = 0; i < players.size(); i++) {

		//cout << endl << "Player Name: " << p.name << endl;
		//cout << players.size() << endl;
		if (players.at(i)->name == p.name) {
			//cout << "should print player hand if they have one" << endl;
<<<<<<< HEAD
			cout <<p.name<<" heres is your hand: "<<p.hand << endl;
=======
			cout <<p.name<<" here is your hand: "<< p.hand << endl;
>>>>>>> origin/Jakob
		}
		else {
			string handString;
			//cout << "is the hand empty perhaps? hand size: " << players.at(i)->hand.size() << endl;
			for (int j = 0; j < players.at(i)->hand.size(); j++) {
				Card c = players.at(i)->hand[j];
				if (j != 0) {
					handString += " ";
				}
				if (c.faceDown) { //IF THE CARD IS FACE DOWN THEN WE PRINT A *
					handString += "*";
				}
				else {
					handString += enum_rank_strings[c.rank];
					handString += enum_suit_strings[c.suit];
				}
			}
			//cout << "boutta try printing the hand wish me luck XD hehehehe" << endl;
<<<<<<< HEAD
			cout << players.at(i)->name<<"'s hand: "<<handString << endl;
=======
			cout << players.at(i)->name<<"'s hand: " handString << endl;
>>>>>>> origin/Jakob
		}
		//return success;
	}
	//cout << "going to return success" << endl;
	return success;
}

int SevenCardStud::before_round() {

	for (size_t i = 0; i < players.size(); ++i) { //New: remove chip from each player before round and add to common chip pot
		--players.at(i)->chips;
		++commonChipPot;
		players.at(i)->move = "";
	}

	(this->main_deck).shuffle(); //shuffle main deck
	int start = (this->dealer) + 1;
	int num_players = (this->players).size();
	if ((this->dealer) == num_players - 1) {
		start = 0;
	}
	for (int i = 0; i < 3; ++i) {
		for (size_t j = 0; j < (this->players).size(); ++j) {
			int position = (start + j) % (this->players).size();
			players[position]->hand << (this->main_deck);
			if (i < 2) { //The first two cards we set facedown to true
				(players[position]->hand)[i].faceDown = true;
			}
			else {
				//players[position]->hand[i].faceDown = false; //cuckiewar will prolly change this because its set to false in the constructor
			}
		}
	}
	//dont need stuff below because in fivecard draw before turn asked if they want to get rid of any cards.you dont do this in 7 card willis
	/*for (size_t j = 0; j < (this->players).size(); ++j) {
	int position = (start + j) % (this->players).size();
	this->before_turn(*players[position]);
	}*/
	return success;
}

int SevenCardStud::round() {
	int start = (this->dealer) + 1;
	int num_players = (this->players).size();
	if ((this->dealer) == num_players - 1) {
		start = 0;
	}
	betting_round();
	for (int i = 1; i <= 4; ++i) {
		for (size_t j = 0; j < (this->players).size(); ++j) {
			int position = (start + j) % (this->players).size();
			int turn_result = this->turn7(*players[position], i); //players are delt a card here
			if (turn_result != success) {
				//cout << "gona return tur_result" << endl;
				return turn_result;
			}
			//cout << "Instantiating after_turn_result...idk why tho" << endl;
			int after_turn_result = this->after_turn(*players[position]); //this prints the player and every ones hands for each player in the game
		}
		//cout << "new betting round chea" << endl;
		betting_round();
		//after_round();
	}
	return success;
}

bool compareHand(std::shared_ptr<Player> p1, std::shared_ptr<Player> p2) {
	if (!p1) {
		return false;
	}
	if (!p2) {
		return true;
	}
	return poker_rank(p1->hand, p2->hand);
}

bool players_same_hands(shared_ptr<Player> p1, shared_ptr<Player> p2) {
	if (!p1) {
		return false;
	}
	if (!p2) {
		return true;
	}
	return equivalent_hands(p1->hand, p2->hand);
}

int SevenCardStud::after_round() {
	cout << "after_round" << endl;
	std::vector<std::shared_ptr<Player>> temp_players(players);
	cout << "temp players size: " << temp_players.size() << endl;
	for (size_t i = 0; i < temp_players.size(); ++i) {
		/*Hand temp_hand = temp_players.at(i)->hand;
		Hand new_hand = sevenChooseFive(temp_hand);*/
		cout << "We in here??" << endl;
		//temp_players.at(i)->hand = temp_players.at(i)->hand.sevenChooseFive();
		Hand new_hand = temp_players.at(i)->hand.sevenChooseFive();
		cout << "YAY" << endl;
		cout << new_hand << endl;
		temp_players.at(i)->hand = new_hand;
		cout << "temp_players.at(i)->hand = new_hand;" << endl;
		cout << temp_players.at(i)->hand << endl;
	}
	cout << "We out here!" << endl;
	std::sort(temp_players.begin(), temp_players.end(), compareHand);

	for (size_t i = 0; i < temp_players.size(); i++) {
		if (players.at(i)->move != "fold") {
			if (i == 0) {
				++(temp_players.at(i)->handsWon);
			}
			else {
				++(temp_players[i]->handsLost);
			}
			cout << temp_players.at(i)->name << endl;
			cout << temp_players.at(i)->handsWon << " hands won" << endl;
			cout << temp_players.at(i)->handsLost << " hands lost" << endl;
			cout << temp_players.at(i)->chips << " chips" << endl;
			cout << "current hand" << endl;
			cout << temp_players.at(i)->hand << endl;
		}
	}
	for (size_t i = 0; i < temp_players.size(); i++) {
		if (players.at(i)->move == "fold") {
			++(temp_players[i]->handsLost);
			cout << temp_players.at(i)->name << endl;
			cout << temp_players.at(i)->handsWon << " hands won" << endl;
			cout << temp_players.at(i)->handsLost << " hands lost" << endl;
			cout << temp_players.at(i)->chips << " chips" << endl;
			cout << "current hand" << endl;
			cout << temp_players.at(i)->hand << endl;
		}
	}

	for (size_t i = 0; i < temp_players.size(); ++i) {
		for (int j = (temp_players.at(i))->hand.size() - 1; j >= 0; --j) {
			Card toMove = (temp_players.at(i)->hand)[j];
			(this->main_deck).add_card(toMove);
			(temp_players.at(i))->hand.remove_card(j);
		}
	}

	main_deck.getCardsFromDeck(discardDeck);
	bool leaveGame = false;

	//NEW. AFTER ROUND CHECK PLAYERS CHIPS AND ASK IF THEY WANT TO RESET OR LEAVE
	for (size_t i = 0; i < temp_players.size(); i++) {
		if (temp_players.at(i)->chips <= 0) {
			cout << "You don't have any chips left. In order to keep playing you must reset your chips to 20. Would you like to do that? Please enter 'yes' or 'no'." << endl;
			string responseChips;
			cin >> responseChips;
			if (responseChips == "yes" || responseChips == "Yes") {
				//reset chips
				temp_players.at(i)->chips = 20;
			}
			else if (responseChips == "no" || responseChips == "No") {
				leaveGame = true;
			}
		}

	}
	while (!leaveGame) {
		cout << "Do any players want to leave the game? Please enter 'yes' or 'no'." << endl;
		string responseLeave;
		cin >> responseLeave;
		if (responseLeave == "yes" || responseLeave == "Yes") {
			cout << "Which player wants to leave? Please enter the name of departing player." << endl;
			string responseName;
			cin >> responseName;
			shared_ptr<Player> player = find_player(responseName);

			if (player) {
				//save player to ofstream
				string fileName = player->name + ".txt";
				ofstream playerFile(fileName, ios::trunc);
				if (playerFile.is_open())
				{
					playerFile << "W" << player->handsWon << "\n";
					playerFile << "L" << player->handsLost << "\n";
					playerFile << "C" << player->chips << "\n"; //new
					playerFile.close();
				}
				remove_player(responseName); //remove player
			}
			else {
				cout << "That player is not currently at the table" << endl;
			}
		}
		else if (responseLeave == "no" || responseLeave == "No") {
			leaveGame = true;
		}
		if (players.size() == 0) {
			break;
		}

	}
	bool joinGame = true;
	while (joinGame) {
		cout << "Do any players want to join the game? Please enter 'yes' or 'no'." << endl;
		string responseJoin;
		cin >> responseJoin;
		if (responseJoin == "no" || responseJoin == "No") {
			joinGame = false;
		}
		else if (responseJoin == "yes" || responseJoin == "Yes") {
			cout << "Which player wants to join? Please enter the name of joining player." << endl;
			string responseNameJoin;
			cin >> responseNameJoin;
			try {
				add_player(responseNameJoin);
			}
			catch (outcome e) {
				if (e == already_playing) {
					cout << "Player already in game. Please enter another response." << endl;
				}
			}
		}

	}

	if (players.size() > 0) {
		dealer = (dealer + 1) % players.size();
	}
	return success;
}
int SevenCardStud::turn5(Player& p) {
	//do nothing
	return 0;
}
