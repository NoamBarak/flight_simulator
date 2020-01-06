#include <iostream>
#include <fstream>
#include <mutex>

#include "VarInfo.h"
#include "Interpreter.h"
#include "lexer.h"
#include "flightsim.h"

std::mutex mutex_lock;
AssignVarCommand assignVarCommand = AssignVarCommand();
Command *assVar = &assignVarCommand;

unordered_map<string, VarInfo> toClient;
unordered_map<string, VarInfo> fromServer;
queue<string> updateOrder;
Interpreter interpreter = Interpreter();
bool firstVarInput = false;
bool done = false;

vector<string> lexer(string filename) {
    Lexer lexer1 = Lexer();
    string right = "->";
    string check;
    vector<string> v1;
    int len;
    vector<string> fileVector;
    string line, checkCom;
    ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        cout << "Unable to open file\n" << endl;
    } else {
        while (!file.eof()) {
            getline(file, line);
            len = line.length();
            for (int i = 0; i < len;) {
                int firstLetter = line.find_first_not_of(" \t");
                checkCom = line.substr(firstLetter, 14);
                //openDataServer command
                if (checkCom == "openDataServer") {
                    lexer1.openDataServerLexer(line, checkCom, i, &v1);
                    break;
                }
                checkCom = line.substr(firstLetter, 20);
                //connectControlClient command
                if (checkCom == "connectControlClient") {
                    lexer1.connectControlClientLexer(line, checkCom, &v1);
                    break;
                }
                checkCom = line.substr(firstLetter, 5);
                //Print or Sleep command
                if (checkCom == "Print" || checkCom == "Sleep") {
                    lexer1.printOrSleepLexer(line, checkCom, &v1);
                    break;
                }
                checkCom = line.substr(firstLetter, 3);
                //defineVar command
                if (checkCom == "var") {
                    lexer1.varLexer(line, checkCom, &v1, i);
                    break;

                }

                //other cases
                if (line.find("{") == -1) {
                    lexer1.otherCasesLexer(line, checkCom, &v1);
                    break;
                } else {
                    //LOOP or CONDITION!
                    int firstSpace = line.find_first_of(" ");
                    int findParen = line.find_first_of("{");
                    string cond = (line.substr(firstSpace + 1, findParen - firstSpace - 1));
                    char lastPlace = cond[cond.length() - 1];
                    //check if there is a white space at the end of the condition
                    if (lastPlace == ' ') {
                        cond = (line.substr(firstSpace + 1, findParen - firstSpace - 2));
                    }
                    //insert 'while' or 'if'
                    v1.push_back(line.substr(0, firstSpace));
                    //insert condition
                    v1.push_back(cond);
                    //insert '{'
                    v1.push_back("{");
                    getline(file, line);
                    while (line.find("}") == -1) {
                        firstLetter = line.find_first_not_of(" \t");
                        checkCom = line.substr(firstLetter, 5);
                        //Print or Sleep command
                        if (checkCom == "Print" || checkCom == "Sleep") {
                            lexer1.printOrSleepLexer(line, checkCom, &v1);
                        } else {
                            checkCom = line.substr(firstLetter, 3);
                            //defineVar command
                            if (checkCom == "var") {
                                lexer1.varLexer(line, checkCom, &v1, i);
                            } else {
                                if (line.find("{") == -1) {
                                    lexer1.otherCasesLexer(line, checkCom, &v1);
                                }
                            }
                        }
                        getline(file, line);
                    }
                    v1.push_back("}");
                }
                break;

            }
        }
    }
    file.close();
    return v1;
}

void parser(unordered_map<string, Command *> map, vector<string> fileVector) {
    int vecLen = fileVector.size();
    int i = 0;
    bool beforeDefineVarCom = false;
    bool afterDefineVarCom = false;
    while (i < vecLen) {
        //Command
        if (map.find(fileVector[i]) != map.end()) {
            Command *c = map.at(fileVector[i]);
            if (fileVector[i] == "var") {
                if (!beforeDefineVarCom) {
                    //this is the first DefineVar command
                    beforeDefineVarCom = true;
                }
            }

            i = c->execute(fileVector, i);
            if (i < vecLen) {
                if (fileVector[i] != "var" && !afterDefineVarCom) {
                    if (beforeDefineVarCom) {
                        //this is the first command after DefineVar
                        afterDefineVarCom = true;
                        firstVarInput = afterDefineVarCom;
                    }
                }
            }
        } else {
            mutex_lock.lock();
            i = assignVarCommand.execute(fileVector, i);
            mutex_lock.unlock();
        }
    }
}
int main(int argc, char *argv[]) {
    // Verifying that a filename is in the arguments
    if (argc < 2) {
        cerr << "File name not added";
        exit(1);
    }
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
    fileVector = lexer(argv[1]);
    // Starting to interpret and execute
    parser(map, fileVector);
    done = true;
    // Waiting for the threads to close and join the main correctly
    threads[0].join();
    threads[1].join();
    return 0;
}