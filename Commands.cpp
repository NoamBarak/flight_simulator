//
// Created by karin on 25/12/2019.
//
#include "flightsim.h"
#include "VarInfo.h"
#include "Command.h"
#include "Interpreter.h"

#include <list>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <queue>

extern unordered_map<string, VarInfo> toClient;
extern unordered_map<string, VarInfo> fromServer;
extern queue<string> updateOrder;
extern Interpreter interpreter;
extern bool firstVarInput;
extern bool done;
bool serverConnected = false;
bool clientConnected = false;
extern std::mutex mutex_lock;
extern Command *assVar;

// Function to turn a string into a double by using our interpreter
double strExpCalculate(string st) {
    double ans;
    bool checkIfNum = true;
    //check if the string is a number or expression - if expression- do interpreter, if number- do stod
    for (unsigned int j = 0; j < st.length(); j++) {
        if (!isdigit(st[j])) {
            //not a number!
            if (st.at(j) != '.') {
                Expression *e = nullptr;
                try {
                    e = interpreter.interpret(st);
                    ans = e->calculate();
                } catch (const char *e1) {
                    if (e1 != nullptr) {
                    }
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

double convStringToNum(vector<string> vector, int index) {
    // Converting the data at vector index to by using the interpreter
    string st = vector.at(index);
    return strExpCalculate(st);
}

int SleepCommand::execute(vector<string> vector, int index) {
    // Interpreting
    double ans = convStringToNum(vector, index + 1);
    this_thread::sleep_for(chrono::milliseconds((int) (ans)));
    return index + 2;
}

void runClient(const char *ip, unsigned short portShort) {
    // Creating the socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Could not create a socket (Client)" << std::endl;
        exit(-1);
    }
    // Bind a socket to an IP
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(portShort);

    while (!clientConnected) {
        int is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
        if (is_connect == -1) {

        } else {
            std::cout << "Client is now connected to server" << std::endl;
            clientConnected = true;
        }
    }
    // We made the connection - we will start to send data
    while (true) {
        if (done)
            break;
        // If there are variables that need to be sent to the simulator
        if (!updateOrder.empty()) {
            // Constructing the correct message to the simulator
            string add = toClient.at(updateOrder.front()).getSim().substr(1, toClient.at(
                    updateOrder.front()).getSim().length() - 2);
            string msg = "set " + add + " " +
                         to_string(toClient.at(updateOrder.front()).getValue()) + "\r\n";
            int len = msg.length() + 1;
            char char_array[len];

            strcpy(char_array, msg.c_str());
            std::vector<char> vec(msg.c_str(), msg.c_str() + len);
            char *a = &vec[0];
            // Sending the message
            int is_sent = send(client_socket, a, strlen(char_array), 0);
            updateOrder.pop();
            if (is_sent == -1) {
                std::cout << "Error sending message" << std::endl;
            } else {
                //std::cout << "Hello message sent to server" << std::endl;
            }
        } else
            // If there are no variables that need to be updated - the thread will sleep
            this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    close(client_socket);
}

int ConnectCommand::execute(vector<string> vector, int index) {
    // Getting the necessary data to open a client server
    string address = vector.at(index + 1).substr(1, vector.at(index + 1).length() - 2);
    const char *ip = address.c_str();
    double ans = convStringToNum(vector, index + 2);
    string port = to_string(ans);
    const char *portNum = port.c_str();
    unsigned short portShort = (unsigned short) strtoul(portNum, NULL, 0);
    // Opening a thread and inserting it into our thread array - so it can later join in main
    this->threads[1] = std::thread(runClient, ip, portShort);
    // Making sure the client is connected
    while (!clientConnected) {
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    return index + 3;
}

unordered_map<string, int> initXMl() {
    // Creating a map containing a var's address and its index in the XML data
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

void runServer(unsigned short portShort) {
    unordered_map<string, int> map = initXMl();
    // Creating the socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
        exit(-1);
    }
    // Bind the socket to an IP address
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portShort);
    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr << "Could not bind the socket to an IP" << std::endl;
        exit(-2);
    }
    // Making the socket listen
    if (listen(socketfd, 5) == -1) {
        std::cerr << "Error during listening command" << std::endl;
        exit(-3);
    } else {
        std::cout << "Server is now listening ..." << std::endl;
    }
    // Accepting a client
    sockaddr_in Caddress;
    int client_socket = accept(socketfd, (struct sockaddr *) &Caddress,
                               (socklen_t *) &Caddress);
    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        exit(-4);
    } else {
        cout << "Accepted Client" << endl;
        serverConnected = true;
    }
    close(socketfd);
    // Getting data from the server
    while (true) {
        if (done)
            break;
        // Reading the data from the server
        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);
        // Getting an XML data from the buffer
        string b = buffer;
        string a;
        // We want to make sure we only get one line of the XML data
        int firstEnter = -1, secondEnter = -1;
        // Finding the first \n
        int count = 0;

        for (int i = 0; i < 1024; i++) {
            if (buffer[i] == '\n') {
                firstEnter = i;
                break;
            }
            if (buffer[i] == ',') {
                count++;
            }
        }
        // If there isn't even one \n or if there isn't enough data - the buffer is invalid
        if (firstEnter == -1 || count < 35) {
            continue;
        }
        // Finding the second \n
        for (int j = (firstEnter + 1); j < 1024; j++) {
            if (buffer[j] == '\n') {
                secondEnter = j;
                break;
            }
        }
        // If the there isnt another \n - it means our data is stored in indexes 0 to firstEnter(\n)
        if (secondEnter == -1) {
            a = b.substr(0, firstEnter - 1);

        } else {
            // The data is stored in indexes firstEnter to secondEnter
            a = b.substr(firstEnter + 1, secondEnter - firstEnter - 2);
        }
        // Now our string only contains the relevant XML data, so we will split the data accordingly
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
            /* firstVarInput- making sure we get at least one initialization of the variables
             * that need to be updated from the server so when we start assigning vars with data
             * our interpreter will include all relevant variables and our program won't fail*/
            if (!fromServer.empty() && firstVarInput) {
                // We will enter our variables maps and so we will use mutex
                mutex_lock.lock();
                for (auto &it: fromServer) {
                    string sim = it.second.getSim();
                    string add = sim.substr(1, sim.length() - 2);
                    int index = map.at(add);
                    it.second.setValue(stod(vector.at(index)));
                }
                mutex_lock.unlock();
                firstVarInput = false;
            }
            // We will enter our variables maps and so we will use mutex
            mutex_lock.lock();
            for (auto &it: fromServer) {
                string sim = it.second.getSim();
                string add = sim.substr(1, sim.length() - 2);
                int index = map.at(add);
                it.second.setValue(stod(vector.at(index)));
            }
            mutex_lock.unlock();
        }
    }
}

int AssignVarCommand::execute(vector<string> vector, int index) {
    string nameOfVar = vector[index];
    string eqSign = vector[index + 1];//always "="
    //value of var
    double ans = convStringToNum(vector, index + 2);
    // if the var is in the -> map
    if (toClient.find(nameOfVar) != toClient.end()) {
        toClient.at(nameOfVar).setValue(ans);
    } else {
        // if the var is not in the map, meaning its a var that doesnt belong to any map
        VarInfo varInfo = VarInfo(nameOfVar);
        varInfo.setValue(ans);
    }
    return index + 3;
}

int OpenServerCommand::execute(vector<string> vector, int index) {
    // Getting the necessary data to open a server
    double ans = convStringToNum(vector, index + 1);
    string port = std::to_string(ans);
    const char *portNum = port.c_str();
    unsigned short portShort = (unsigned short) strtoul(portNum, NULL, 0);
    // Opening a thread and inserting it into our thread array - so it can later join in main
    this->threads[0] = std::thread(runServer, portShort);
    // Making sure the server is connected
    while (!serverConnected) {
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    return index + 2;
}

int PrintCommand::execute(vector<string> vector, int index) {
    string output = vector[index + 1];
    if (output[0] == '"') {
        // If it's a string
        string add = output.substr(1, output.length() - 2);
        output = add;
    } else {
        // If it's not a string - we will interpret
        double ans = convStringToNum(vector, index + 1);
        output = to_string(ans);
    }
    cout << output << endl;
    return index + 2;
}

int DefineVarCommand::execute(vector<string> vector, int index) {
    string nameOfVar, arrowOrEq, sim, address;
    double value = 0;
    nameOfVar = vector[index + 1];
    arrowOrEq = vector[index + 2];
    // If the var is initialized - var name arrow sim("address")
    if (vector[index + 2] == "->" || vector[index + 2] == "<-") {
        sim = vector[index + 3];//always "sim"
        address = vector[index + 4];
        // Adding the variable to the map
        VarInfo varInfo = VarInfo(nameOfVar, address, vector[index + 2]);
        // Adding the var to the right map
        if (vector[index + 2] == "->") {
            toClient.emplace(std::make_pair(nameOfVar, varInfo));
        } else if (vector[index + 2] == "<-") {
            fromServer.emplace(std::make_pair(nameOfVar, varInfo));
        }
        return index + 5;
    }
    // If the var is initialized - var name = something
    value = convStringToNum(vector, index + 3);
    VarInfo varInfo1 = VarInfo(nameOfVar);
    varInfo1.setValue(value);
    return index + 4;
}

bool conditionCheck(string cond) {
    // A function to check if the condition is true/false
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
    //if the values are almost the same, change them to be the same
    if ((left - right < 0.0000000000001 && left - right > 0) || (right - left < 0.0000000000001 && right - left > 0))
        right = left;
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
    else
        return false;
}

int IfCommand::execute(vector<string> vector, int index) {
    string ifCom = vector[index];//always "if"
    string cond = vector[index + 1];
    string leftParen = vector[index + 2];
    index = index + 3;
    list<Command *> commandList;
    bool checkCond = conditionCheck(cond);
    if (checkCond) {
        while (vector[index] != "}") {
            // Entering each command to the commands list
            if (map.find(vector[index]) != map.end()) {
                Command *c = map.at(vector[index]);
                commandList.push_back(c);
                mutex_lock.lock();
                index = c->execute(vector, index);
                mutex_lock.unlock();
            } else {
                commandList.push_back(assVar);
                mutex_lock.lock();
                index = assVar->execute(vector, index);
                mutex_lock.unlock();
            }
        }
    } else {
        //if false- we just need to return the relevant index of the vector
        while (vector[index] != "}") {
            index++;
        }
    }

    return index + 1;
}

int WhileCommand::execute(vector<string> vector, int index) {
    string whileCom = vector[index]; //always "while"
    string cond = vector[index + 1];
    string leftParen = vector[index + 2]; // {
    index = index + 3;
    int saveIndex = index;
    bool bigCheck = true;
    bool checkCond = conditionCheck(cond);
    bool isFirst = true;
    list<Command *> commandList;
    commandList.clear();
    while (bigCheck) {
        index = saveIndex;
        if (checkCond) {
            if (isFirst) {
                isFirst = false;
                while (vector[index] != "}") {
                    // Entering each command to the commands list
                    if (map.find(vector[index]) != map.end()) {
                        Command *c = map.at(vector[index]);
                        commandList.push_back(c);
                        mutex_lock.lock();
                        index = c->execute(vector, index);
                        mutex_lock.unlock();
                    } else {
                        commandList.push_back(assVar);
                        mutex_lock.lock();
                        index = assVar->execute(vector, index);
                        mutex_lock.unlock();
                    }
                }
            } else {
                for (auto const &c : commandList) {
                    if (c != NULL) {
                        mutex_lock.lock();
                        index = c->execute(vector, index);
                        mutex_lock.unlock();
                    }
                }
            }
            checkCond = conditionCheck(cond);
        } else {
            //if false- we just need to return the relevant index of the vector
            bigCheck = false;
            while (vector[index] != "}") {
                index++;
            }
            return index + 1;
        }
    }
    return index + 1;
}