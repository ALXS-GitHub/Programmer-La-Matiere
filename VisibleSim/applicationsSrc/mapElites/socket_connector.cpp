#include "socket_connector.hpp"

SocketClient::SocketClient() {
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        cerr << "Erreur lors de la création du socket" << endl;
        return;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
}

SocketClient::~SocketClient() {
    close(client_socket);
}

bool SocketClient::connectToServer() {
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        cerr << "Erreur lors de la connexion au serveur (ne pas oublier de lancer le server python avant...)" << endl;
        return false;
    }
    cout << "Connecté au serveur" << endl;
    return true;
}

bool SocketClient::isConnected() {
    return client_socket != -1;
}

void SocketClient::sendData(const char* data, const string& tag) {

    cout << "Sending data: " << data << endl;

    // Get the length of the data
    uint32_t length = strlen(data);

    // Convert the length to network byte order
    uint32_t length_net = htonl(length);

    // Send the length
    send(client_socket, &length_net, sizeof(length_net), 0);

    // Send the data
    send(client_socket, data, length, 0);

    // Set a timeout of 5 seconds
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0) {
        cout << "Failed to set timeout: " << strerror(errno) << endl;
        return;
    }

    // Receive an acknowledgment
    char ack[1024];
    int bytes_received = recv(client_socket, ack, 1024, 0);
    if (bytes_received <= 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            cout << "Timeout waiting for ACK for" << tag << endl;
        } else {
            cout << "Failed to receive acknowledgment: " << strerror(errno) << endl;
        }
        return;
    }

    ack[bytes_received] = '\0'; // Null-terminate the string
    string ack_str(ack);

    if (ack_str != "ACK:" + tag) {
        cout << "Failed to receive ACK for " << tag << ", got" << ack_str << endl;
        return;
    }
}

char* SocketClient::receiveData(const size_t &size, const string& tag) {
    // Receive the data
    char* buffer = new char[size + 1];
    if (!buffer) {
        cout << "Failed to allocate memory" << endl;
        return nullptr;
    }
    int bytes_received = recv(client_socket, buffer, size, 0);
    if (bytes_received <= 0) {
        cout << "Failed to receive message: " << strerror(errno) << endl;
        delete[] buffer;
        return nullptr;
    }

    // Send an acknowledgment
    string ack = "ACK:" + tag;
    send(client_socket, ack.c_str(), ack.size(), 0);

    buffer[bytes_received] = '\0'; // Null-terminate the string
    return buffer;
}

vector<double> SocketClient::receiveDataVector() {
    // Receive the size of the 1D array
    
    string size_str = receiveData(1024, "SIZE");
    size_t size = stoul(size_str);
    cout << "Size of the 1D array: " << size << endl;
    // Receive the data of the 1D array
    char* buffer = receiveData(size * sizeof(double), "WEIGHTS");
    if (!buffer) {
        return vector<double>();
    }

    double* data = (double*)buffer;

    // for (int i = 0; i < size; ++i) {
    //     cout << data[i] << " ";
    // }
    // cout << endl;

    vector<double> data_vector(data, data + size);

    delete[] buffer;

    return data_vector;
}
