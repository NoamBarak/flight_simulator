//
// Created by karin on 23/12/2019.
//
//
// Created by noam on 11/11/2019.
//

#ifndef UNTITLED1_INTERPRETER_H
#define UNTITLED1_INTERPRETER_H

#include <iostream>
#include "ex1.h"
#include <vector>
#include <queue>
#include "map"

class Interpreter {
private:
    map<string, double> varNval;
public:
    void setVariables(string str);

    Expression *interpret(string st);

    int findPlace(string st, int index);

    double evaluate(queue<string> tokens);

    int precedence(char op);

    bool isOp(char ch);

    double unaryCalc(double num, string op);

    double binaryCalc(double num1, double num2, string op);

    bool checkParen(string st);

    bool isNumber(const string &s);
};


#endif //UNTITLED1_INTERPRETER_H