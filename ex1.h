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
protected:
    double value;

public:
    Value(double value);

    double calculate();

    //~Value() override;
};

class Variable : public Expression {
protected:
    string var;
    double value;
public:
    double calculate();
    Variable(string var, double value);

    Variable &operator++();

    Variable &operator++(int);

    Variable &operator--();

    Variable &operator--(int);

    Variable &operator+=(double num);

    Variable &operator-=(double num);

};

// Binary Operator
class BinaryOperator : public Expression {
protected:
    Expression* e1;
    Expression* e2;

public:
    virtual double calculate() = 0;
    BinaryOperator(Expression* left,Expression* right);

};

class Plus : public BinaryOperator {
public:
    Plus(Expression *e1, Expression *e2);
    double calculate();

};

class Minus : public BinaryOperator {
public:
    Minus(Expression *e1, Expression *e2);
    double calculate();
};

class Mul : public BinaryOperator {
public:
    Mul (Expression *e1, Expression *e2);
    double calculate();

};

class Div : public BinaryOperator {
public:
    Div (Expression *e1, Expression *e2);
    double calculate();

};

// Unary
class UnaryOperator : public Expression {
protected:
    Expression *e;
    UnaryOperator (Expression *e);

public:
    virtual double calculate() = 0;
};

class UPlus : public UnaryOperator {
public:
    UPlus (Expression *e);
    double calculate();

};

class UMinus : public UnaryOperator {
public:
    UMinus (Expression *e);
    double calculate();
};


#endif //EX1_EX1_H