# Explication de l'application C3DReconfDemo

## Etape 1 : Propagation du gradient de couleur

On utilise `myBroadcastFunc` pour envoyer un message avec un paramètre `distance` qui correspond à la distance entre le point courant et le point de départ. `myAcknowledgeFunc` permet de récupérer le message jusqu'au leader (afin de confirmer que le message a bien été reçu par l'ensemble des modules). 

On ne veut pas modifier la couleur des modules qui se trouvent dans la zone `target` (définie dans le fichier `config.xml`). On utilise alors `target->isInTarget(module->position)` pour savoir si oui ou non le module est dans la zone target et donc si on doit modifier sa couleur ou non.

## Etape 2 : Recherche des modules à déplacer