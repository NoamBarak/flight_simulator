//
// Created by karin on 25/12/2019.
//
#include "flightsim.h"
#include "ex1.h"
#include <list>
#include <iostream>

extern unordered_map<string, VarInfo> toClient;
extern unordered_map<string, VarInfo> fromServer;
extern queue<string> updateOrder;
extern Interpreter interpreter;
extern bool firstVarInput;
extern bool done;
bool serverConnected = false;
bool clientConnected = false;
std::mutex mutex_lock;
AssignVarCommand assignVarCommand = AssignVarCommand();
Command *assVar = &assignVarCommand;

double convStringToNum(vector<string> vector, int index) {
    double ans;
    string st;
    st = vector.at(index);
    bool checkIfNum = true;
    //check if the string is a number or expression
    // if expression- do interpreter
    //if number- do stod
    for (int j = 0; j < st.length(); j++) {
        if (!isdigit(st[j])) {
            //not a number!
            if (st.at(j) != '.') {
                //Interpreter *i = new Interpreter();
                //cout << "CONV here 1 " << endl;
                Expression *e = nullptr;
                try {
                    // cout << "CONV here 2 " << endl;
                    e = interpreter.interpret(st);
                    //cout << "CONV here 3 " << endl;
                    ans = e->calculate();
                    //cout << "CONV here 4 " << endl;
                    //delete e;
                    //delete i;

                } catch (const char *e) {
                    if (e != nullptr) {
                        cout << "ERROR" << endl;
                        //delete e;
                    }
//                    if (i != nullptr) {
//                        delete i;
//                    }
                    std::cout << e << std::endl;
                }
                checkIfNum = false;
                break;
            }
        }
    }
    //cout << "CONV here 5 " << endl;
    if (checkIfNum) {
        ans = stod(st);
    }
    return ans;
}

double strExpCalculate(string st) {
    double ans;
    bool checkIfNum = true;
    //check if the string is a number or expression
    // if expression- do interpreter
    //if number- do stod
    for (int j = 0; j < st.length(); j++) {
        if (!isdigit(st[j])) {
            //not a number!
            if (st.at(j) != '.') {
                //Interpreter *i = new Interpreter();
                Expression *e = nullptr;
                try {
                    e = interpreter.interpret(st);
                    ans = e->calculate();
                    delete e;
                    //delete i;

                } catch (const char *e) {
                    if (e != nullptr) {
                        delete e;
                    }
//                    if (i != nullptr) {
//                        delete i;
//                    }
                    std::cout << e << std::endl;
                }
                checkIfNum = false;
                break;
            }
        }
    }
    if (checkIfNum) {
        ans = stod(st);
    }
    return ans;
}

int SleepCommand::execute(vector<string> vector, int index, bool onlyIndex) {
    if (onlyIndex)
        return index + 2;
    cout << "Sleeping ..." << endl;
    double ans = convStringToNum(vector, index + 1);
    this_thread::sleep_for(chrono::milliseconds((int) (ans)));
    //cout << "COM-" << vector.at(index) << ",  VAL-" << ans << endl;

    return index + 2;
}

void runClient(const char *ip, unsigned short portShort) {
    //create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        //error
        std::cerr << "Could not create a socket (Client)" << std::endl;
        exit(-1);
    }

    //bind socket to IP address
    // we first need to create the sockaddr obj.
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip); //give me any IP allocated for my machine
    address.sin_port = htons(portShort);

    while (!clientConnected) {
        int is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));

        if (is_connect == -1) {
            //std::cout << "Could not connect to host server (Client)" << std::endl;
            //exit(-2);
        } else {
            std::cout << "Client is now connected to server" << std::endl;
            clientConnected = true;
        }
    }


    // If we made a connection we will send the data
    while (true) {
        // if there are variables that need to be sent to the simulator
        //cout << "Queue Check:" << endl;
        if (!updateOrder.empty()) {
            //queue<string> check = updateOrder;
            //cout << "Queue Check:" << endl;
            /*while (!check.empty()) {
                cout << "\t " << check.front() << ", Val: " << toClient.at(check.front()).getValue()
                     << endl;
                check.pop();
            }*/
            // set bla/bla/bla value
            string add = toClient.at(updateOrder.front()).getSim().substr(1, toClient.at(
                    updateOrder.front()).getSim().length() - 2);
            string msg = "set " + add + " " +
                         to_string(toClient.at(updateOrder.front()).getValue()) + "\r\n";
            //cout << "Message - " << msg << endl;
            char char_array[msg.length() + 1];
            strcpy(char_array, msg.c_str());
            int is_sent = send(client_socket, char_array, strlen(char_array), 0);
            updateOrder.pop();
            if (is_sent == -1) {
                std::cout << "Error sending message" << std::endl;
            } else {
                //std::cout << "Hello message sent to server" << std::endl;
            }
        } else
            this_thread::sleep_for(std::chrono::milliseconds(200)); // no variables in the queue
    }
    close(client_socket);
}

