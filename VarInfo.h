//
// Created by karin on 03/01/2020.
//

#ifndef FLIGHT_SIMULATOR_VARINFO_H
#define FLIGHT_SIMULATOR_VARINFO_H

#include "flightsim.h"
class VarInfo {
private:
    string name;
    double value;
    string sim;
    string direction;
public:
    VarInfo(string name1, string sim1, string dir);

    VarInfo(string name1);

    string getSim() {
        return this->sim;
    }

    double getValue() {
        return this->value;
    }

    void setValue(double value1);
};
#endif //FLIGHT_SIMULATOR_VARINFO_H
