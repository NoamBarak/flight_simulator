//
// Created by karin on 24/12/2019.
//
class VarInfo {
private:
    double value;
    string sim;
    string direction;
public:
    void setValue(double value1) {
        this->value = value1;
        // if the direction is -> we need to update the simulator
    }
};
