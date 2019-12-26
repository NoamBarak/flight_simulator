//
// Created by karin on 25/12/2019.
//
#include "flightsim.h"
#include "Interpreter.h"

extern unordered_map<string, VarInfo> toClient;
extern unordered_map<string, VarInfo> fromServer;
extern queue<string> updateOrder;
extern Interpreter interpreter;
extern bool firstVarInput;
std::mutex mutex_lock;

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
                Interpreter *i = new Interpreter();
                Expression *e = nullptr;
                try {

                    e = i->interpret(st);
                    ans = e->calculate();
                    delete e;
                    delete i;

                } catch (const char *e) {
                    if (e != nullptr) {
                        delete e;
                    }
                    if (i != nullptr) {
                        delete i;
                    }
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

int SleepCommand::execute(vector<string> vector, int index) {
    cout << "Sleeping ..." << endl;
    double ans = convStringToNum(vector, index + 1);
    cout << "COM-" << vector.at(index) << ",  VAL-" << ans << endl;
    return index + 2;
}

void runClient(const char *ip, unsigned short portShort) {
    cout << "Client thread" << endl;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Could not create a socket" << endl;
        exit(-1);
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(portShort);
    int is_connected = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    if (is_connected == -1) {
        std::cerr << "Could not connect to host server" << endl;
    } else {
        cout << "Connected to server" << endl;
    }
}

int ConnectCommand::execute(vector<string> vector, int index) {
    cout << "Connecting ..." << endl;
    string ipStr = vector.at(index + 1);
    const char *ip = ipStr.c_str();
    string port = vector.at(index + 2);
    const char *portNum = port.c_str();
    unsigned short portShort = (unsigned short) strtoul(portNum, NULL, 0);
    //std::thread threadClient(runClient, ip, portShort);
    //threadClient.detach();

    string address = vector.at(index + 1);
    //value of port
    double ans = convStringToNum(vector, index + 2);
    cout << "COM-" << vector.at(index) << ",  AD-" << address << ",  VAL-" << ans << endl;
    return index + 3;
}

void initXML(string *xmlArr) {
    string xmlOrder[36];
    xmlOrder[0] = "/instrumentation/airspeed-indicator/indicated-speed-kt";
    xmlOrder[1] = "/sim/time/warp";
    xmlOrder[2] = "/controls/switches/magnetos";
    xmlOrder[3] = "//instrumentation/heading-indicator/offset-deg";
    xmlOrder[4] = "/instrumentation/altimeter/indicated-altitude-ft";
    xmlOrder[5] = "/instrumentation/altimeter/pressure-alt-ft";
    xmlOrder[6] = "/instrumentation/attitude-indicator/indicated-pitch-deg";
    xmlOrder[7] = "/instrumentation/attitude-indicator/indicated-roll-deg";
    xmlOrder[8] = "/instrumentation/attitude-indicator/internal-pitch-deg";
    xmlOrder[9] = "/instrumentation/attitude-indicator/internal-roll-deg";
    xmlOrder[10] = "/instrumentation/encoder/indicated-altitude-ft";
    xmlOrder[11] = "/instrumentation/encoder/pressure-alt-ft";
    xmlOrder[12] = "/instrumentation/gps/indicated-altitude-ft";
    xmlOrder[13] = "/instrumentation/gps/indicated-ground-speed-kt";
    xmlOrder[14] = "/instrumentation/gps/indicated-vertical-speed";
    xmlOrder[15] = "/instrumentation/heading-indicator/indicated-heading-deg";
    xmlOrder[16] = "/instrumentation/magnetic-compass/indicated-heading-deg";
    xmlOrder[17] = "/instrumentation/slip-skid-ball/indicated-slip-skid";
    xmlOrder[18] = "/instrumentation/turn-indicator/indicated-turn-rate";
    xmlOrder[19] = "/instrumentation/vertical-speed-indicator/indicated-speed-fpm";
    xmlOrder[20] = "/controls/flight/aileron";
    xmlOrder[21] = "/controls/flight/elevator";
    xmlOrder[22] = "/controls/flight/rudder";
    xmlOrder[23] = "/controls/flight/flaps";
    xmlOrder[24] = "/controls/engines/engine/throttle";
    xmlOrder[25] = "/controls/engines/current-engine/throttle";
    xmlOrder[26] = "/controls/switches/master-avionics";
    xmlOrder[27] = "/controls/switches/starter";
    xmlOrder[28] = "/engines/active-engine/auto-start";
    xmlOrder[29] = "/controls/flight/speedbrake";
    xmlOrder[30] = "/sim/model/c172p/brake-parking";
    xmlOrder[31] = "/controls/engines/engine/primer";
    xmlOrder[32] = "/controls/engines/current-engine/mixture";
    xmlOrder[33] = "/controls/switches/master-bat";
    xmlOrder[34] = "/controls/switches/master-alt";
    xmlOrder[35] = "/engines/engine/rpm";
    xmlArr = xmlOrder;
}
unordered_map<string, int> initXMl(){
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
void runServer1(unsigned short portShort) {
    unordered_map<string, int> map =  initXMl();
    /*xmlOrder[0] = "/instrumentation/airspeed-indicator/indicated-speed-kt";
    xmlOrder[1] = "/sim/time/warp";
    xmlOrder[2] = "/controls/switches/magnetos";
    xmlOrder[3] = "//instrumentation/heading-indicator/offset-deg";
    xmlOrder[4] = "/instrumentation/altimeter/indicated-altitude-ft";
    xmlOrder[5] = "/instrumentation/altimeter/pressure-alt-ft";
    xmlOrder[6] = "/instrumentation/attitude-indicator/indicated-pitch-deg";
    xmlOrder[7] = "/instrumentation/attitude-indicator/indicated-roll-deg";
    xmlOrder[8] = "/instrumentation/attitude-indicator/internal-pitch-deg";
    xmlOrder[9] = "/instrumentation/attitude-indicator/internal-roll-deg";
    xmlOrder[10] = "/instrumentation/encoder/indicated-altitude-ft";
    xmlOrder[11] = "/instrumentation/encoder/pressure-alt-ft";
    xmlOrder[12] = "/instrumentation/gps/indicated-altitude-ft";
    xmlOrder[13] = "/instrumentation/gps/indicated-ground-speed-kt";
    xmlOrder[14] = "/instrumentation/gps/indicated-vertical-speed";
    xmlOrder[15] = "/instrumentation/heading-indicator/indicated-heading-deg";
    xmlOrder[16] = "/instrumentation/magnetic-compass/indicated-heading-deg";
    xmlOrder[17] = "/instrumentation/slip-skid-ball/indicated-slip-skid";
    xmlOrder[18] = "/instrumentation/turn-indicator/indicated-turn-rate";
    xmlOrder[19] = "/instrumentation/vertical-speed-indicator/indicated-speed-fpm";
    xmlOrder[20] = "/controls/flight/aileron";
    xmlOrder[21] = "/controls/flight/elevator";
    xmlOrder[22] = "/controls/flight/rudder";
    xmlOrder[23] = "/controls/flight/flaps";
    xmlOrder[24] = "/controls/engines/engine/throttle";
    xmlOrder[25] = "/controls/engines/current-engine/throttle";
    xmlOrder[26] = "/controls/switches/master-avionics";
    xmlOrder[27] = "/controls/switches/starter";
    xmlOrder[28] = "/engines/active-engine/auto-start";
    xmlOrder[29] = "/controls/flight/speedbrake";
    xmlOrder[30] = "/sim/model/c172p/brake-parking";
    xmlOrder[31] = "/controls/engines/engine/primer";
    xmlOrder[32] = "/controls/engines/current-engine/mixture";
    xmlOrder[33] = "/controls/switches/master-bat";
    xmlOrder[34] = "/controls/switches/master-alt";
    xmlOrder[35] = "/engines/engine/rpm";*/

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
            for (int i = 0; i < vector.size(); i++) {
                std::cout << vector.at(i) << ' ';
                std::cout << vector.size() << ' ';
            }

            if (!fromServer.empty() && firstVarInput) {
                mutex_lock.lock();
                for (auto &it: fromServer) {
                    string sim = it.second.getSim();
                    string add = sim.substr(1, sim.length() - 2);
                    int index = map.at(add);
                    it.second.setValue(stod(vector.at(index)));
                    cout << "\tVar name :" << it.first << " " << stod(vector.at(index)) << endl;
                }
                mutex_lock.unlock();
                firstVarInput = false;
            }
            mutex_lock.lock();
            for (auto &it: fromServer) {
                string sim = it.second.getSim();
                string add = sim.substr(1, sim.length() - 2);
                int index = map.at(add);
                it.second.setValue(stod(vector.at(index)));
                cout << "\tVar name :" << it.first << " " << stod(vector.at(index)) << endl;
            }
            mutex_lock.unlock();
        }
    }
}

