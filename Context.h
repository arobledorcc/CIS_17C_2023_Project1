#ifndef CONTEXT_H
#define CONTEXT_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <map>

using namespace std;

class Context {
public:

    Context() {
    };

    virtual ~Context() {
    };

    string GetCardName(int);
    string GetCardDisplay(int);
    string GetDialogue(string);
    int GetCardType(string);
    void LoadCardNames(string);
    void LoadCardDisplay(string);
    void LoadDialogue(string);
    void LoadCardLookup(string);
private:
    Context(Context const&);
    void operator=(Context const&);
    map<int, string> cardNames;
    map<int, string> cardDisplay;
    map<string, string> dialogueStrings;
    map<string, int> cardLookup;
    string toUpperCase(string);
    void fileError(string);
};

string Context::GetCardName(int card) {
    return cardNames[card];
}

string Context::GetCardDisplay(int card) {
    return cardDisplay[card];
}

string Context::GetDialogue(string key) {
    return dialogueStrings[key];
}

int Context::GetCardType(string key) {
    string upperkey = toUpperCase(key);
    return cardLookup.count(upperkey) ? cardLookup[upperkey] : -1;
}

void Context::LoadCardNames(string path) {
    ifstream file(path);
    if (!file.is_open())
        fileError(path);
    string input;
    while (getline(file, input)) {
        int card = stoi(input);
        getline(file, input);
        cardNames[card] = input;
    }
    file.close();
}

void Context::LoadCardDisplay(string path) {
    ifstream file(path);
    if (!file.is_open())
        fileError(path);
    string input;
    while (getline(file, input)) {
        int card = stoi(input);
        getline(file, input);
        cardDisplay[card] = input;
    }
    file.close();
}

void Context::LoadDialogue(string path) {
    ifstream file(path);
    if (!file.is_open())
        fileError(path);
    string input;
    string escape;
    getline(file, escape);
    while (getline(file, input)) {
        string name = input;
        string dialogue = "";
        bool reading = true;
        bool firstline = true;
        while (reading) {
            getline(file, input);
            reading = input != escape;
            if (reading) {
                if (!firstline)
                    dialogue += '\n';
                dialogue += input;
                firstline = false;
            }
        }
        dialogueStrings[name] = dialogue;
    }
    file.close();
}

void Context::LoadCardLookup(string path) {
    ifstream file(path);
    if (!file.is_open())
        fileError(path);
    string input;
    string escape;
    getline(file, escape);
    while (getline(file, input)) {
        int card = stoi(input);
        bool reading = true;
        while (reading) {
            getline(file, input);
            reading = input != escape;
            if (reading) {
                cardLookup[toUpperCase(input)] = card;
            }
        }
    }
    file.close();
}

string Context::toUpperCase(string input) {
    string output = input;
    auto it = output.begin();
    while (it != output.end())
        *it++ = toupper(*it);
    return output;
}

void Context::fileError(string path) {
    cout << "ERROR: Failed reading file : " << path << endl;
    exit(EXIT_FAILURE);
}
#endif /* CONTEXT_H */

