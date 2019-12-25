//
// Created by noam on 24/12/2019.
//

#ifndef ADVANCEDEX3_LEXER_H
#define ADVANCEDEX3_LEXER_H
using namespace std;

#include <string>
#include <vector>

class lexer {
public:
    static void openDataServerLexer(string line, string checkCom, int i, vector<string> *v1);

    static void connectControlClientLexer(string line, string checkCom, vector<string> *v1);

    static void printOrSleepLexer(string line, string checkCom, vector<string> *v1);

    static void varLexer(string line, string checkCom, vector<string> *v1, int i);

    static void otherCasesLexer(string line, string checkCom, vector<string> *v1);
};

#endif //ADVANCEDEX3_LEXER_H
