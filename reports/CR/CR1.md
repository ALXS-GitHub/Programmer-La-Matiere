<script src="https://cdn.jsdelivr.net/gh/ALXS-GitHub/Markdown-Themes@latest/Aesthetic/cdnimport.js"></script>
<script defer>
    window.addEventListener("load", function() {
    document.color.setColor("green");
    document.font.setFont("arial");
});
</script>

# Compte rendu 1 

<date>13/03/2024</date>

<author>Munch Alexis, Vaillant Erwan, Yang Michaël</author>

## Algorithmes de recherche

Chaque robot a son propre réseau de neurones. En entrée de ce réseau, les voisins immédiats (éventuellement l'état du robot et les état des robots voisins). En sortie les actions à effectuer (se déplacer, communiquer, ne rien faire, etc.).

Méthode MAP-Elites pour l'entraînement.

### Grille

On va utiliser un algorithme génétique (voir plus de détails en dessous) appliqué aux réseaux de neurones pour remplir la grille. On utilisera un réseau de neurones possédant la même forme au cours des différentes générations (couches internes, entrées et sorties identiques, les coefficients seulement changeront). On réalisera différentes grilles pour plusieurs versions de réseaux de neurones. 

Quelques dimensions possibles pour la grille :

- Hauteur max
- Largeur max
- Surface 
- Évidements (Volume total - nb robots * Volume des robots)
- Volume
- Distance max entre les robots
- (Batterie / distance parcourue)

### Entrées

- Voisins
- Mouvements possibles
- Mouvements possibles des voisins

### Sorties

- Mouvement (Gauche, Droite, Avant, Arrière, Haut Gauche, Haut Droite, Haut Avant, Haut Arrière, Bas Gauche, Bas Droite, Bas Avant, Bas Arrière, Ne rien faire pour les cubes)

? Problème : Comment obtenir des sorties cohérentes, ne générant pas de mouvements incohérents ou impossible (question : sortie unique du réseau à interpréter ou plusieurs sorties ?)

## Algorithme génétique

Population de plusieurs individus à tester lors d'une génération. Chaque individu est un réseau de neurones (liste de coefficients).

### Sélection

Une sélection sur les individus qui génèrent des résultat les plus diversifiés et divergeant des solutions précédentes.

Mais dans le cas de MAP-Elites, la sélection n'est pas nécessaire. On peut simplement ajouter les individus à la grille et les comparer avec les autres individus déjà présents.

### Croisement

On peut réaliser différents types de croisements :

- Croisements multi points (entre plusieurs parents $\rightarrow$ on prend des parties de chaque parent pour créer un nouvel individu)
- Croisements sur les poids (avec des fonctions polynomiales)

### Mutations

Plusieurs types de mutations possibles :

- Permutation de poids
- Changement de poids de matières aléatoire


