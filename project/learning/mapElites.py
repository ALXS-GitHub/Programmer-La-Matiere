import numpy as np
import pickle as pkl

class Archive:
    def __init__(self, cols,rows):
        self.cols = cols
        self.rows = rows
        self.archive = [[None for _ in range(cols)] for _ in range(rows)]
        self.archive = np.array(self.archive)
    
    def add(self, individual, x, y, energy):
        if self.archive[x][y] is None:
            self.archive[x][y] = (individual,energy)
        else:
            _,prev_energy = self.archive[x][y]
            if energy > prev_energy: # le critère conservé ici est l'énergie
                self.archive[x][y] = (individual,energy)
    
    def get_random_individual(self,noise):
        x = np.random.randint(0, self.cols)
        y = np.random.randint(0, self.rows)
        while self.archive[x][y] is None:
            x = np.random.randint(0, self.cols)
            y = np.random.randint(0, self.rows)
        individual = self.archive[x][y]
        noise = np.random.normal(0, noise, individual.shape)
        individual += noise
        return individual


class MapElites:
    def __init__(self, config,cols,rows, noise=0.1):
        self.shape = config.shape
        self.noise = noise
        self.archive = Archive(cols=cols, rows=rows)
        self.individuals_added = 0
    
    def register_results(self, individual, x, y, energy):
        self.archive.add(individual, x, y, energy)
        self.individuals_added += 1

    def get_next_individual(self):
        if self.individuals_added == 0:
            self.individuals_added += 1
            return np.random.normal(0, 10, self.shape)
        newIndividual = self.archive.get_random_individual(self.noise)
        return newIndividual.reshape(self.shape)
    
    def displayHeatmap(self):
        # à voir comment afficher la heatmap
        pass

    def export_to_file(self, filename):
        """Export the archive to a file using pickle."""
        with open(filename, 'wb') as f:
            pkl.dump(self, f)
    
    def load_from_file(self, filename):
        """Load the archive from a file using pickle."""
        with open(filename, 'rb') as f:
            self = pkl.load(f)



    