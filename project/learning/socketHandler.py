import socket
import numpy as np

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
        data = np.random.rand(3, 3, 3)
        print(data)
        
        connection.sendall(str(data.shape).encode())
        
        connection.sendall(data.tobytes())

    finally:
        connection.close()