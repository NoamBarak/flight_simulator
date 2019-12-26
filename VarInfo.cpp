//
// Created by karin on 24/12/2019.
//
#include "flightsim.h"

extern queue<string> updateOrder;
extern Interpreter interpreter;

class VarInfo {
private:
    string name;
    double value;
    string sim;
    string direction;
public:
    VarInfo(string name1, string sim1, string dir) {
        // var name ->\-< sim add
        this->name = name1;
        this->sim = sim1;
        this->direction = dir;
    }
    VarInfo(string name1){
        // var name = val
        this->name = name1;
    }

    string getSim() {
        return this->sim;
    }

    void setValue(double value1) {
        this->value = value1;
        string varAsString = std::to_string(value1);
        string varAssign = this->name + "=" + varAsString;
        interpreter.setVariables(varAssign);
        // if the direction is -> we need to update the simulator
        if (this->direction == "->") {
            updateOrder.push(this->name);
        }
    }
};
