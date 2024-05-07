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
def send_data(connection, data, tag):
    """
    Send data to the client and wait for an ACK

    Args:
        connection (socket): The connection to the client
        data (bytes): The data to send
        tag (str): The tag to check the ACK
    """
    connection.sendall(data)
    connection.settimeout(5)
    
    try:
        ack = connection.recv(1024).decode()
        if ack != f"ACK:{tag}":
            raise Exception(f"Failed to receive ACK for {tag}")
    except socket.timeout:
        raise Exception(f"Timeout waiting for ACK for {tag}")

def receive_data(connection, size, tag):
    """
    Receive data from the client and send an ACK

    Args:
        connection (socket): The connection to the client
        size (int): The size of the data to receive
        tag (str): The tag to send the ACK
    """
    connection.settimeout(None)
    
    # recieve data
    received_data = b''
    received_data = connection.recv(size)
    
    # send ACK
    connection.sendall(f"ACK:{tag}".encode())

    print(received_data)
    return received_data

print("Attente de la connexion du client...")
while True:
    connection, client_address = server_socket.accept()

    try:
        print("Connexion établie avec", client_address)
        
        # ? Matrice "jouet" pour tester la communication
        # TODO: ici, il faut lancer une itération d'entrainement, puis remplacer data par la matrice des poids du nouveau réseau à éval
        data = generate_random_weights(2, 25, 125, 27, False)
        
        send_data(connection, str(data.size).encode(), "SIZE")
        send_data(connection, data, "WEIGHTS")
        print(data)
        
        # Wait for the client to send data
        size_to_receive = int(receive_data(connection, 1024, "SIZE").decode()) # here size to receive already takes into account the size of the data in bytes
        received_data = receive_data(connection, size_to_receive, "POSITIONS").decode()
        nb_of_moves = receive_data(connection, 1024, "TOTAL_MOVES").decode()
        
        # received_data should be constitued of vectors of size 3
        received_data = received_data.split(" ")
        received_data = [int(x) for x in received_data if x != ""]
        
        print("NB moves ", nb_of_moves)
        print("Positions ", received_data)

    finally:
        connection.close()
        
        
    break