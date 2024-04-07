# Explications

Ici le dossier s'appelle test1, il sert a tester un robot qui avance en ligne droite. (ps j'ai juste copié collé le C3DReconfDemo donc c'est pour ça que les fichiers s'appellent C3DReconfDemo)

Ce premier test cherche surtout a faire avancer le robot pendant quelques mouvements sur un tapis de robots.

## Note importante sur des tests réalisés

Lors de la recherche de l'ensemble des mouvements `vector<std::pair<const Catoms3DMotionRulesLink*, Catoms3DRotation>> motions = Catoms3DMotionEngine::getAllRotationsForModule(module)`

Cette fonction revoie un vecteur de l'ensemble des mouvements du robot Dans l'ordre de la liste des mouvements (cf `/resources/catomsMotions.png`) Par exemple, peu importe les ports connectés du robot actuel, il suivra toujours le mouvement voulu selon la liste des mouvements.