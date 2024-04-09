# Explications

Ce test cherche à faire tourner un robot sur lui-même grace à un réseau de neurones.

## Note importante sur des tests réalisés

Lors de la recherche de l'ensemble des mouvements `vector<std::pair<const Catoms3DMotionRulesLink*, Catoms3DRotation>> motions = Catoms3DMotionEngine::getAllRotationsForModule(module)`

Cette fonction revoie un vecteur de l'ensemble des mouvements du robot Dans l'ordre de la liste des mouvements (cf `/resources/catomsMotions.png`) Par exemple, peu importe les ports connectés du robot actuel, il suivra toujours le mouvement voulu selon la liste des mouvements.