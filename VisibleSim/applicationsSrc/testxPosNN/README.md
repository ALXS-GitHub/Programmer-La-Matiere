# Explications

sur terrain plat :

- (1,0,0)
- (1,0,0)
- (0,1,0)
- (0,1,0)
- (-1,0,0)
- (-1,0,0)
- (0,-1,0)
- (0,-1,0)
- (-1,-1,0)
- (-1,1,0)
- (1,-1,0)
- (1,1,0)

Pour se représenter ces mouvements, on peux créer un mappage en base 3, où 0 = 0, 1 = 1, 2 = -1, et on a donc :

- 0 = 0,0,0
- 1 = 1,0,0
- 2 = -1,0,0
- 3 = 0,1,0
- 4 = 1,1,0
- 5 = -1,1,0
- 6 = 0,-1,0
- 7 = 1,-1,0
- 8 = -1,-1,0
- 9 = 0,0,1
- 10 = 1,0,1
- 11 = -1,0,1
- 12 = 0,1,1
- 13 = 1,1,1
- 14 = -1,1,1
- 15 = 0,-1,1
- 16 = 1,-1,1
- 17 = -1,-1,1
- 18 = 0,0,-1
- 19 = 1,0,-1
- 20 = -1,0,-1
- 21 = 0,1,-1
- 22 = 1,1,-1
- 23 = -1,1,-1
- 24 = 0,-1,-1
- 25 = 1,-1,-1
- 26 = -1,-1,-1

soit 27 mouvements (reste à voir s'il sont tous possibles)

Avec cette représentation il est plus facile de coder les mouvements et donc de les entrer dans le réseau de neurones.

## But

Ici le but est de faire avancer le robot jusqu'à une position donnée, en utilisant un réseau de neurones.

On choisit la position x = 10.

Les poids déterminés à l'avance sont les suivants : 

`vector<vector<vector<double>>> weights = {{{-1,10.5},{-1,0},{1,-10.5},{-1,0},{0.0501,0}}};`

On a une seule entrée : la position x du robot. On rajoute un biais sur toutes les couches (sauf la dernière bien sur). On a 5 sorties :
Le robot avance en x+1, y+1, x-1, y-1, ou il s'arrête.

ATTENTION, par convention les biais sont toujours en dernier dans les vecteurs de poids.

Par de simple calculs on peut voir que le réseau de neurones est capable de faire avancer le robot jusqu'à la position x = 10 et de l'arrêter là.