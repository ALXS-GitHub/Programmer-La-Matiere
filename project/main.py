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

server_address = ('localhost', 6969)
server_socket.bind(server_address)

server_socket.listen(1)

print("Attente de la connexion du client...")
while True:
    connection, client_address = server_socket.accept()

    try:
        print("Connexion établie avec", client_address)
        
        # ? Matrice "jouet" pour tester la communication
        # TODO: ici, il faut lancer une itération d'entrainement, puis remplacer data par la matrice des poids du nouveau réseau à éval
        data = generate_random_weights(2, 25, 125, 27, False)
        
        connection.sendall(str(data.size).encode())
        time.sleep(1)
        connection.sendall(data.tobytes())
        # time.sleep(1)
        print(data)

    finally:
        connection.close()
        
        
    break