int AssignVarCommand::execute(vector<string> vector, int index) {
    cout << "assigning var ... " << endl;
    string nameOfVar = vector[index];
    string eqSign = vector[index + 1];//always "="
    //value of var
    // double ans = convStringToNum(vector, index + 2);
    double ans = 0;
    cout << "VAR-" << vector.at(index) << ",  SIGN-" << eqSign << ",  VAL-" << ans << endl;
    return index + 3;
}

int OpenServerCommand::execute(vector<string> vector, int index) {
    cout << "Opening Server ..." << endl;
    string port = vector.at(index + 1);
    const char *portNum = port.c_str();
    unsigned short portShort = (unsigned short) strtoul(portNum, NULL, 0);
    //std::thread threadServer(runServer1, portShort);
    //this->threads[0]=thread(&OpenServerCommand::runServer,this,port);

    // detach / join ?
    //this->threads[0]= thread((runServer, portShort));
    //threadServer.join();
    //this->threads[0].join();

    /*std::thread threadServer(runServer1, portShort);
    threadServer.join();*/
    this->threads[0] = std::thread(runServer1, portShort);
    cout << "created thread" << endl;

    //value of port
    double ans = convStringToNum(vector, index + 1);
    cout << "COM-" << vector.at(index) << ",  VAL-" << ans << endl;
    return index + 2;
}

