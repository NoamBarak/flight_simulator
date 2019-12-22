//
// Created by karin on 19/12/2019.
//

#ifndef ADVANCEDE3_FLIGHTSIM_H
#define ADVANCEDE3_FLIGHTSIM_H

#include "Command.h"
#include <thread>
class SleepCommand : public Command {
public:
    int virtual execute(vector<string> vector,int index) override;
};
class OpenServerCommand : public Command {
private:
    thread* threads;
public:
    OpenServerCommand(thread* threads1){
        this->threads = threads1;
    }
    int virtual execute(vector<string> vector,int index) override ;
};

class ConnectCommand : public Command {
public:
    int virtual execute(vector<string> vector,int index)  override ;
};

class DefineVarCommand : public Command {
public:
    int execute(vector<string> vector,int index)  override ;
};

class PrintCommand : public Command {
public:
    int execute(vector<string> vector,int index)  override ;
};
#endif //ADVANCEDE3_FLIGHTSIM_H
