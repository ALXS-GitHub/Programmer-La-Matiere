import socket
import numpy as np
import time

def generate_random_weights(num_hidden_layers, num_neurons_per_hidden_layer, num_inputs, num_outputs, use_bias):
    weights = []

    for i in range(num_hidden_layers + 1):
        if i == 0:
            weights.append(np.random.rand(num_neurons_per_hidden_layer, num_inputs + (1 if use_bias else 0)))
        elif i == num_hidden_layers:
            weights.append(np.random.rand(num_outputs, num_neurons_per_hidden_layer + (1 if use_bias else 0)))
        else:
            weights.append(np.random.rand(num_neurons_per_hidden_layer, num_neurons_per_hidden_layer + (1 if use_bias else 0)))

    weights = np.concatenate([arr.flatten() for arr in weights])
    return weights

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Set the SO_REUSEADDR option
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

server_address = ('localhost', 6969)
server_socket.bind(server_address)

server_socket.listen(1)

# TODO: il faudra peut-être rendre les send et recv en non bloquants pour pouvoir envoyer et recevoir en même temps à plusieurs clients
def send_data_with_ack(connection, data, tag):
    connection.sendall(data)
    connection.settimeout(5)
    
    try:
        ack = connection.recv(1024).decode()
        if ack != f"ACK:{tag}":
            raise Exception(f"Failed to receive ACK for {tag}")
    except socket.timeout:
        raise Exception(f"Timeout waiting for ACK for {tag}")

def receive_data(connection):
    data_size = int(connection.recv(16).decode())
    data_buffer = b""
    while len(data_buffer) < data_size:
        more_data = connection.recv(data_size - len(data_buffer))
        if not more_data:
            raise Exception("Failed to receive all data")
        data_buffer += more_data

    # Deserialize the received data
    received_data = pickle.loads(data_buffer)
    return received_data

print("Attente de la connexion du client...")
while True:
    connection, client_address = server_socket.accept()

    try:
        print("Connexion établie avec", client_address)
        
        # ? Matrice "jouet" pour tester la communication
        # TODO: ici, il faut lancer une itération d'entrainement, puis remplacer data par la matrice des poids du nouveau réseau à éval
        data = generate_random_weights(2, 25, 125, 27, False)
        
        send_data_with_ack(connection, str(data.size).encode(), "SIZE")
        send_data_with_ack(connection, data, "WEIGHTS")
        print(data)
        
        # Wait for the client to send data
        # received_data = receive_data(connection)
        # print(received_data)

    finally:
        connection.close()
        
        
    break