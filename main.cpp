#include <iostream>
#include "Command.h"
#include "flightsim.h"
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <bits/unique_ptr.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <map>
//using namespace std;

int SleepCommand::execute(vector<string> vector, int index) {
    cout << "Sleeping" << endl;
    return 2;
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
    std::thread threadClient(runClient, ip, portShort);
    threadClient.detach();
    return 3;
}

// The server thread runs the server
void runServer(unsigned short portShort) {
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
    int client_socket = accept(socketfd, (struct sockaddr *) &Caddress,
                               (socklen_t *) &Caddress);

    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        exit(-4);
    }

    close(socketfd); //closing the listening socket

    //reading from client
    while (true) {
        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);
        std::cout << buffer << std::endl;
    }
}

int OpenServerCommand::execute(vector<string> vector, int index) {
    cout << "Opening Server ..." << endl;
    string port = vector.at(index + 1);
    const char *portNum = port.c_str();
    unsigned short portShort = (unsigned short) strtoul(portNum, NULL, 0);
    std::thread threadServer(runServer, portShort);
    threadServer.join();
    return 2;
}

int PrintCommand::execute(vector<string> vector, int index) {
    cout << "Printing ..." << endl;
    return 2;
}

int DefineVarCommand::execute(vector<string> vector, int index) {
    cout << "Sleeping" << endl;
    return 2;
}

unordered_map<string, Command> initCommandMap() {
    /*unordered_map<string, Command*> map ;
    //map<string, Command*> map1 ;
    OpenServerCommand openServerCommand = OpenServerCommand();
    Command& openServerCommand1 = openServerCommand;
    map.emplace(std::make_pair("openDataServer",&openServerCommand1));
    ConnectCommand connectCommand = ConnectCommand();
    Command& connectCommand1 = connectCommand;
    map.emplace(std::make_pair("connectControlClient",&connectCommand1));
    DefineVarCommand defineVarCommand = DefineVarCommand();
    Command& defineVarCommand1 = defineVarCommand;
    map.emplace(std::make_pair("var",&defineVarCommand1));
    PrintCommand printCommand = PrintCommand();
    Command& printCommand1 = printCommand;
    map.emplace(std::make_pair("Print",&printCommand1));
    SleepCommand sleepCommand = SleepCommand();
    Command& sleepCommand1 = sleepCommand;
    map.emplace(std::make_pair("Sleep",&sleepCommand1));
    std::cout << typeid(sleepCommand1).name() << '\n';*/
    unordered_map<string, Command> map;
    //map<string, Command*> map1 ;
/*    OpenServerCommand openServerCommand = OpenServerCommand();
    Command &openServerCommand1 = openServerCommand;
    map.emplace(std::make_pair("openDataServer", openServerCommand1));
    ConnectCommand connectCommand = ConnectCommand();
    Command &connectCommand1 = connectCommand;
    map.emplace(std::make_pair("connectControlClient", connectCommand1));
    DefineVarCommand defineVarCommand = DefineVarCommand();
    Command &defineVarCommand1 = defineVarCommand;
    map.emplace(std::make_pair("var", defineVarCommand1));
    PrintCommand printCommand = PrintCommand();
    Command &printCommand1 = printCommand;
    map.emplace(std::make_pair("Print", printCommand1));*/
    SleepCommand sleepCommand = SleepCommand();
    Command &sleepCommand1 = sleepCommand;
    map.emplace(std::make_pair("Sleep", sleepCommand1));
    cout<<"here!!!!!!!!!\n"<<endl;
    std::cout <<"here " << typeid(sleepCommand1).name() << '\n';
    return map;
}

//read file
vector<string> lexer() {
    vector<string> v1;
    int len;
    vector<string> fileVector;
    string line;
    ifstream file;
    file.open("fly.txt");
    if (!file.is_open()) {
        cout << "Unable to open file\n" << endl;
    } else {
        while (!file.eof()) {//not the end of the file
            getline(file, line);
            len = line.length();
            char *v = new char[len];
            strcpy(v, line.c_str());
            char *splitLine = strtok(v, " ");
            //split- SPACE
            while (splitLine) {
                v1.push_back(splitLine);
                splitLine = strtok(NULL, " ");
            }
        }
    }
    file.close();
    vector<string> v2;
    int fileVecSize = v1.size();
    // split- LEFT PAREN
    for (int i = 0; i < fileVecSize; i++) {
        len = v1[i].length() + 1;
        char *v = new char[len];
        strcpy(v, v1[i].c_str());
        char *splitLine = strtok(v, "(");
        while (splitLine) {
            v2.push_back(splitLine);
            splitLine = strtok(NULL, "(");
        }
    }
    vector<string> v3;
    fileVecSize = v2.size();
    // split- RIGHT PAREN
    for (int i = 0; i < fileVecSize; i++) {
        len = v2[i].length() + 1;
        char *v = new char[len];
        strcpy(v, v2[i].c_str());
        char *splitLine = strtok(v, ")");
        while (splitLine) {
            v3.push_back(splitLine);
            splitLine = strtok(NULL, ")");
        }
    }
    fileVecSize = v3.size();
    // split- COMMA
    for (int i = 0; i < fileVecSize; i++) {
        len = v3[i].length() + 1;
        char *v = new char[len];
        strcpy(v, v3[i].c_str());
        char *splitLine = strtok(v, ",");
        while (splitLine) {
            fileVector.push_back(splitLine);
            // cout << splitLine << endl;
            splitLine = strtok(NULL, ",");
        }
    }
    return fileVector;
}

int main() {
    vector<string> fileVector;
    unordered_map<string, Command> map = initCommandMap();
    fileVector = lexer();
    Command* c = &map.at("Sleep");
    std::cout <<"here " << typeid(c).name() << '\n';
    int i = 0;
/*
    //Command* c {map.at("OpenDataServer")};
    std::cout << "in main - " << typeid(c).name() << '\n';
    //Command& c = map.at("Sleep");
    //std::cout << typeid(c1).name() << '\n';

*/
    if (c != NULL)
        i += c->execute(fileVector, i);


    OpenServerCommand *openServerCommand = new OpenServerCommand;
    //i += openServerCommand->execute(fileVector, i);


    ConnectCommand *connectCommand = new ConnectCommand;
    //connectCommand->execute(fileVector, i);
    return 0;
}
