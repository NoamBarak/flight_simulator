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
        cout << "setValue here 1 VALUE: "<< value1 << endl;
        this->value = value1;
        cout << "setValue here 2 NAME: "<< this->name << endl;
        string varAsString = std::to_string(value1);
        cout << "setValue here 3: " << endl;
        string varAssign = this->name + "=" + varAsString;
        cout << "setValue here 4: "<< varAssign << endl;
        interpreter.setVariables(varAssign);
        cout << "setValue here 5: " << endl;
        // if the direction is -> we need to update the simulator
        if (this->direction == "->") {
            cout << "setValue here 6: " << endl;
            updateOrder.push(this->name);
            cout << "setValue here 7: " << endl;
            cout<<"Pushed to queue"<< this->name <<endl;
        }
    }
};
