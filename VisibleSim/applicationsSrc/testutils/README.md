# Explications

Le but du module `testutils` est de fournir des fonctions utilitaires pour pouvoir utiliser le modèle de manière plus générale.

## Fonctions

### `void computePossibleMoves()`

Cette fonction permet de calculer les mouvements possibles pour le robot. Elle utilise le nouvel attribut `map<vector<int>,int> possibleMoves` de la classe pour stocker les mouvements possibles. La clé correspond au mouvement et la valeur correspond à l'index du mouvement dans le nouvel attribut `vector<vector<int>> NNMovesMapping` de la classe. `NNMovesMapping` est un vecteur qui permet d'indexer les mouvements possibles pour pouvoir interpréter les sorties du réseau de neurones.

### `int getBestMoveIndex(vector<double> output)`

Cette fonction permet de récupérer l'index du meilleur mouvement possible correspondant à la sortie du réseau de neurones. Elle prend en compte la map `possibleMoves` pour utiliser uniquement les mouvements possibles. Attention la valeur renvoyée par cette fonction correspond bien à l'index de la sortie du réseau de neurones et non à l'index du mouvement dans `NNMovesMapping`.

### `void moveFromOutput(int index)`

Cette fonction permet de déplacer le robot en fonction de l'index de sortie du réseau de neurones. Elle utilise la map `possibleMoves` pour récupérer le mouvement correspondant à l'index.

Réseau de neurones -> index du meilleur mouvement (output) -> mouvement (ex (1,0,0)) -> index du déplacement (possibleMoves ou motions) -> Mouvement

### `vector<vector<int>> getEndOfSimulationPositions()`

Cette fonction permet de récupérer les positions finales des robots `leader` à la fin de la simulation.

