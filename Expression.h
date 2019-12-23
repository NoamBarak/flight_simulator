//
// Created by noam on 22/12/2019.
//

#ifndef ADVANCEDEX3_EXPRESSION_H
#define ADVANCEDEX3_EXPRESSION_H


using namespace std;
/**
 * Expression Interface
 */
class Expression {

public:
    virtual double calculate() = 0;
    virtual ~Expression() {}
};

#endif //ADVANCEDEX3_EXPRESSION_H
