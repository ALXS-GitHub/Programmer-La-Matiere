#include "../src/socket_connector.hpp"

int main() {
    SocketClient client;
    if (client.connectToServer()) {
        client.receiveData();
    }
    return 0;
}