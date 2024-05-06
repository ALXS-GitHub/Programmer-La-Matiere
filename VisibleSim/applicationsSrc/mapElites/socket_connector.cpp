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

void SocketClient::receiveData() {
    // Receive the size of the 1D array
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, 1024, 0);
    if (bytes_received <= 0) {
        cout << "Failed to receive size: " << strerror(errno) << endl;
        return;
    }
    string size_str(buffer);
    cout << "Size: " << size_str << endl;

    // Receive the data of the 1D array
    int size;
    sscanf(size_str.c_str(), "%d", &size);
    size_t data_size = size * sizeof(double);
    char *data_buffer = new char[data_size];
    bytes_received = recv(client_socket, data_buffer, data_size, 0);
    if (bytes_received <= 0) {
        cout << "Failed to receive data: " << strerror(errno) << endl;
        delete[] data_buffer;
        return;
    }

    // Convert the data to a 1D array
    double *data = reinterpret_cast<double *>(data_buffer);
    for (int i = 0; i < size; ++i) {
        cout << data[i] << " ";
    }
    cout << endl;

    // Free the memory
    delete[] data_buffer;
}

vector<double> SocketClient::receiveDataVector() {
    // Receive the size of the 1D array
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, 1024, 0);
    if (bytes_received <= 0) {
        cout << "Failed to receive size: " << strerror(errno) << endl;
        return vector<double>();
    }
    string size_str(buffer);
    cout << "Size: " << size_str << endl;

    // Receive the data of the 1D array
    int size;
    sscanf(size_str.c_str(), "%d", &size);
    size_t data_size = size * sizeof(double);
    char *data_buffer = new char[data_size];
    if (!data_buffer) {
        cout << "Failed to allocate memory" << endl;
        return vector<double>();
    }
    bytes_received = recv(client_socket, data_buffer, data_size, 0);
    if (bytes_received <= 0) {
        cout << "Failed to receive data: " << strerror(errno) << endl;
        delete[] data_buffer;
        return vector<double>();
    }

    // Convert the data to a 1D array
    double *data = reinterpret_cast<double *>(data_buffer);

    for (int i = 0; i < size; ++i) {
        cout << data[i] << " ";
    }
    cout << endl;

    vector<double> data_vector(data, data + size);

    // Free the memory
    delete[] data_buffer;
    data_buffer = nullptr;

    return data_vector;
}
