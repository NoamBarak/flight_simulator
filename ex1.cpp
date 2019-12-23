//
// Created by karin on 23/12/2019.
//

//
// Created by noam on 10/11/2019.
//

#include <string>
#include "ex1.h"

//Constructors
Variable::Variable(string var1, double value1) {
    this->var = var1;
    this->value = value1;
}

Value::Value(double value1) {
    this->value = value1;
}

BinaryOperator::BinaryOperator(Expression *e10, Expression *e20) {
    this->e1 = e10;
    this->e2 = e20;
}

UnaryOperator::UnaryOperator(Expression *e1) {
    this->e = e1;
}

Plus::Plus(Expression *e10, Expression *e20) : BinaryOperator(e10, e20) {
}

Minus::Minus(Expression *e10, Expression *e20) : BinaryOperator(e10, e20) {
}

Mul::Mul(Expression *e10, Expression *e20) : BinaryOperator(e10, e20) {
}

Div::Div(Expression *e10, Expression *e20) : BinaryOperator(e10, e20) {
}

UPlus::UPlus(Expression *e1) : UnaryOperator(e1) {

}

UMinus::UMinus(Expression *e1) : UnaryOperator(e1) {

}

//Overloads
Variable &Variable::operator++() {
    // this->value = ++this->value;
    // return *this;
    double temp = this->value;
    ++temp;
    this->value = temp;
    return *this;
}

Variable &Variable::operator++(int) {
    double temp = this->value;
    ++temp;
    this->value = temp;
    return *this;
}

Variable &Variable::operator--() {
    //  this->value = --this->value;
    double temp = this->value;
    --temp;
    this->value = temp;
    return *this;
}

Variable &Variable::operator--(int) {
    double temp = this->value;
    --temp;
    this->value = temp;
    return *this;
}

Variable &Variable::operator+=(double num) {
    this->value = (this->value) + num;
    return *this;
}

Variable &Variable::operator-=(double num) {
    this->value = (this->value) - num;
    return *this;
}

//Calculates
double Variable::calculate() {
    return this->value;
}

double Value::calculate() {
    return this->value;
}

double Plus::calculate() {
    return (this->e1->calculate() + this->e2->calculate());
}

double Minus::calculate() {
    return (this->e1->calculate() - this->e2->calculate());
}

double Mul::calculate() {
    return (this->e1->calculate() * this->e2->calculate());
}

double Div::calculate() {
    if (this->e2->calculate() == 0)
        throw ("division by zero");
    return (this->e1->calculate() / this->e2->calculate());
}

double UPlus::calculate() {
    return (this->e->calculate());
}

double UMinus::calculate() {
    return ((this->e->calculate()) * (-1));
}