int ConnectCommand::execute(vector<string> vector, int index, bool onlyIndex) {
    if (onlyIndex)
        return index + 3;
    cout << "Connecting ..." << endl;
    string add = vector.at(index + 1).substr(1, vector.at(index + 1).length() - 2);
    const char *ip = add.c_str();
    double ans = convStringToNum(vector, index + 2);
    string port = to_string(ans);
    const char *portNum = port.c_str();
    unsigned short portShort = (unsigned short) strtoul(portNum, NULL, 0);
    this->threads[1] = std::thread(runClient, ip, portShort);
    // Making sure the client is connected
    while (!clientConnected) {
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    return index + 3;
}

unordered_map<string, int> initXMl() {
    unordered_map<string, int> map;
    map.emplace(std::make_pair("/instrumentation/airspeed-indicator/indicated-speed-kt", 0));
    map.emplace(std::make_pair("/sim/time/warp", 1));
    map.emplace(std::make_pair("/controls/switches/magnetos", 2));
    map.emplace(std::make_pair("/instrumentation/heading-indicator/offset-deg", 3));
    map.emplace(std::make_pair("/instrumentation/altimeter/indicated-altitude-ft", 4));
    map.emplace(std::make_pair("/instrumentation/altimeter/pressure-alt-ft", 5));
    map.emplace(std::make_pair("/instrumentation/attitude-indicator/indicated-pitch-deg", 6));
    map.emplace(std::make_pair("/instrumentation/attitude-indicator/indicated-roll-deg", 7));
    map.emplace(std::make_pair("/instrumentation/attitude-indicator/internal-pitch-deg", 8));
    map.emplace(std::make_pair("/instrumentation/attitude-indicator/internal-roll-deg", 9));
    map.emplace(std::make_pair("/instrumentation/encoder/indicated-altitude-ft", 10));
    map.emplace(std::make_pair("/instrumentation/encoder/pressure-alt-ft", 11));
    map.emplace(std::make_pair("/instrumentation/gps/indicated-altitude-ft", 12));
    map.emplace(std::make_pair("/instrumentation/gps/indicated-ground-speed-kt", 13));
    map.emplace(std::make_pair("/instrumentation/gps/indicated-vertical-speed", 14));
    map.emplace(std::make_pair("/instrumentation/heading-indicator/indicated-heading-deg", 15));
    map.emplace(std::make_pair("/instrumentation/magnetic-compass/indicated-heading-deg", 16));
    map.emplace(std::make_pair("/instrumentation/slip-skid-ball/indicated-slip-skid", 17));
    map.emplace(std::make_pair("/instrumentation/turn-indicator/indicated-turn-rate", 18));
    map.emplace(std::make_pair("/instrumentation/vertical-speed-indicator/indicated-speed-fpm", 19));
    map.emplace(std::make_pair("/controls/flight/aileron", 20));
    map.emplace(std::make_pair("/controls/flight/elevator", 21));
    map.emplace(std::make_pair("/controls/flight/rudder", 22));
    map.emplace(std::make_pair("/controls/flight/flaps", 23));
    map.emplace(std::make_pair("/controls/engines/engine/throttle", 24));
    map.emplace(std::make_pair("/controls/engines/current-engine/throttle", 25));
    map.emplace(std::make_pair("/controls/switches/master-avionics", 26));
    map.emplace(std::make_pair("/controls/switches/starter", 27));
    map.emplace(std::make_pair("/engines/active-engine/auto-start", 28));
    map.emplace(std::make_pair("/controls/flight/speedbrake", 29));
    map.emplace(std::make_pair("/sim/model/c172p/brake-parking", 30));
    map.emplace(std::make_pair("/controls/engines/engine/primer", 31));
    map.emplace(std::make_pair("/controls/engines/current-engine/mixture", 32));
    map.emplace(std::make_pair("/controls/switches/master-bat", 33));
    map.emplace(std::make_pair("/controls/switches/master-alt", 34));
    map.emplace(std::make_pair("/engines/engine/rpm", 35));
    return map;
}

// The server thread runs the server
void runServer(unsigned short portShort) {
    unordered_map<string, int> map = initXMl();
    //create socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
        exit(-1);
    }

    //bind socket to IP address
    // we first need to create the sockaddr obj.
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
    address.sin_port = htons(portShort);
    //we need to convert our number
    // to a number that the network understands.

    //the actual bind command
    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr << "Could not bind the socket to an IP" << std::endl;
        exit(-2);
    }

    //making socket listen to the port

    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr << "Error during listening command" << std::endl;
        exit(-3);
    } else {
        std::cout << "Server is now listening ..." << std::endl;
    }

    // accepting a client
    sockaddr_in Caddress;
    int addreslen = sizeof(Caddress);
    int client_socket = accept(socketfd, (struct sockaddr *) &Caddress,
                               (socklen_t *) &Caddress);

    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        exit(-4);
    } else {
        cout << "Accepted Client" << endl;
        serverConnected = true;
    }

    close(socketfd); //closing the listening socket !

    //reading from client
    while (true) {
        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);
        string a = buffer;
        //std::cout << a << std::endl;
        // Noam part HERE!
        vector<string> vector;
        string st;
        int last = 0;
        int find = a.find_first_of(",", last);
        while (find < a.length() && find != (-1)) {
            st = a.substr(last, find - last - 1);
            vector.push_back(st);
            last = find + 1;
            find = a.find_first_of(",", last);
        }
        st = a.substr(last);
        vector.push_back(st);
        if (vector.size() > 1) {
            /*for (int i = 0; i < vector.size(); i++) {
                std::cout << vector.at(i) << ' ';
            }*/

            if (!fromServer.empty() && firstVarInput) {
                mutex_lock.lock();
                //cout << "First" << endl;
                for (auto &it: fromServer) {
                    string sim = it.second.getSim();
                    string add = sim.substr(1, sim.length() - 2);
                    int index = map.at(add);
                    it.second.setValue(stod(vector.at(index)));
                    //cout << "\tVar name :" << it.first << " " << stod(vector.at(index)) << endl;
                    // Print Check for the queue
                }

                mutex_lock.unlock();
                firstVarInput = false;
            }
            mutex_lock.lock();
            //cout << "Second" << endl;
            for (auto &it: fromServer) {
                string sim = it.second.getSim();
                string add = sim.substr(1, sim.length() - 2);
                int index = map.at(add);
                it.second.setValue(stod(vector.at(index)));
                //cout << "\tVar name :" << it.first << " " << it.second.getValue() << endl;
            }
            /*cout << "Interpreter map Variables: " << endl;
            for (auto &it: interpreter.getVariables()) {
                cout << "Name: "<<it.first << ", Val: " << it.second<<endl;
            }*/
            mutex_lock.unlock();
        }
    }
}

