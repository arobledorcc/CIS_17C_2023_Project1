#ifndef DECK_H
#define DECK_H

#include <cstdlib>
#include <stack>
#include <list>
#include <iterator>

using namespace std;

class Deck {
public:

    Deck() {
    };

    virtual ~Deck() {
    };

    int TakeCard();
    void SetupCards(int, int);
    bool IsEmpty();
private:
    stack<int> cards;
};

int Deck::TakeCard() {
    int card = -1;
    if (!cards.empty()) {
        card = cards.top();
        cards.pop();
    }
    return card;
}

void Deck::SetupCards(int count, int book_size) {
    list<int> temp_cards;
    for (int i = 0; i < count; i++) {
        for (int a = 0; a < book_size; a++)
            temp_cards.push_back(i);
    }
    //Shuffle them!
    int size = count*book_size;
    while (size != 0) {
        auto itr = temp_cards.begin();
        advance(itr, rand() % size);
        cards.push(*itr);
        temp_cards.erase(itr);
        size--;
    }
}

bool Deck::IsEmpty() {
    return cards.size() == 0;
};

#endif /* DECK_H */

