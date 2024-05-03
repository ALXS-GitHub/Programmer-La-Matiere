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

### `map<bID, BaseSimulator::BuildingBlock *> getLeaders()`

Cette fonction permet de récupérer les robots `leader` de la simulation. Elle utilise la méthode `BaseSimulator::getWorld()->getMap()` pour récupérer la map des robots de la simulation. Elle parcourt ensuite cette map pour récupérer les robots `leader`.

### `vector<vector<int>> getEndOfSimulationPositions()`

Cette fonction permet de récupérer les positions finales des robots `leader` à la fin de la simulation.

### `vector<bool> getRobotsArround(int radius)`

Cette fonction permet de récupérer les robots autour du robot courant dans un rayon donné. Elle utilise la méthode `BaseSimulator::getWorld()->getMap()` pour récupérer la map des robots de la simulation. Elle parcourt ensuite cette map pour récupérer les robots autour du robot courant en utilisant la distance dans le cube.

## Classe `Utils` 

IMPORTANT

Cette classe est une classe qui devrait normalement faire partie du simulateur mais pour des raisons de simplicité (et pour éviter de modifier le code du simulateur) on la créée au dessus du code du robot. Cette classe contient uniquement des méthodes statiques (donc pas besoin d'instancier la classe) qui permettent de récupérer des informations sur la simulation et de faire communiquer des informations importantes entre les robots (par exemple les position où les robots choisissent de se rendre, ce qui permet d'éviter les collisions).

### `uint64_t Utils::motionsProcessed`

Cet attribut permet de compter le nombre de mouvements traités par les robots.

### `uint64_t Utils::getMotionsProcessed()`

Cette fonction permet de récupérer le nombre de mouvements traités par les robots.

### `void Utils::incrementMotionsProcessed()`

Cette fonction permet d'incrémenter le nombre de mouvements traités par les robots.

### `struct CoordinatesHash` et `unordered_set<vector<int>, Utils::CoordinatesHash> Utils::takenDestinations`

Cette structure permet de créer un hash pour les coordonnées des robots. Elle est utilisée pour stocker les positions des robots dans un `unordered_set`.

L'attribut `takenDestinations` est un `unordered_set` qui permet de stocker les positions où les robots choisissent de se rendre pour leur prochain déplacement.

Pourquoi un `unordered_set` ? Parce que c'est une structure de données qui permet de stocker des éléments uniques et de vérifier si un élément est déjà présent en temps constant. C'est donc très pratique pour stocker les positions des robots et vérifier si un robot est déjà à une position donnée.

Pourquoi faire un hash des coordonnées ? Parce que les coordonnées sont un vecteur et que les vecteurs ne sont pas des types de données primitifs. Il faut donc définir une fonction de hash pour pouvoir les stocker dans un `unordered_set`.

### `void Utils::addTakenDestination(vector<int> destination)`

Cette fonction permet d'ajouter une destination à `takenDestinations`.

### `void Utils::removeTakenDestination(vector<int> destination)`

Cette fonction permet de retirer une destination de `takenDestinations`.

### `bool Utils::isDestinationTaken(vector<int> destination)`

Cette fonction permet de vérifier si une destination est déjà prise par un robot.

### `void Utils::printTakenDestinations()`

Cette fonction permet d'afficher les destinations prises par les robots. Elle est utile pour le debugging.

### `int Utils::getCubeDistance(vector<int> pos1, vector<int> pos2)`

Cette fonction permet de calculer la distance entre deux robots dans un cube (on prend le maximum des distances entre les coordonnées x, y et z).
