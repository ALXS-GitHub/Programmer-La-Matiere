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

- `int pivotPort = findNeighborPort((*motion).second.pivot);` : renvoie le port du module actuel qui peut servir de pivot (ce port est donc déjà connecté, il s'agit du port de départ en quelque sorte)

- `int targetPort = (*motion).first->isOctaFace()` : renvoie le port du module actuel qui peut servir de cible (ce port n'est pas connecté, il s'agit du port d'arrivée en quelque sorte)

```cpp
Cell3DPosition finalPos;
short finalOrient;  
(*motion).second.getFinalPositionAndOrientation(finalPos, finalOrient);
```

- `finalPos` : position finale du module actuel après le mouvement
- `finalOrient` : orientation finale du module actuel après le mouvement (spécifique au simulateur, je n'ai pas encore trop de détails sur cette valeur)