int AssignVarCommand::execute(vector<string> vector, int index, bool onlyIndex) {
    if (onlyIndex)
        return index + 3;
    string nameOfVar = vector[index];
    string eqSign = vector[index + 1];//always "="
    //value of var
    double ans = convStringToNum(vector, index + 2);
    //mutex_lock.lock();
    // if the var is in the -> map
    cout << "Name of Var: " << nameOfVar << " index: " << index << " ANSWER: " << (ans) << endl;
    if (toClient.find(nameOfVar) != toClient.end()) {
        toClient.at(nameOfVar).setValue(ans);
    } else {
        // if the var is not in the map, meaning its a var that doesnt belong to any map
        VarInfo varInfo = VarInfo(nameOfVar);
        varInfo.setValue(ans);
    }
    //mutex_lock.unlock();
    // cout << "VAR-" << vector.at(index) << ",  SIGN-" << eqSign << ",  VAL-" << ans << endl;
    return index + 3;
}

int OpenServerCommand::execute(vector<string> vector, int index, bool onlyIndex) {
    if (onlyIndex)
        return index + 2;
    cout << "Opening Server ..." << endl;
    double ans = convStringToNum(vector, index + 1);
    string port = std::to_string(ans);
    const char *portNum = port.c_str();
    unsigned short portShort = (unsigned short) strtoul(portNum, NULL, 0);
    this->threads[0] = std::thread(runServer, portShort);
    // Making sure the server is connected
    while (!serverConnected) {
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    return index + 2;
}

int PrintCommand::execute(vector<string> vector, int index, bool onlyIndex) {
    if (onlyIndex)
        return index + 2;
    cout << "Printing ..." << endl;
    string output = vector[index + 1];
    if (output[0] == '"') {
        // its a string
        string add = output.substr(1, output.length() - 2);
        output = add;
    } else {
        double ans = convStringToNum(vector, index + 1);
        output = to_string(ans);
    }
    cout << output << endl;
    //cout << "COM-" << vector.at(index) << ",  OUTPUT-" << output << endl;
    return index + 2;
}

int DefineVarCommand::execute(vector<string> vector, int index, bool onlyIndex) {
    cout << "var shit: ";
    string nameOfVar, arrowOrEq, sim, address;
    double value = 0;
    nameOfVar = vector[index + 1];
    arrowOrEq = vector[index + 2];

    if (vector[index + 2] == "->" || vector[index + 2] == "<-") {
        if (onlyIndex)
            return index + 5;
        sim = vector[index + 3];//always "sim"
        address = vector[index + 4];
        cout << "COM-" << vector.at(index) << ",  NAME-" << nameOfVar <<
             ",  SIGN-" << arrowOrEq << ",  SIM-" << sim << ",  AD-" << address << endl;
        // Adding the variable to the map
        VarInfo varInfo = VarInfo(nameOfVar, address, vector[index + 2]);
        if (vector[index + 2] == "->") {
            //map.emplace(std::make_pair("openDataServer", openServerCommand2));
            toClient.emplace(std::make_pair(nameOfVar, varInfo));
        } else if (vector[index + 2] == "<-") {
            fromServer.emplace(std::make_pair(nameOfVar, varInfo));
        }
        return index + 5;
    }
    if (onlyIndex)
        return index + 4;
    value = convStringToNum(vector, index + 3);
    cout << "COM-" << vector.at(index) << ",  NAME-" << nameOfVar <<
         ",  SIGN-" << arrowOrEq << ",  VAL-" << value << endl;
    VarInfo varInfo1 = VarInfo(nameOfVar);
    varInfo1.setValue(value);
    return index + 4;
}

int IfCommand::execute(vector<string> vector, int index, bool onlyIndex) {
    cout << "If Command" << endl;
    string ifCom = vector[index];//always "if"
    string cond = vector[index + 1];
    string leftParen = vector[index + 2];
    index = index + 3;
    int startIndex = index;
    while (vector[index] != "}") {
        //Command
        if (map.find(vector[index]) != map.end()) {
            Command *c = map.at(vector[index]);
            index = c->execute(vector, index, true);
        } else {
            AssignVarCommand assignVarCommand = AssignVarCommand();
            index = assignVarCommand.execute(vector, index, true);

        }
    }
    return index + 1;
}

bool conditionCheck(string cond) {
    string leftPart, op, rightPart;
    int firPlace;
    char options[] = {'<', '>', '!', '='};
    firPlace = cond.find_first_of(options, 0);
    char check = cond[firPlace + 1];
    leftPart = cond.substr(0, firPlace);
    if (check == '<' || check == '>' || check == '!' || check == '=') {
        op = cond.substr(firPlace, 2);
        rightPart = cond.substr(firPlace + 2);
    } else {
        op = cond.substr(firPlace, 1);
        rightPart = cond.substr(firPlace + 1);
    }
    double left = strExpCalculate(leftPart);
    double right = strExpCalculate(rightPart);
    if (op == "<")
        return left < right;
    else if (op == ">")
        return left > right;
    else if (op == "<=")
        return left <= right;
    else if (op == ">=")
        return left >= right;
    else if (op == "==")
        return left == right;
    else if (op == "!=")
        return left != right;
}

int WhileCommand::execute(vector<string> vector, int index, bool onlyIndex) {
    /* interpreter.setVariables("alt=2");
     interpreter.setVariables("h0=2");
     interpreter.setVariables("heading=2");
     interpreter.setVariables("aileron=2");
     interpreter.setVariables("roll=2");
     interpreter.setVariables("elevator=2");
     interpreter.setVariables("pitch=2");*/

    /*cout << "While Command" << endl;
    string whileCom = vector[index]; //always "while"
    string cond = vector[index + 1];
    cout << "Condition: " << cond << endl;
    string leftParen = vector[index + 2]; // {
    index = index + 3;
    bool first = true;
    // saving the index of the first command
    int startIndex = index;
    cout << "Start index " << startIndex << endl;
    // creating a list of commands we need to execute if the condiiton is true
    static list<Command *> commandList;
    commandList.clear();
    while (vector[index] != "}") {
        string insdk = vector[index];
        cout << "index: " << insdk << index << endl;
        // Entering each command to the commands list
        if (map.find(vector[index]) != map.end()) {
            Command *c = map.at(vector[index]);
            cout << "2" << endl;
            if(first){
                commandList.push_front(c);
                first = false;
                continue;
            }
            commandList.push_back(c);
            index = c->execute(vector, index, true);
        } else {
            if(first){
                commandList.push_front(assVar);
                cout << "1" << endl;
                first = false;
                continue;
            }
            commandList.push_back(assVar);
            cout << "1" << endl;
            index = assVar->execute(vector, index, true);
        }
    }
    cout << "While here 2" << endl;
    // checking the condition every iteration
    while (conditionCheck(cond)) {
        // each iteration the helper index will be at the index of the first command in our while condition
        int helperIndex = startIndex;
        mutex_lock.lock();
        // executing each command in out map
        for (auto const &c : commandList) {
            if (c != NULL) {
                cout << "While here 3 index: " << helperIndex << endl;
                helperIndex = c->execute(vector, helperIndex, false); //segmentation fault
                //cout << "While here 4" << startIndex << endl;
            }
            mutex_lock.unlock();
        }
        break;
    }*/
    cout << "While Command!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    string whileCom = vector[index]; //always "while"
    string cond = vector[index + 1];
    cout << "Condition:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << cond << endl;
    string leftParen = vector[index + 2]; // {
    index = index + 3;
    int saveIndex = index;
    bool bigCheck = true;
    bool checkCond = conditionCheck(cond);
    bool isFirst = true;
    list < Command * > commandList;
    int num = 1;
    commandList.clear();
    while (bigCheck) {
        cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << num << "&&&&&&&&&&&&&&&&&" << endl;
        index = saveIndex;
        if (checkCond) {
            cout << "COND TRUE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
            if (isFirst) {
                cout << "FIRST TIME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                isFirst = false;
                while (vector[index] != "}") {
                    // Entering each command to the commands list
                    cout << "COM--" << vector[index] << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                    cout << "INDEX--" << index << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                    if (map.find(vector[index]) != map.end()) {
                        Command *c = map.at(vector[index]);
                        commandList.push_back(c);
                        mutex_lock.lock();
                        index = c->execute(vector, index, false);
                        mutex_lock.unlock();
                    } else {

                        /*AssignVarCommand assignVarCommand = AssignVarCommand();
                        Command *assVar = &assignVarCommand;*/
                        commandList.push_back(assVar);
                        mutex_lock.lock();
                        index = assVar->execute(vector, index, false);
                        mutex_lock.unlock();
                    }
                }
            } else {
                for (auto const &c : commandList) {
                    if (c != NULL) {
                        mutex_lock.lock();
                        index = c->execute(vector, index, false); //segmentation fault
                        mutex_lock.unlock();
                    }
                }
            }
            checkCond = conditionCheck(cond);
        } else {
            bigCheck = false;
            while (vector[index] != "}") {
                index++;
            }
            cout << "FINISHED WHILE alt: server - " << fromServer.at("alt").getValue() << " interpreter: "
                 << interpreter.getVariables().at("alt") << endl;
            return index + 1;
        }
        num++;
    }

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
            //cout << "here !" << fileVector[i] << "!" << endl;
            if (fileVector[i] == "var") {
                if (!beforeDefineVarCom) {
                    //this is the first DefineVar command
                    beforeDefineVarCom = true;
                    cout << "!!!!!!!!!!!!!!" << fileVector[i + 1] << "!!!!!!!!!!!!" << endl;
                }
            }

            i = c->execute(fileVector, i, false);
            if (i < vecLen) {
                if (fileVector[i] != "var" && !afterDefineVarCom) {
                    if (beforeDefineVarCom) {
                        //this is the first command after DefineVar
                        afterDefineVarCom = true;
                        firstVarInput = afterDefineVarCom;
                        /*while (firstVarInput) {
                            std::this_thread::sleep_for(std::chrono::microseconds(1000));
                        }*/
                        cout << "------------" << fileVector[i] << "----------" << endl;
                    }
                }
            }
        }
            /*else {
                //loop or condition
                if (fileVector[i].find("while") != string::npos || fileVector[i].find("if") != string::npos) {
                    //cout << "here !" << fileVector[i] << "!" << endl;
                    i = LoopOrCondCommand(fileVector, i, map);
                }*/
        else {
            //  cout << "here !" << fileVector[i] << "!" << endl;
            //AssignVarCommand assignVarCommand = AssignVarCommand();
            mutex_lock.lock();
            i = assignVarCommand.execute(fileVector, i, false);
            mutex_lock.unlock();
        }
    }
}