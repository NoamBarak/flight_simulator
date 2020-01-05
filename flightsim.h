//
// Created by karin on 19/12/2019.
//

#ifndef ADVANCEDE3_FLIGHTSIM_H
#define ADVANCEDE3_FLIGHTSIM_H

#include <thread>
#include "Command.h"
#include <unordered_map>
#include "map"

class SleepCommand : public Command {
public:
    int virtual execute(vector<string> vector, int index) override;
    virtual ~SleepCommand() {}
};

class OpenServerCommand : public Command {
private:
    thread *threads;
public:
    OpenServerCommand(thread *threads1) {
        this->threads = threads1;
    }

    int virtual execute(vector<string> vector, int index) override;
    virtual ~OpenServerCommand() {}
};

class ConnectCommand : public Command {
private:
    thread *threads;
public:
    ConnectCommand(thread *thread1) {
        this->threads = thread1;
    }

    int virtual execute(vector<string> vector, int index) override;
    virtual ~ConnectCommand() {}
};

class DefineVarCommand : public Command {
public:
    int execute(vector<string> vector, int index) override;
    virtual ~DefineVarCommand() {}
};

class PrintCommand : public Command {
public:
    int execute(vector<string> vector, int index) override;
    virtual ~PrintCommand() {}
};

class IfCommand : public Command {
private:
    unordered_map<string, Command *> map;
public:
    IfCommand(unordered_map<string, Command *> map1) {
        this->map = map1;
    }

    int execute(vector<string> vector, int index) override;
    virtual ~IfCommand() {}
};

class WhileCommand : public Command {
private:
    unordered_map<string, Command *> map;
public:
    WhileCommand(unordered_map<string, Command *> map1) {
        this->map = map1;
    }

    int execute(vector<string> vector, int index) override;
    virtual ~WhileCommand() {}
};

class AssignVarCommand : public Command {
public:
    int execute(vector<string> vector, int index) override;
    virtual ~AssignVarCommand() {}
};


#endif //ADVANCEDE3_FLIGHTSIM_H