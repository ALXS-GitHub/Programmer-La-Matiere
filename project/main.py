import socket
import numpy as np
import time
import os
import subprocess
import struct
import concurrent.futures
from learning.mapElites import MapElites
import math


# set the current working directory to the file's directory
os.chdir(os.path.dirname(os.path.abspath(__file__)))

class Master:
    def __init__(self, host='localhost', port=6969, executable="../VisibleSim/applicationsBin/mapElites/mapElites", executable_config="../VisibleSim/applicationsBin/mapElites/config.xml"):
        # server configuration
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((host, port))
        self.server_socket.listen(1)

        # other configuration
        self.executable = executable
        self.executable_config = executable_config
        self.weights = None

        self.num_hidden_layers = 2
        self.num_neurons_per_hidden_layer = 25
        self.num_inputs = 125
        self.num_outputs = 27
        self.map_elites = MapElites({"size":self.num_inputs + self.num_outputs + self.num_hidden_layers*self.num_neurons_per_hidden_layer}, 80, 80, 1.5)
        temp = self.map_elites.load_from_file("mapElites.pkl")
        if temp is not None:
            self.map_elites = temp
        self.map_elites.set_noise(10)
    def unbind_socket(self):
        """
        Unbind the socket
        """
        self.server_socket.close()


    def load_weights(self, filename):
        """
        Load the weights from a file
        
        Args:
            filename (str): The name of the file to load the weights from
        """
        weights = np.loadtxt(filename)
        self.weights = weights
        return weights

    def generate_random_weights(self, num_hidden_layers, num_neurons_per_hidden_layer, num_inputs, num_outputs, use_bias, save=False):
        """
        Generate random weights for a neural network
        
        Args:
            num_hidden_layers (int): The number of hidden layers
            num_neurons_per_hidden_layer (int): The number of neurons per hidden layer
            num_inputs (int): The number of inputs
            num_outputs (int): The number of outputs
            use_bias (bool): If True, add a bias to the neurons
        """
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
        if save:
            np.savetxt('logs/weights.log', weights)
        
        self.weights = weights
        return weights

    def generate_weights(self):
        """
        Generate weights for a neural network
        
        Args:
            N/A
        """
        
        return self.map_elites.get_next_individual()

    # TODO: il faudra peut-être rendre les send et recv en non bloquants pour pouvoir envoyer et recevoir en même temps à plusieurs clients
    def send_data(self, connection, data, tag):
        """
        Send data to the client and wait for an ACK

        Args:
            connection (socket): The connection to the client
            data (bytes): The data to send
            tag (str): The tag to check the ACK
        """
        connection.sendall(data)
        connection.settimeout(60)
        
        try:
            ack = connection.recv(1024).decode()
            if ack != f"ACK:{tag}":
                raise Exception(f"Failed to receive ACK for {tag}")
        except socket.timeout:
            raise Exception(f"Timeout waiting for ACK for {tag}")

    def receive_data(self, connection, tag):
        """
        Receive data from the client and send an ACK
        In details, it first receives the length of the data, then the data itself
        
        Args:
            connection (socket): The connection to the client
            tag (str): The tag to send in the ACK
        """
        length_bytes = connection.recv(4)
        length = struct.unpack('!I', length_bytes)[0]
        
        data = b''
        data = connection.recv(length)
        
        connection.sendall(f"ACK:{tag}".encode())
        return data

    def print_subprocess_output(self, process):
        """
        Print the output of a subprocess

        Args:
            process (subprocess.Popen): The subprocess to get the output from
        """
        print("SUBPROCESS")
        stdout, stderr = process.communicate()
        print(stdout.decode())
        print(stderr.decode())

    def run(self, auto=True):
        """
        Run the server and wait for a client to connect
        
        Args:
            auto (bool): If True, the client will be automatically started
        """
        print("Attente de la connexion du client...")
        # !!! from the python code, only run this in terminal mode
        if auto:
            process = subprocess.Popen([self.executable, "-c", self.executable_config, "-t"], start_new_session=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        connection, client_address = self.server_socket.accept()
        
        # Run the client

        try:
            print("Connexion établie avec", client_address)
            
            # ? Matrice "jouet" pour tester la communication
            # TODO: ici, il faut lancer une itération d'entrainement, puis remplacer data par la matrice des poids du nouveau réseau à éval
            #data = self.generate_random_weights(2, 25, 125, 27, False)
            data = self.generate_weights()
            #data,_ = self.map_elites.get_custom_individual(4, 6) # sert à récupérer un individu spécifique dans la map
            # data = load_weights('logs/weights_error.log')
            
            self.send_data(connection, str(data.size).encode(), "SIZE")
            self.send_data(connection, data, "WEIGHTS")
            
            
            
            # Wait for the client to send data
            try:
                # size_to_receive = int(receive_data(connection, 4, "SIZE").decode()) # here size to receive already takes into account the size of the data in bytes
                received_data = self.receive_data(connection, "POSITIONS").decode()
                nb_of_moves = self.receive_data(connection, "TOTAL_MOVES").decode()
            except Exception as e:
                print(f"An error occurred: {e}")
                # print_subprocess_output(process);
            
            # received_data should be constitued of vectors of size 3
            received_data = received_data.split(" ")
            received_data = [int(x) for x in received_data if x != ""]
            received_data = np.array(received_data).reshape(-1, 3)
            
            # TODO : Calculer les coordonnées dans la map en fonction de received_data
            # ! Attention à bien scale les coordonnées. Par exemple si le max théorique pour une dimension est 100 et que la map a pas 100 cases, il faut normaliser les coordonnées entre 0 et 1 puis les multiplier par le nombre de cases de la map
            #Point de comparaison
            level,number_robot_level = self.number_robot(received_data) #nombre par niveau
            nb_amas = len(self.amas_robot(received_data))
            
            print("NB moves ", nb_of_moves)
            print("Positions \n", received_data)
            print("Level |", level)
            print("Robot |", number_robot_level)
            print("Number of robots on base :", number_robot_level[0])
            print("Max height :", level[-1])
            print("Amas de robots : ", nb_amas)
            x_coord = min(level[-1],80)-1
            y_coord = min(nb_amas,80)-1
            self.map_elites.register_results(data, x_coord, y_coord, nb_of_moves)
            print("Pushing neural network to mapElites at coordinates", x_coord, y_coord, "with nb of moves", nb_of_moves)


        finally:
            connection.close()
            self.map_elites.displayHeatmap()
            self.map_elites.export_to_file("mapElites.pkl")
        
    # TODO : change this function to work with mapElites
    # in the future, !be careful! with this function (! to avoid writing in the same memory space !)
    def run_parallel(self, steps = 50):
        with concurrent.futures.ThreadPoolExecutor() as executor:
            for i in range(steps):
                executor.submit(self.run)


    #calculer le nombre de robots sur le tapis, z = 1
    def number_robot(self, data_position):

        level = []
        number_robot = []
        for i in range(len(data_position)):
            if data_position[i][2] > len(level):
                for j in range(len(level) + 1, data_position[i][2] + 1):
                    level.append(j)
                    number_robot.append(0)
        
            number_robot[data_position[i][2] - 1] += 1

        return level, number_robot

    def distance(self, robot1, robot2):
        return math.sqrt((robot1[0] - robot2[0])**2 + (robot1[1] - robot2[1])**2 + (robot1[2] - robot2[2])**2)

    def voisin(self, r1, r2) :
        return self.distance(r1,r2) <= 1

    def amas_robot(self, data_position):
        data = data_position.copy()
        amas = []
        while len(data) > 0:
            amas.append([data[0]])
            data = data[1:]
            i = 0
            while i < len(amas[-1]):
                j = 0
                while j < len(data):
                    if self.voisin(amas[-1][i], data[j]):
                        amas[-1].append(data[j])
                        data = np.delete(data, j, axis=0)
                    else:
                        j += 1
                i += 1
        return amas
if __name__ == "__main__":
    for i in range(10000):
        try:
            master = Master()
            #master.run_parallel(50) # ps ici les outputs sont mélangés, mais c'est normal (si vous voulez les voir dans l'ordre faite une boucle for avec master.run())
            master.run(auto=True)
            master.unbind_socket()
        except Exception as e:
            print(f"An error occurred: {e}")
            master.unbind_socket()
    
