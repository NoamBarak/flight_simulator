//
// Created by karin on 25/12/2019.
//
#include "flightsim.h"
#include "Interpreter.h"

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

// The server thread runs the server
void runServer1(unsigned short portShort) {
    /*// Creating socket
    int sockftd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockftd == -1) {
        std::cerr << "Could not create a socket" << endl;
        exit(-1);
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portShort);
    // Binding
    if (bind(sockftd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr << "Could not bind socket to IP" << endl;
        exit(-2);
    }
    // Making the socket listen to the port
    if (listen(sockftd, 5) == -1) {
        std::cerr << "Error in listening command" << endl;
        exit(-3);
    }
    // Accepting the client
    int client_socket = accept(sockftd, (struct sockaddr *) &address,
                               (socklen_t *) &address);
    if (client_socket == -1) {
        std::cerr << "Error in accepting client" << endl;
        exit(-4);
    }*/
    /*//create socket
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
    int client_socket = accept(socketfd, (struct sockaddr *) &Caddress,
                               (socklen_t *) &Caddress);
    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        exit(-4);
    }else
        cout<<"Accepted client"<<endl;
    close(socketfd); //closing the listening socket
    //reading from client
    while (true) {
        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);
        std::cout << buffer << std::endl;
    }*/
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
/*    int client_socket = accept(socketfd, (struct sockaddr *)&Caddress,
                               (socklen_t*)&addreslen);*/

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
        std::cout << buffer << std::endl;
    }
}

int otherCasesCommand(vector<string> vector, int index) {
    string nameOfVar = vector[index];
    string eqSign = vector[index + 1];//always "="
    //value of var
    //  double ans = convStringToNum(vector, index + 2);
    double ans = 0;
    //  cout << "VAR-" << vector.at(index) << ",  SIGN-" << eqSign <<",  VAL-" << ans << endl;
    return 3;
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
        return index + 5;
    }
    // value= convStringToNum(vector,index+4);
    cout << "COM-" << vector.at(index) << ",  NAME-" << nameOfVar <<
         ",  SIGN-" << arrowOrEq << ",  VAL-" << value << endl;
    return index + 4;
}

int LoopOrCondCommand(vector<string> vector, int index, unordered_map<string, Command *> map) {

}

int IfCommand::execute(vector<string> vector, int index) {
    string ifCom = vector[index];//always "if"
    string cond = vector[index + 1];
    string leftParen = vector[index + 2];
    index = index + 3;
    while (vector[index] != "}") {
        //Command
        if (map.find(vector[index]) != map.end()) {
            Command *c = map.at(vector[index]);
            index = index + c->execute(vector, index);
        } else {
            index = index + otherCasesCommand(vector, index);
        }
    }
    return index + 1;
}

int WhileCommand::execute(vector<string> vector, int index) {
    string whileCom = vector[index]; //always "while"
    string cond = vector[index + 1];
    string leftParen = vector[index + 2];
    index = index + 3;
    while (vector[index] != "}") {
        //Command
        if (map.find(vector[index]) != map.end()) {
            Command *c = map.at(vector[index]);
            index = index + c->execute(vector, index);
        } else {
            index = index + otherCasesCommand(vector, index);
        }
    }
    return index + 1;
}

void parser(unordered_map<string, Command *> map, vector<string> fileVector) {
    int vecLen = fileVector.size();
    int i = 0;
    while (i < vecLen) {
        //Command
        if (map.find(fileVector[i]) != map.end()) {
            Command *c = map.at(fileVector[i]);
            //cout << "here !" << fileVector[i] << "!" << endl;
            i = c->execute(fileVector, i);
        } else {
            //loop or condition
            if (fileVector[i].find("while") != string::npos || fileVector[i].find("if") != string::npos) {
                //cout << "here !" << fileVector[i] << "!" << endl;
                i = LoopOrCondCommand(fileVector, i, map);


            } else {
                //  cout << "here !" << fileVector[i] << "!" << endl;
                i = i + otherCasesCommand(fileVector, i);
            }
        }
    }
}