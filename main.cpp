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
    cout << "Sleeping ..." << endl;
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
        std::cerr << "Could not create a socket"<<std::endl;
        exit (-1);
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
        std::cerr<<"Could not bind the socket to an IP"<<std::endl;
        exit (-2);
    }

    //making socket listen to the port

    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr<<"Error during listening command"<<std::endl;
        exit (-3);
    } else{
        std::cout<<"Server is now listening ..."<<std::endl;
    }

    // accepting a client
    sockaddr_in Caddress;
    int addreslen = sizeof(Caddress);
    int client_socket = accept(socketfd, (struct sockaddr *)&Caddress,
                               (socklen_t*)&Caddress);
/*    int client_socket = accept(socketfd, (struct sockaddr *)&Caddress,
                               (socklen_t*)&addreslen);*/

    if (client_socket == -1) {
        std::cerr<<"Error accepting client"<<std::endl;
        exit (-4);
    }else{
        cout<<"Accepted Client"<<endl;
    }

    close(socketfd); //closing the listening socket

    //reading from client
    while(true) {
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
    //std::thread threadServer(runServer1, portShort);
    //this->threads[0]=thread(&OpenServerCommand::runServer,this,port);

    // detach / join ?
    //this->threads[0]= thread((runServer, portShort));
    //threadServer.join();
    //this->threads[0].join();

    /*std::thread threadServer(runServer1, portShort);
    threadServer.join();*/
    this->threads[0] = std::thread (runServer1, portShort);
    cout<<"created thread"<<endl;
    return 2;
}

int PrintCommand::execute(vector<string> vector, int index) {
    cout << "Printing ..." << endl;
    return 2;
}

int DefineVarCommand::execute(vector<string> vector, int index) {
    cout << "var shit" << endl;
    return 2;
}

void openDataServerLexer(string line, string checkCom, int i, vector<string> *v1) {
    string port;
    //insert the relevant command into vector
    v1->push_back(checkCom);
    i = i + checkCom.length();
    //insert the value into vector
    //i+1, and line.length()-1-(i+1) because of the parentheses
    port = line.substr(i + 1, line.length() - 1 - (i + 1));
    v1->push_back(port);
}

void connectControlClientLexer(string line, string checkCom, vector<string> *v1) {
    string address, value;
    int saveFirst, saveSecond, saveComma;
    int len = line.length();
    saveFirst = line.find_first_of('"');
    saveSecond = line.find_first_of('"', saveFirst + 1);
    saveComma = line.find_first_of(",");
    //insert the relevant command into vector
    v1->push_back(checkCom);
    //insert address into vector
    v1->push_back(line.substr(saveFirst, saveSecond - saveFirst + 1));
    //insert port into vector
    v1->push_back(line.substr(saveComma + 1, len - saveComma - 2));
}

void printOrSleepLexer(string line, string checkCom, vector<string> *v1) {
    int len = line.length();
    int firstParen = line.find_first_of("(");
    //insert the relevant command into vector
    v1->push_back(checkCom);
    //insert address into vector
    v1->push_back(line.substr(firstParen + 1, len - firstParen - 2));
}

void varLexer(string line, string checkCom, vector<string> *v1, int i) {
    int len = line.length();
    i = i + checkCom.length() + 1;
    int spaceBeforeArrow = line.find_first_of(" ", i);
    int spaceAfterArrow = line.find_first_of(" ", spaceBeforeArrow + 1);
    int arrowOrEq = line.find_first_of("(", spaceAfterArrow + 1);
    string checkIfArrow = line.substr(spaceBeforeArrow + 1, spaceAfterArrow - spaceBeforeArrow - 1);
    //insert the relevant command into vector
    v1->push_back(checkCom);
    //insert name of var into vector
    v1->push_back(line.substr(i, spaceBeforeArrow - i));
    //insert arrow or '=' into vector
    v1->push_back(line.substr(spaceBeforeArrow + 1, spaceAfterArrow - spaceBeforeArrow - 1));
    if (checkIfArrow.find("<") != -1 || checkIfArrow.find(">") != -1) {
        //insert the word "sim"
        v1->push_back(line.substr(spaceAfterArrow + 1, arrowOrEq - spaceAfterArrow - 1));
        //insert address into vector
        v1->push_back(line.substr(arrowOrEq + 1, len - arrowOrEq - 2));
    } else {
        int placeOfEq = line.find_first_of("=");
        int spacePlace = line.find_first_of(" ", placeOfEq);
        //insert the string after '='
        v1->push_back(line.substr(spacePlace + 1));
    }
}

void otherCasesLexer(string line, string checkCom, vector<string> *v1) {
    int firstLetter = line.find_first_not_of("\t");
    int eqSignPlace = line.find_first_of("=");
    //insert name of var into vector
    string str = (line.substr(firstLetter, eqSignPlace-firstLetter));
    char lastPlace = str[str.length() - 1];
    //there is a white space between the variable and '='
    if (lastPlace == ' ') {
        v1->push_back(line.substr(firstLetter, eqSignPlace-firstLetter - 1));
    } else {
        v1->push_back(line.substr(firstLetter, eqSignPlace-firstLetter));
    }
    v1->push_back("=");
    int valueBegin = line.find_first_not_of(" ", eqSignPlace + 1);
    str = line.substr(valueBegin);
    v1->push_back(str);
}

