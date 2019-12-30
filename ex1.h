//
// Created by karin on 08/11/2019.
//

#ifndef EX1_EX1_H
#define EX1_EX1_H
using namespace std;

#include <stack>
#include <iostream>
#include <queue>
#include <cstring>
#include <string>
#include <map>

/**
 * Expression Interface
 */
class Expression {

public:
    virtual double calculate() = 0;

    virtual ~Expression() {}
};

// Value
class Value : public Expression {
private:
    double value;
public:
    Value(double val);

    double calculate() override;

    //~Value() override;
};

class Variable : public Expression {
public:
    string name;
    double val;

    Variable(string str, double val1);

    string getString();

    double getVal();

    double calculate() override;

    //~Variable();
    Variable &operator++();

    Variable &operator--();

    Variable &operator++(int);

    Variable &operator--(int);
};


// Binary Operator
class BinaryOperator : public Expression {
private:
    Expression *right;
    Expression *left;

public:
    BinaryOperator(Expression *l, Expression *r);

    Expression *getLeft();

    Expression *getRight();

    double calculate() override;

    //~BinaryOperator();

};

// Plus
class Plus : public BinaryOperator {
public:
    Plus(Expression *l1, Expression *r1) : BinaryOperator(l1, r1) {}

    double calculate() override;
};

// Minus
class Minus : public BinaryOperator {
public:
    Minus(Expression *l1, Expression *r1) : BinaryOperator(l1, r1) {}

    double calculate() override;
};

// Mul
class Mul : public BinaryOperator {
public:
    Mul(Expression *l1, Expression *r1) : BinaryOperator(l1, r1) {}

    double calculate() override;
};

// Div
class Div : public BinaryOperator {
public:
    Div(Expression *l1, Expression *r1) : BinaryOperator(l1, r1) {}

    double calculate() override;
};

// Unary
class UnaryOperator : public Expression {
private:
    Expression *exp;
public:
    UnaryOperator(Expression *e1);

    Expression *getExpression();
    //~UnaryOperator();
};

class UPlus : public UnaryOperator {
public:
    UPlus(Expression *e) : UnaryOperator(e) {}

    double calculate() override;
};

class UMinus : public UnaryOperator {
public:
    UMinus(Expression *e) : UnaryOperator(e) {}

    double calculate() override;
};

class Interpreter {
private:
    map<std::string, double> variables;
public:
    map<string, double> getVariables();

    void addVariable(string str);

    Expression *interpret(string equation);

    int precedence(char c);

    bool isOperator(char c);

    bool isPolishOperator(string c);

    Expression *reversePolish(queue<string> queue);

    void setVariables(string string1);
};

#endif //EX1_EX1_H