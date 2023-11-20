#ifndef HAND_H
#define HAND_H

#include <cstdlib>
#include <map>

using namespace std;

class Hand {
public:

    Hand() {
        highest_value = -1;
    };

    virtual ~Hand() {
    };

    int CardCount(int card);
    void AddCards(int card, int count);
    int TakeCards(int card);
    bool IsEmpty();
private:
    map<int, int> cards;
    int highest_value;
};

int Hand::CardCount(int card) {
    int count = 0;
    if(cards.size()==0||card>highest_value)
        count = -1;
    else if (cards.count(card)!=0)
        count = cards[card];
    return count;
}

void Hand::AddCards(int card, int count) {
    if(cards.count(card)!=0)
    {
      cards[card] += count;
    }else{
      cards[card] = count;
    }
    if(card>highest_value)
    {
        highest_value = card;
    }
}

int Hand::TakeCards(int card) {
    int count = 0;
    if (cards.count(card)!=0) {
        count = cards[card];
        cards.erase(card);
        if(highest_value == card)
        {
            highest_value = -1;
            auto itr = cards.begin();
            while(itr!=cards.end()){
                if(itr->first > highest_value)
                {
                    highest_value=itr->first;
                }
                itr++;
            }
        }
    }
    return count;
}\

bool Hand::IsEmpty() {
    return cards.begin() == cards.end();
};

#endif /* HAND_H */

