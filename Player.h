#ifndef PLAYER_H
#define PLAYER_H

#include <set>
#include <queue>

using namespace std;

#include "Hand.h"

class Player {
public:

    Player() {
        books = new set<int>();
        newbooks = new queue<int>();
    };

    ~Player() {
        delete books;
        delete newbooks;
    };
    int CountCards(int card);
    int GiveCards(int card, int count);
    int TakeCards(int card);
    bool HandEmpty();
    bool HasNewBook();
    set<int>* Books();
    queue<int>* NewBooks();
private:
    Hand hand;
    set<int>* books;
    queue<int>* newbooks;
};

int Player::CountCards(int card) {
    return hand.CardCount(card);
};

int Player::GiveCards(int card, int count) {
    int book = -1;
    hand.AddCards(card, count);
    if (hand.CardCount(card) == 4) {
        TakeCards(card);
        books->insert(card);
        newbooks->push(card);
        book = card;
    }
    return book;
};

int Player::TakeCards(int card) {
    return hand.TakeCards(card);
};

bool Player::HandEmpty() {
    return hand.IsEmpty();
};

bool Player::HasNewBook() {
    return newbooks->size() > 0;
}

set<int>* Player::Books() {
    return books;
};

queue<int>* Player::NewBooks() {
    return newbooks;
};
#endif /* PLAYER_H */

