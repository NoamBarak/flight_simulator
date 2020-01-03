//
// Created by karin on 24/12/2019.
//
#include "flightsim.h"

extern queue<string> updateOrder;
extern Interpreter interpreter;

VarInfo::VarInfo(string name1, string sim1, string dir) {
    this->name = name1;
    this->sim = sim1;
    this->direction = dir;
}

VarInfo::VarInfo(string name1) {
    // var name = val
    this->name = name1;
}

void VarInfo::setValue(double value1) {
    this->value = value1;
        // Each time the value changes it is initialized/updated in the interpreter
        string varAsString = std::to_string(value1);
        string varAssign = this->name + "=" + varAsString;
        interpreter.setVariables(varAssign);
        // if the direction is -> we need to update the simulator
        if (this->direction == "->") {
            updateOrder.push(this->name);
        }
}
/*

class VarInfo {
private:
    string name;
    double value;
    string sim;
    string direction;
public:
    VarInfo(string name1, string sim1, string dir) {
        this->name = name1;
        this->sim = sim1;
        this->direction = dir;
    }

    VarInfo(string name1) {
        // var name = val
        this->name = name1;
    }

    string getSim() {
        return this->sim;
    }

    double getValue() {
        return this->value;
    }

    void setValue(double value1) {
        this->value = value1;
if(this->name=="alt"){
            cout<<"alt "<< this->getValue()<<endl;
        }

        // Each time the value changes it is initialized/updated in the interpreter
        string varAsString = std::to_string(value1);
        string varAssign = this->name + "=" + varAsString;
        interpreter.setVariables(varAssign);
        // if the direction is -> we need to update the simulator
        if (this->direction == "->") {
            updateOrder.push(this->name);
        }
    }
};
*/
