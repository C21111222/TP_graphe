# TP_graphe

## Description

Ce TP2 consiste à créer aléatoirement un graphe non orienté à partir d'une grille de taille n x n, chaque point étant un sommet du graphe. Deux sommets adjacents (qui se touchent) peuvent (aléatoirement) être reliés par une arête. Le graphe est ensuite colorié selon les différentes composantes connexes. Le nombre de couleurs utilisées est affiché. Le graphe est ensuite affiché dans une fenêtre graphique grace à la bibliothèque graphique graphviz.

## Compilation

Pour compiler le programme, il suffit de taper la commande suivante dans un terminal :

```bash
make
```

## Utilisation

Pour lancer le programme, il suffit de taper la commande suivante dans un terminal :

```bash
make run ARG1=[taille de la grille] ARG2=[probabilité d'ajout d'une arête]
```

Par exemple :

```bash
make run ARG1=5 ARG2=0.5
```

![Exemple de graphe](images/exemple.png)

