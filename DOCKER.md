# Docker Setup

Tuto youtube pour setup docker dans vscode : https://youtu.be/dihfA7Ol6Mw?si=SUZeHgweVA33_Bsu

Si docker n'est pas intallé, vscode propose de l'installer. Sinon, il faut l'installer manuellement. (sous windows, c'est miexu de l'installer dans WSL2)

Vu que le docker est déjà configuré dans le projet, ça va directement lancer le container.

Important: Il faut setup xserver pour que le container puisse afficher les fenêtres. `xhost +local:`

Pour tester si tout fonctionne bien : 

(la première fois pour build VisibleSim : `cd ./VisibleSim && make -j 8`)

```bash
cd ./VisibleSim/applicationsBin/e1_flooding
./e1_flooding
```