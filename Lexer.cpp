//
// Created by karin on 25/12/2019.
//

#include "lexer.h"

void Lexer::openDataServerLexer(string line, string checkCom, int i, vector <string> *v1) {
    string port;
    //insert the relevant command into vector
    v1->push_back(checkCom);
    i = i + checkCom.length();
    //insert the value into vector
    //i+1, and line.length()-1-(i+1) because of the parentheses
    port = line.substr(i + 1, line.length() - 1 - (i + 1));
    v1->push_back(port);
}

void Lexer::connectControlClientLexer(string line, string checkCom, vector <string> *v1) {
    string address, value;
    int saveFirst, saveSecond, saveComma;
    int len = line.length();
    saveFirst = line.find_first_of('"');
    saveSecond = line.find_first_of('"', saveFirst + 1);
    saveComma = line.find_first_of(",");
    //insert the relevant command into vector
    v1->push_back(checkCom);
    //insert address into vector
    v1->push_back(line.substr(saveFirst, saveSecond - saveFirst + 1));
    //insert port into vector
    v1->push_back(line.substr(saveComma + 1, len - saveComma - 2));
}

void Lexer::printOrSleepLexer(string line, string checkCom, vector <string> *v1) {
    int len = line.length();
    int firstParen = line.find_first_of("(");
    //insert the relevant command into vector
    v1->push_back(checkCom);
    //insert address into vector
    v1->push_back(line.substr(firstParen + 1, len - firstParen - 2));
}

void Lexer::varLexer(string line, string checkCom, vector <string> *v1, int i) {
    int len = line.length();
    i = i + checkCom.length() + 1;
    int spaceBeforeArrow = line.find_first_of(" ", i);
    int spaceAfterArrow = line.find_first_of(" ", spaceBeforeArrow + 1);
    int arrowOrEq = line.find_first_of("(", spaceAfterArrow + 1);
    string checkIfArrow = line.substr(spaceBeforeArrow + 1, spaceAfterArrow - spaceBeforeArrow - 1);
    //insert the relevant command into vector
    v1->push_back(checkCom);
    //insert name of var into vector
    v1->push_back(line.substr(i, spaceBeforeArrow - i));
    //insert arrow or '=' into vector
    v1->push_back(line.substr(spaceBeforeArrow + 1, spaceAfterArrow - spaceBeforeArrow - 1));
    if (checkIfArrow.find("<") != -1 || checkIfArrow.find(">") != -1) {
        //insert the word "sim"
        v1->push_back(line.substr(spaceAfterArrow + 1, arrowOrEq - spaceAfterArrow - 1));
        //insert address into vector
        v1->push_back(line.substr(arrowOrEq + 1, len - arrowOrEq - 2));
    } else {
        int placeOfEq = line.find_first_of("=");
        int spacePlace = line.find_first_of(" ", placeOfEq);
        //insert the string after '='
        v1->push_back(line.substr(spacePlace + 1));
    }
}

void Lexer::otherCasesLexer(string line, string checkCom, vector <string> *v1) {
    checkCom.length();
    int firstLetter = line.find_first_not_of(" \t");
    int eqSignPlace = line.find_first_of("=");
    //insert name of var into vector
    string str = (line.substr(firstLetter, eqSignPlace - firstLetter));
    char lastPlace = str[str.length() - 1];
    //there is a white space between the variable and '='
    if (lastPlace == ' ') {
        v1->push_back(line.substr(firstLetter, eqSignPlace - firstLetter - 1));
    } else {
        v1->push_back(line.substr(firstLetter, eqSignPlace - firstLetter));
    }
    v1->push_back("=");
    int valueBegin = line.find_first_not_of(" ", eqSignPlace + 1);
    str = line.substr(valueBegin);
    v1->push_back(str);
}
