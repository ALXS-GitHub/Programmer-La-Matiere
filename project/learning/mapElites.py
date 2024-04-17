import numpy as np

class Archive:
    def __init__(self, cols,rows):
        self.cols = cols
        self.rows = rows
        self.archive = [[None for _ in range(cols)] for _ in range(rows)]
        self.archive = np.array(self.archive)
    
    def add(self, individual, x, y):
        self.archive[x][y] = np.array(individual).flatten()
    
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
    
    def register_results(self, individual, x, y):
        # TODO : Utiliser les fonctions d'éval de Michaël pour évaluer l'individu et donc déterminer ses coordonnées dans l'archive.
        self.archive.add(individual, x, y)

    def get_next_individual(self):
        newIndividual = self.archive.get_random_individual(self.noise)
        return newIndividual.reshape(self.shape)
    
    def displayHeatmap(self):
        # à voir comment afficher la heatmap
        pass



    