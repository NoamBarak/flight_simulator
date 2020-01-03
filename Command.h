#ifndef FLIGHTSIM_COMMAND_H
#define FLIGHTSIM_COMMAND_H
#include <iostream>
#include <vector>

using namespace std;
/**
 * Command Interface
 */
class  Command {

public:
    int virtual  execute (vector<string> vector,int index) {
        cout<<"Base execute"<< vector[index]<<endl;
        return 0;
    };
    virtual ~Command() {}
};

#endif //FLIGHTSIM_COMMAND_Hf