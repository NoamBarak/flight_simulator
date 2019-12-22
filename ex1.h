//
// Created by noam on 10/11/2019.
//

#include <iostream>
#include "Interpreter.h"
#ifndef PROJECT_EXPRESSION_H
#define PROJECT_EXPRESSION_H

using namespace std;

/**
 * Expression Interface
 */
class Expression {

public:
    virtual double calculate() = 0;

    virtual ~Expression() {}
};


class Value : public Expression {
protected:
    double value;

public:
    Value(double value);

    double calculate();
};

class UnaryOperator : public Expression {
protected:
    Expression *e;
    UnaryOperator (Expression *e);

public:
    virtual double calculate() = 0;
};

class BinaryOperator : public Expression {
protected:
    Expression* e1;
    Expression* e2;

public:
    virtual double calculate() = 0;
    BinaryOperator(Expression* left,Expression* right);

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


#endif //PROJECT_EXPRESSION_H