int PrintCommand::execute(vector<string> vector, int index) {
    cout << "Printing ..." << endl;
    string output = vector[index + 1];
    cout << "COM-" << vector.at(index) << ",  OUTPUT-" << output << endl;
    return index + 2;
}

int DefineVarCommand::execute(vector<string> vector, int index) {
    cout << "var shit" << endl;
    string nameOfVar, arrowOrEq, sim, address;
    double value = 0;
    nameOfVar = vector[index + 1];
    arrowOrEq = vector[index + 2];

    if (vector[index + 2] == "->" || vector[index + 2] == "<-") {
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
    // value= convStringToNum(vector,index+4);
    cout << "COM-" << vector.at(index) << ",  NAME-" << nameOfVar <<
         ",  SIGN-" << arrowOrEq << ",  VAL-" << value << endl;
    return index + 4;
}

int IfCommand::execute(vector<string> vector, int index) {
    cout << "If Command" << endl;
    string ifCom = vector[index];//always "if"
    string cond = vector[index + 1];
    string leftParen = vector[index + 2];
    index = index + 3;
    while (vector[index] != "}") {
        //Command
        if (map.find(vector[index]) != map.end()) {
            Command *c = map.at(vector[index]);
            index = c->execute(vector, index);
        } else {
            AssignVarCommand assignVarCommand = AssignVarCommand();
            index = assignVarCommand.execute(vector, index);

        }
    }
    return index + 1;
}

int WhileCommand::execute(vector<string> vector, int index) {
    cout << "While Command" << endl;
    string whileCom = vector[index]; //always "while"
    string cond = vector[index + 1];
    string leftParen = vector[index + 2];
    index = index + 3;
    while (vector[index] != "}") {
        //Command
        if (map.find(vector[index]) != map.end()) {
            Command *c = map.at(vector[index]);
            index = c->execute(vector, index);
        } else {
            AssignVarCommand assignVarCommand = AssignVarCommand();
            index = assignVarCommand.execute(vector, index);
        }
    }
    return index + 1;
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

            i = c->execute(fileVector, i);
            if (i < vecLen) {
                if (fileVector[i] != "var" && !afterDefineVarCom) {
                    if (beforeDefineVarCom) {
                        //this is the first command after DefineVar
                        afterDefineVarCom = true;
                        firstVarInput = afterDefineVarCom;
                        while(firstVarInput){
                            std::this_thread::sleep_for(std::chrono::microseconds(1000));
                        }
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
            AssignVarCommand assignVarCommand = AssignVarCommand();
            i = assignVarCommand.execute(fileVector, i);
        }
    }
}

