#ifndef SOCKET_CONNECTOR_HPP
#define SOCKET_CONNECTOR_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>


using namespace std;

#define PORT 6969

class SocketClient {
private:
    int client_socket;
    struct sockaddr_in server_address;

public:
    SocketClient();
    ~SocketClient();
    bool connectToServer();
    char* receiveData(const size_t &size, const string& tag);
    vector<double> receiveDataVector();
};

#endif // SOCKET_CONNECTOR_HPP