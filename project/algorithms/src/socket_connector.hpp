#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>


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
    void receiveData();
};