# Explications

Ici le dossier s'appelle test2NN, il sert a tester un robot avec réseau de neurones dans un environnement contenant des obstacles.

## Note importante sur des tests réalisés

Lors de la recherche de l'ensemble des mouvements `vector<std::pair<const Catoms3DMotionRulesLink*, Catoms3DRotation>> motions = Catoms3DMotionEngine::getAllRotationsForModule(module)`

Cette fonction revoie un vecteur de l'ensemble des mouvements du robot Dans l'ordre de la liste des mouvements (cf `/resources/catomsMotions.png`) Par exemple, peu importe les ports connectés du robot actuel, il suivra toujours le mouvement voulu selon la liste des mouvements.

-> Dernière affirmation à confirmer (car si tous les mouvements ne sont pas possibles, sont-ils toujours dans le même ordre ? idem si il y a des montées ou des descentes, est-ce que l'ordre est toujours le même ?)

## Choses que j'ai appris avec des obstacles

- `vector<std::pair<const Catoms3DMotionRulesLink*, Catoms3DRotation>> motions = Catoms3DMotionEngine::getAllRotationsForModule(module)` ne renvoie pas les mouvements impossibles (donc <= 12 mouvements)

- Les mouvements peuvent être plus compliqué à comprendre que prévu (notamment avec les obstacles)

Quelques fonctions qui renvoient des données utiles :

- `Cell3DPosition position = module->position;` : renvoie la position du module actuel.

- `int pivotPort = findNeighborPort((*motion).second.pivot);` : renvoie le port du module actuel qui peut servir de pivot (ce port est donc déjà connecté, il s'agit du port de départ en quelque sorte)

- `int pivotPort = (*motion).first->getConFromID()` : idem que la fonction précédente

- `int targetPort = (*motion).first->isOctaFace()` : renvoie le port du module actuel qui peut servir de cible (ce port n'est pas connecté, il s'agit du port d'arrivée en quelque sorte)

```cpp
Cell3DPosition finalPos;
short finalOrient;  
(*motion).second.getFinalPositionAndOrientation(finalPos, finalOrient);
```

- `finalPos` : position finale du module actuel après le mouvement
- `finalOrient` : orientation finale du module actuel après le mouvement (spécifique au simulateur, je n'ai pas encore trop de détails sur cette valeur)

## Mouvements plus complexes : 

sur un terrain plat, on a la liste des mouvements suivant (voir `/resources/catomsMotions.png` pour comprendre pourquoi) : 

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

Mais avec des obstacles, on peut avoir des mouvements plus complexes, incluant des montées et des descentes, par exemple :

- (1,1,1)
- (0,1,1)
- (1,0,1)
...

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

## Comment renvoyer les données à la fin de la simulation ?

Il faut déjà collecter les données des leaders : 

```cpp
void test2NNCode::onEndOfSimulation() {
    // !! the onEndOfSimulation function is called at the end of the simulation for only ONE module !!
    console << "End of simulation" << "\n";
    cout << "End of simulation" << endl;
    
    // ? so we need to get all the modules from the world
    map<bID, BaseSimulator::BuildingBlock *> modules = BaseSimulator::getWorld()->getMap();
    // get all leaders
    for (auto &elem : modules) { // ? iterate over all modules
        test2NNCode *blockCode = dynamic_cast<test2NNCode *>(elem.second->blockCode); // ? typecast the blockcode to test2NNCode to access the isLeader variable (and specific module data)
        if (blockCode->isLeader) { // ? check if the module is a leader
            console << "Leader Id : " << elem.first << "\n";
            cout << "Leader Id : " << elem.first << endl;
            blockCode->module->setColor(WHITE);
            console << "Final position : " << blockCode->module->position << "\n"; // ? print the final position of the leader
            cout << "Final position : " << blockCode->module->position << endl;
            console << "Module Id" << elem.first << "\n";
            cout << "Module Id" << elem.first << endl;
        }
    }
}
```

On verra plus tard pour envoyer ces données depuis le socket.