#include <cstdlib>
#include <set>
#include <list>
#include <string>
#include <iterator>
using namespace std;
//UTILITY CLASSES//
#include "Context.h"
#include "InputHandler.h"
//Game Helper.
#include "Dealer.h"
#include "Player.h"
//CONST UTILITY
Context context;
InputHandler input;

void MainMenu();
void Rules();
void NewGame();
void GameLoop();
void PlayerTurn(Dealer*);
void ComputerTurn(Dealer*, int);
void GameOver(Dealer*);
void DisplayBooks(Player*);
void DisplayHand(Player*);
void Wait();

int main(int argc, char** argv) {
    srand(static_cast<unsigned int> (time(0)));
    context.LoadCardNames("cardnames.txt");
    context.LoadCardDisplay("carddisplay.txt");
    context.LoadDialogue("dialogue.txt");
    context.LoadCardLookup("cardlookup.txt");
    MainMenu();
    return 0;
}

void MainMenu() {
    bool end_game = false;
    bool title = true;
    do {
        if (title) {
            cout << context.GetDialogue("title") << endl;
            title = false;
        }
        cout << context.GetDialogue("choice");
        char choice = input.GetLetter();
        switch (choice) {
            case 'A':
                // New Game
                NewGame();
                title = true;
                break;
            case 'B':
                // Rules
                Rules();
                title = true;
                break;
            case 'C':
                // Exit
                end_game = true;
                break;
            default:
                cout << context.GetDialogue("invalid_title_range") << endl;
        }
    } while (end_game == false);
};

void Rules() {
    cout << context.GetDialogue("rules") << endl;
    Wait();
};

void NewGame() {
    GameLoop();
    Wait();
};

void GameLoop() {
    int turn = 0;
    Dealer* dealer = new Dealer();
    dealer->AddPlayers(4);
    dealer->ServeCards();
    bool game_valid = true;
    do {
        game_valid = false;
        for (int i = 0; i < dealer->PlayerCount(); i++) {
            if (dealer->PlayerPlaying(i)) {
                game_valid = true;
                if (i == 0) {
                    PlayerTurn(dealer);
                } else {
                    ComputerTurn(dealer, i);
                }
                turn++;
            }
        }
    } while (game_valid);
    GameOver(dealer);
    delete dealer;
};

void PlayerTurn(Dealer* dealer) {
    cout << context.GetDialogue("yourturn") << endl;
    bool in_play = true;
    if (dealer->PlayerHandEmpty(0)) {
        cout << context.GetDialogue("emptyhand") << endl;
        int drawn_card = dealer->PlayerDraws(0);
        if (drawn_card == -1) {
            cout << context.GetDialogue("emptydeck") << endl;
            cout << context.GetDialogue("player_disqualified") << endl;
            in_play = false;
        } else {
            cout << context.GetDialogue("drawcard");
            cout << context.GetCardDisplay(drawn_card) << endl;
        }
    }
    if (in_play) {
        Player* player = dealer->GetPlayer(0);
        cout << context.GetDialogue("player_book");
        cout << endl;
        DisplayBooks(player);
        cout << endl;
        cout << context.GetDialogue("hand");
        cout << endl;
        DisplayHand(player);
        cout << endl;
        cout << context.GetDialogue("gofish_prompt") << endl;
        list<int> players;
        string list_prompt = "";
        for (int i = 1; i < dealer->PlayerCount(); i++) {
            if (!dealer->PlayerHandEmpty(i)) {
                list_prompt += 'A' + (i - 1);
                list_prompt += ") ";
                list_prompt += context.GetDialogue("player") + ' ';
                list_prompt += '1' + i;
                list_prompt += '\n';
                players.push_back(i);
            }
        }
        char choice = 0;
        while (choice == 0) {
            cout << list_prompt << endl;
            choice = input.GetLetter();
            if (choice < 'A' || choice > 'A' + players.size() - 1) {
                cout << context.GetDialogue("invalidinput") << endl;
                cout << context.GetDialogue("gofish_prompt_invalid") << endl;
                choice = 0;
            }
        }
        auto itr = players.begin();
        advance(itr, (choice - 'A'));
        int target = *itr;
        int card = -1;
        while (card == -1) {
            cout << context.GetDialogue("askcard") << endl;
            card = context.GetCardType(input.GetLastWord());
            if (card == -1) {
                cout << context.GetDialogue("invalidinput") << endl;
            }
        }
        int cards_grabbed = dealer->GoFish(0, target, card);
        if (cards_grabbed != 0) {
            cout << context.GetDialogue("card_taken") << ' ';
            cout << cards_grabbed << ' ';
            cout << context.GetCardName(card) << endl;
        } else {
            cout << context.GetDialogue("gofish") << endl;
            int card_drawn = dealer->PlayerDraws(0);
            if (card_drawn != -1) {
                cout << context.GetDialogue("drawcard") << ' ';
                cout << context.GetCardName(card_drawn) << endl;
            } else {
                cout << context.GetDialogue("emptydeck") << endl;
                cout << context.GetDialogue("player_disqualified") << endl;
            }
        }
    }
};

