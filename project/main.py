import socket
import numpy as np
import time
import os
import subprocess
import struct

# set the current working directory to the file's directory
os.chdir(os.path.dirname(os.path.abspath(__file__)))

def load_weights(filename):
    weights = np.loadtxt(filename)
    return weights

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
    
    # save the weights in a file for debbuging
    np.savetxt('weights.log', weights)
    
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

def receive_data(connection, tag):
    length_bytes = connection.recv(4)
    length = struct.unpack('!I', length_bytes)[0]
    
    data = b''
    data = connection.recv(length)
    
    connection.sendall(f"ACK:{tag}".encode())
    return data

def print_subprocess_output(process):
    """
    Print the output of a subprocess

    Args:
        process (subprocess.Popen): The subprocess to get the output from
    """
    print("SUBPROCESS")
    stdout, stderr = process.communicate()
    print(stdout.decode())
    print(stderr.decode())

print("Attente de la connexion du client...")
# !!! from the python code, only run this in terminal mode
i = 50

while i > 0:
    process = subprocess.Popen(["../VisibleSim/applicationsBin/mapElites/mapElites", "-c", "../VisibleSim/applicationsBin/mapElites/config.xml", "-t"], start_new_session=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    connection, client_address = server_socket.accept()
    
    # Run the client

    try:
        print("Connexion établie avec", client_address)
        
        # ? Matrice "jouet" pour tester la communication
        # TODO: ici, il faut lancer une itération d'entrainement, puis remplacer data par la matrice des poids du nouveau réseau à éval
        data = generate_random_weights(2, 25, 125, 27, False)
        # data = load_weights('weights_error.log')
        
        send_data(connection, str(data.size).encode(), "SIZE")
        send_data(connection, data, "WEIGHTS")
        print(data)
        
        
        
        # Wait for the client to send data
        try:
            # size_to_receive = int(receive_data(connection, 4, "SIZE").decode()) # here size to receive already takes into account the size of the data in bytes
            received_data = receive_data(connection, "POSITIONS").decode()
            nb_of_moves = receive_data(connection, "TOTAL_MOVES").decode()
        except Exception as e:
            print(f"An error occurred: {e}")
            # print_subprocess_output(process);
        
        # received_data should be constitued of vectors of size 3
        received_data = received_data.split(" ")
        received_data = [int(x) for x in received_data if x != ""]
        received_data = np.array(received_data).reshape(-1, 3)
        
        
        
        print("NB moves ", nb_of_moves)
        print("Positions \n", received_data)

    finally:
        connection.close()
        
        
    i -= 1