//read file
vector<string> lexer() {
    string right = "->";
    string check;
    vector<string> v1;
    int len;
    vector<string> fileVector;
    string line, checkCom;
    ifstream file;
    file.open("fly.txt");
    if (!file.is_open()) {
        cout << "Unable to open file\n" << endl;
    } else {
        while (!file.eof()) {
            getline(file, line);
            len = line.length();
            for (int i = 0; i < len;) {
                int firstLetter = line.find_first_not_of("\t");
                checkCom = line.substr(firstLetter, 14);
                //openDataServer command
                if (checkCom == "openDataServer") {
                    openDataServerLexer(line, checkCom, i, &v1);
                    break;
                }
                checkCom = line.substr(firstLetter, 20);
                //connectControlClient command
                if (checkCom == "connectControlClient") {
                    connectControlClientLexer(line, checkCom, &v1);
                    break;
                }
                checkCom = line.substr(firstLetter, 5);
                //Print or Sleep command
                if (checkCom == "Print" || checkCom == "Sleep") {
                    printOrSleepLexer(line, checkCom, &v1);
                    break;
                }
                checkCom = line.substr(firstLetter, 3);
                //defineVar command
                if (checkCom == "var") {
                    varLexer(line, checkCom, &v1, i);
                    break;

                }

                //other cases
                if (line.find("{") == -1) {
                    otherCasesLexer(line, checkCom, &v1);
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
                        firstLetter = line.find_first_not_of("\t");
                        checkCom = line.substr(firstLetter, 5);
                        //Print or Sleep command
                        if (checkCom == "Print" || checkCom == "Sleep") {
                            printOrSleepLexer(line, checkCom, &v1);
                        }
                        else {
                            checkCom = line.substr(firstLetter, 3);
                            //defineVar command
                            if (checkCom == "var") {
                                varLexer(line, checkCom, &v1, i);
                            } else {
                                if (line.find("{") == -1) {
                                    otherCasesLexer(line, checkCom, &v1);
                                }
                            }
                        }
                        getline(file, line);
                    }
                    v1.push_back("}");
                }
                break;

            }
            //.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }
    }
    file.close();
    return v1;
}

int main() {
    thread threads[2];
    // Initializing the <string,Command*> map
    unordered_map<string, Command*> map ;
    // Adding the commands to the map
    OpenServerCommand openServerCommand = OpenServerCommand(threads);
    Command* openServerCommand2 = &openServerCommand;
    map.emplace(std::make_pair("openDataServer", openServerCommand2));
    ConnectCommand connectCommand = ConnectCommand();
    Command* connectCommand2 = &connectCommand;
    map.emplace(std::make_pair("connectControlClient", connectCommand2));
    DefineVarCommand defineVarCommand = DefineVarCommand();
    Command* defineVarCommand2 = &defineVarCommand;
    map.emplace(std::make_pair("var", defineVarCommand2));
    SleepCommand sleepCommand = SleepCommand();
    Command* sleepCommand2 = &sleepCommand;
    map.emplace(std::make_pair("Sleep", sleepCommand2));
    PrintCommand printCommand = PrintCommand();
    Command* printCommand2 = &printCommand;
    map.emplace(std::make_pair("Print", printCommand2));
    // Initializing the vector
    vector<string> fileVector;
    fileVector = lexer();

    Command* c = map.at("Print");
    std::cout <<"here 0 : " << typeid(c).name() << '\n';
    int k= c->execute(fileVector,1);
    (map.at("Sleep"))->execute(fileVector,1);
    (map.at("var"))->execute(fileVector,1);
    (map.at("openDataServer"))->execute(fileVector,0);
    threads[0].join();
    //(map.at("Sleeping"))->execute(fileVector,0);
    int i = 0;
/*
    //Command* c {map.at("OpenDataServer")};
    std::cout << "in main - " << typeid(c).name() << '\n';
    //Command& c = map.at("Sleep");
    //std::cout << typeid(c1).name() << '\n';

*/
    //if (c != NULL)
       // i += c->execute(fileVector, i);


    //OpenServerCommand *openServerCommand3 = new OpenServerCommand;
    //i += openServerCommand3->execute(fileVector, i);


    //ConnectCommand *connectCommand = new ConnectCommand;
    //connectCommand->execute(fileVector, i);
    // Noam Testing line 287 bkbkjbkjb

































    // End of Noam Testing till 321

    // Karin Testing line 323

    //OpenServerCommand *openServerCommand1 = new OpenServerCommand;
    //i += openServerCommand1->execute(fileVector, i);






























    // End of Karin Testing 357



    return 0;
}
