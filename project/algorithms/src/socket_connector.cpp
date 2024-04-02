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
    // Recevoir la forme de la matrice 3D
    char buffer[4096];
    recv(client_socket, buffer, 4096, 0);
    string shape_str(buffer);
    cout << "Shape: " << shape_str << endl;

    // Recevoir les données de la matrice 3D
    int shape[3];
    sscanf(shape_str.c_str(), "(%d, %d, %d)", &shape[0], &shape[1], &shape[2]);
    size_t data_size = shape[0] * shape[1] * shape[2] * sizeof(double);
    char *data_buffer = new char[data_size];
    recv(client_socket, data_buffer, data_size, 0);

    // Convertir les données en une matrice 3D
    double *data = reinterpret_cast<double *>(data_buffer);
    for (int i = 0; i < shape[0]; ++i) {
        for (int j = 0; j < shape[1]; ++j) {
            for (int k = 0; k < shape[2]; ++k) {
                cout << data[i * shape[1] * shape[2] + j * shape[2] + k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    // Libérer la mémoire
    delete[] data_buffer;
}
