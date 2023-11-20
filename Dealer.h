#ifndef DEALER_H
#define DEALER_H

#include <list>
#include <iterator>

using namespace std;

#include "Deck.h"
#include "Player.h"

class Dealer {
public:

    Dealer() {

    };

    Dealer(const Dealer& orig) {
    };

    ~Dealer() {
        auto itr = players.begin();
        while (itr != players.end()) {
            delete *itr;
            itr++;
        }
    };
    void AddPlayers(int count);
    int PlayerCount();
    void ServeCards();
    bool PlayerPlaying(int player);
    bool PlayerHandEmpty(int player);
    int GoFish(int a, int b, int card);
    int PlayerDraws(int player);
    bool CheckGameFinished();

    //REFERENCE FUNCTIONS FOR DISPLAY PURPOSES//
    Player* GetPlayer(int player);
private:
    Deck deck;
    list<Player*> players;
};

void Dealer::AddPlayers(int count) {
    for (int i = 0; i < count; i++) {
        Player* player = new Player();
        players.push_back(player);
    }
};

int Dealer::PlayerCount() {
    return players.size();
};

//Serve each player 5 cards.

void Dealer::ServeCards() {
    deck.SetupCards(13, 4);
    auto itr = players.begin();
    while (itr != players.end()) {
        for (int i = 0; i < 5; i++) {
            Player* player = *itr;
            player->GiveCards(deck.TakeCard(), 1);
        }
        itr++;
    }
};

//Check if player is playing

bool Dealer::PlayerPlaying(int a) {
    auto itr = players.begin();
    advance(itr, a);
    Player* player = *itr;
    return !(player->HandEmpty() && deck.IsEmpty());
};

bool Dealer::PlayerHandEmpty(int a) {
    auto itr = players.begin();
    advance(itr, a);
    Player* player = *itr;
    return player->HandEmpty();
}

//Perform Go Fish, Return cards stolen.

int Dealer::GoFish(int a, int b, int card) {
    auto itr_a = players.begin();
    auto itr_b = players.begin();
    advance(itr_a, a);
    advance(itr_b, b);
    Player* player_a = *itr_a;
    Player* player_b = *itr_b;
    int count = player_b->TakeCards(card);
    if (count != 0) {
        player_a->GiveCards(card, count);
    }
    return count;
};

//Player Draws Card.

int Dealer::PlayerDraws(int player) {
    int card = -1;
    if (!deck.IsEmpty()) {
        card = deck.TakeCard();
        auto itr = players.begin();
        advance(itr, player);
        Player* player = *itr;
        player->GiveCards(card, 1);
    }
    return card;
};

//Check if the game is still going on.

bool Dealer::CheckGameFinished() {
    bool gamevalid = false;
    auto itr = players.begin();
    while (itr != players.end()) {
        Player* player = *itr;
        if (!(player->HandEmpty() && deck.IsEmpty())) {
            gamevalid = true;
            break;
        }
        itr++;
    }
    return gamevalid;
}

//REFERENCE FUNCTIONS FOR DISPLAY PURPOSES//

Player* Dealer::GetPlayer(int a) {
    auto itr = players.begin();
    advance(itr, a);
    Player* player = *itr;
    return player;
}
#endif /* DEALER_H */

