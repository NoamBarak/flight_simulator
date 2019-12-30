#include <iostream>
#include "Command.h"
#include "flightsim.h"
#include "VarInfo.cpp"
#include "Lexer.cpp"
#include "Commands.cpp"
#include "ex1.h"
#include "ex1.cpp"

unordered_map<string, VarInfo> toClient;
unordered_map<string, VarInfo> fromServer;
queue<string> updateOrder;
Interpreter interpreter = Interpreter();
bool firstVarInput = false;
bool done = false;

int main() {
    // Initializing a thread array
    thread threads[2];
    // Initializing the <string,Command*> map
    unordered_map<string, Command *> map;
    // Adding the commands to the map
    OpenServerCommand openServerCommand = OpenServerCommand(threads);
    Command *openServerCommand2 = &openServerCommand;
    map.emplace(std::make_pair("openDataServer", openServerCommand2));
    ConnectCommand connectCommand = ConnectCommand(threads);
    Command *connectCommand2 = &connectCommand;
    map.emplace(std::make_pair("connectControlClient", connectCommand2));
    DefineVarCommand defineVarCommand = DefineVarCommand();
    Command *defineVarCommand2 = &defineVarCommand;
    map.emplace(std::make_pair("var", defineVarCommand2));
    SleepCommand sleepCommand = SleepCommand();
    Command *sleepCommand2 = &sleepCommand;
    map.emplace(std::make_pair("Sleep", sleepCommand2));
    PrintCommand printCommand = PrintCommand();
    Command *printCommand2 = &printCommand;
    map.emplace(std::make_pair("Print", printCommand2));
    WhileCommand whileCommand = WhileCommand(map);
    Command *whileCommand2 = &whileCommand;
    map.emplace(std::make_pair("while", whileCommand2));
    IfCommand ifCommand = IfCommand(map);
    Command *ifCommand2 = &ifCommand;
    map.emplace(std::make_pair("if", ifCommand2));

    // Initializing the vector
    vector<string> fileVector;
    fileVector = lexer();
    // Starting to interpret and execute
    parser(map, fileVector);
    done = true;
    // Waiting for the threads to close and join the main correctly
    threads[0].join();
    threads[1].join();
    return 0;
}