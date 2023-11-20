#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <cctype>

using namespace std;

class InputHandler {
public:

    InputHandler() {
    };

    virtual ~InputHandler() {
    };
    string GetInput();
    char GetLetter();
    string GetLastWord();
private:
    string toUpperCase(string);
};

string InputHandler::GetInput() {
    string input;
    getline(cin, input);
    return input;
}

char InputHandler::GetLetter() {
    string input;
    getline(cin, input);
    return toupper(input[0]);
}

string InputHandler::GetLastWord() {
    string input;
    getline(cin, input);
    int last_space = input.find_last_of(' ');
    int last_valid = input.size() - 1;
    while (last_valid > 0) {
        char check = input[last_valid];
        if (isalnum(check))
            break;
        else
            last_valid--;
    }
    string word = input.substr(last_space + 1, last_valid - last_space);
    return word;
}

string InputHandler::toUpperCase(string input) {
    string output = input;
    auto it = output.begin();
    while (it != output.end())
        *it++ = toupper(*it);
    return output;
}
#endif /* INPUTHANDLER_H */

