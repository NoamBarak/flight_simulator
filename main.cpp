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


    Command *c = map.at("while");
    //c->execute(fileVector,0);
    //std::cout << "here 0 : " << typeid(c).name() << '\n';

    //int k = c->execute(fileVector, 1);
    //   (map.at("Sleep"))->execute(fileVector, 1);
    //   (map.at("var"))->execute(fileVector, 1);
    //   (map.at("openDataServer"))->execute(fileVector, 0);
    //(map.at("Sleeping"))->execute(fileVector,0);
    // Noam Testing line 287


    cout << "---------------NOAM--------------- " << endl;

    parser(map, fileVector);

    /*cout << "---------------Karin--------------- " << endl;
    this_thread::sleep_for(std::chrono::microseconds(8000));
    cout << "to client map (->):" << endl;
    for (auto &it: toClient) {
        // Do stuff
        //cout << "\tVar name :" << it.first <<endl;
        // ההדפסה בשורה הבאה מסוכנת, יכולה לעשות שגיאה - תלוי בזמן ריצה של הת'רדים
        cout << "\tVar name :" << it.first << ", Val: " << it.second.getValue() << endl;
    }
    cout << "from server map (<-):" << endl;
    for (auto &it: fromServer) {
        // Do stuff
        cout << "\tVar name :" << it.first << ", Val: " << it.second.getValue() << endl;
    }
    cout << "Interpreter map Variables: " << endl;
    for (auto &it: interpreter.getVariables()) {
        cout << "\tName: " << it.first << ", Val: " << it.second << endl;
    }
    // Print Check for the queue
    cout << "Queue Check:" << endl;
    while (!updateOrder.empty()) {
        cout << "\t " << updateOrder.front() << ", Val: " << toClient.at(updateOrder.front()).getValue() << endl;
        updateOrder.pop();
    }*/





























    // End of Noam Testing till 321

    // Karin Testing line 323

    //OpenServerCommand *openServerCommand1 = new OpenServerCommand;
    //i += openServerCommand1->execute(fileVector, i);






























    // End of Karin Testing 357

    done = true;
    threads[0].join();
    threads[1].join();
    return 0;
}