void ComputerTurn(Dealer* dealer, int player) {
    bool in_play = true;
    if (dealer->PlayerHandEmpty(player)) {
        int drawn_card = dealer->PlayerDraws(player);
        if (drawn_card == -1) {
            cout << context.GetDialogue("player") << ' ';
            cout << player + 1;
            cout << context.GetDialogue("disqualified") << endl;
            in_play = false;
        }
    }
    if (in_play) {
        cout << context.GetDialogue("player") << ' ';
        cout << player + 1;
        cout << context.GetDialogue("turn") << endl;
        list<int> players;
        for (int i = 0; i < dealer->PlayerCount(); i++) {
            if (!dealer->PlayerHandEmpty(i) && i != player) {
                players.push_back(i);
            }
        }
        auto itr = players.begin();
        advance(itr, rand() % players.size());
        int target = *itr;
        int card = 0;
        list<int> cards;
        Player* this_player = dealer->GetPlayer(player);
        int count;
        do {
            count = this_player->CountCards(card);
            if (count > 0)
                cards.push_back(card);
            card++;
        } while (count != -1);
        itr = cards.begin();
        advance(itr, rand() % cards.size());
        card = *itr;
        int cards_grabbed = dealer->GoFish(player, target, card);
        cout << context.GetDialogue("player") << ' ';
        cout << player + 1 << ' ';
        cout << context.GetDialogue("asks") << ' ';
        cout << context.GetDialogue("player") << ' ';
        cout << target + 1 << ' ';
        cout << context.GetDialogue("for") << ' ';
        cout << context.GetCardName(card) << endl;

        if (cards_grabbed != 0) {
            cout << context.GetDialogue("player") << ' ';
            cout << player + 1 << ' ';
            cout << context.GetDialogue("takes") << ' ';
            cout << context.GetCardName(card) << endl;
        } else {
            cout << context.GetDialogue("gofish") << endl;
            int card_drawn = dealer->PlayerDraws(player);
            if (card_drawn != -1) {
                cout << context.GetDialogue("player") << ' ';
                cout << player + 1 << ' ';
                cout << context.GetDialogue("drawcard_computer") << endl;
            } else {
                cout << context.GetDialogue("emptydeck") << endl;
                cout << context.GetDialogue("player") << ' ';
                cout << player + 1 << ' ';
                cout << context.GetDialogue("disqualified") << endl;
            }
        }
    }
};

void GameOver(Dealer* dealer) {
    set<int> highscore_players;
    int highscore_books = -1;
    for (int i = 0; i < dealer->PlayerCount(); i++) {
        int books = dealer->GetPlayer(i)->Books()->size();
        ;
        if (highscore_books == books) {
            highscore_players.insert(i);
        } else if (highscore_books < books) {
            highscore_players.clear();
            highscore_players.insert(i);
            highscore_books = books;
        }
        cout << context.GetDialogue("player") << ' ';
        cout << i + 1;
        cout << " Has ";
        cout << books;
        cout << ' ';
        cout << context.GetDialogue("book");
        cout << endl;
    }
    if (highscore_players.size() == 1) {
        cout << context.GetDialogue("player") << ' ';
        cout << *highscore_players.begin() + 1 << ' ';
        cout << context.GetDialogue("mostbooks") << '!' << endl;
        cout << context.GetDialogue("player");
        cout << ' ';
        cout << context.GetDialogue("winner") << endl;
    } else {
        auto itr = highscore_players.begin();
        while (itr != highscore_players.end()) {
            cout << context.GetDialogue("player") << ' ';
            cout << *itr + 1;
            itr++;
            if (itr != highscore_players.end())
                cout << ',';
        }
        cout << ' ';
        cout << context.GetDialogue("tied") << ' ';
        cout << highscore_books << '!' << endl;
    }
};

void DisplayBooks(Player* player) {
    set<int>* bookset = player->Books();
    auto itr = bookset->begin();
    while (itr != bookset->end()) {
        cout << context.GetCardDisplay(*itr);
        itr++;
    }
}

void DisplayHand(Player* player) {
    int card = 0;
    int count;
    do {
        count = player->CountCards(card);
        for (int i = 0; i < count; i++) {
            cout << context.GetCardDisplay(card);
        }
        card++;
    } while (count != -1);
}

void Wait() {
    cout << context.GetDialogue("wait") << endl;
    input.GetLetter();
};