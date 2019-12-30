//
// Created by karin on 19/12/2019.
//

#ifndef ADVANCEDE3_FLIGHTSIM_H
#define ADVANCEDE3_FLIGHTSIM_H

#include "Command.h"
#include "ex1.h"
#include <thread>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include "map"

class SleepCommand : public Command {
public:
    int virtual execute(vector<string> vector, int index, bool onlyIndex) override;
};

class OpenServerCommand : public Command {
private:
    thread *threads;
public:
    OpenServerCommand(thread *threads1) {
        this->threads = threads1;
    }

    int virtual execute(vector<string> vector, int index, bool onlyIndex) override;
};

class ConnectCommand : public Command {
private:
    thread *threads;
public:
    ConnectCommand(thread *thread1) {
        this->threads = thread1;
    }

    int virtual execute(vector<string> vector, int index, bool onlyIndex) override;
};

class DefineVarCommand : public Command {
public:
    int execute(vector<string> vector, int index, bool onlyIndex) override;
};

class PrintCommand : public Command {
public:
    int execute(vector<string> vector, int index, bool onlyIndex) override;
};

class IfCommand : public Command {
private:
    unordered_map<string, Command *> map;
public:
    IfCommand(unordered_map<string, Command *> map1) {
        this->map = map1;
    }
    int execute(vector<string> vector, int index, bool onlyIndex) override;
};

class WhileCommand : public Command {
private:
    unordered_map<string, Command *> map;
public:
    WhileCommand(unordered_map<string, Command *> map1) {
        this->map = map1;
    }
    int execute(vector<string> vector, int index, bool onlyIndex) override;
};

class AssignVarCommand : public Command {
public:
    int execute(vector<string> vector, int index, bool onlyIndex) override;
};

#endif //ADVANCEDE3_FLIGHTSIM_H