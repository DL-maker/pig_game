# Jeu de Cochons (Pig Dice Game)

Une implémentation en C du jeu de dés "Pig" (ou Jeu de Cochons), un jeu de prise de risque où les joueurs doivent décider entre accumuler des points ou les sécuriser.

## Règles du Jeu

Dans cette version simplifiée du jeu de cochons:

- À chaque tour, un joueur lance un dé à 6 faces autant de fois qu'il le souhaite.
- Chaque lancer ajoute la valeur du dé au score temporaire du tour.
- Si le joueur obtient un "1", il perd tous les points accumulés pendant ce tour et c'est au tour du joueur suivant.
- Le joueur peut choisir de "banquer" (sécuriser) ses points à tout moment, ajoutant son score temporaire à son score total.
- Le premier joueur à atteindre ou dépasser 100 points (par défaut) gagne la partie.

## Fonctionnalités

- Mode multijoueur pour 2 joueurs humains ou plus
- Mode joueur unique contre différents niveaux d'IA
- Plusieurs niveaux de difficulté pour les bots:
  - **Facile**: Continue à lancer jusqu'à 5 points, puis décision aléatoire
  - **Moyen**: Banque après 10 points ou s'il peut gagner
  - **Difficile**: Stratégie adaptative basée sur l'état du jeu et les scores des adversaires
  - **Aléatoire**: Décisions aléatoires (mais banque toujours s'il peut gagner)
- Interface en ligne de commande intuitive
- Possibilité de modifier le score cible pour gagner
- Création de parties mixtes (humains et bots)

## Compilation et Exécution

### Prérequis

- Compilateur GCC
- Make

### Compilation

```bash
make
```

Le programme compilé sera disponible dans le dossier `bin/`.

### Exécution

Il existe deux façons de lancer le jeu:

1. **Mode interactif**:
   ```bash
   ./bin/cochon
   ```

2. **Mode avec arguments** (où les arguments sont les noms des joueurs humains):
   ```bash
   ./bin/cochon Alice Bob
   ```

## Structure du Projet

```
.
├── src/
│   ├── main.c       # Point d'entrée principal
│   ├── game.c       # Gestion du jeu
│   ├── game.h       # Définitions des structures et fonctions du jeu
│   ├── player.c     # Logique du joueur humain
│   ├── player.h     # Définitions des fonctions du joueur
│   ├── bot.c        # Implémentation des IA
│   ├── bot.h        # Définitions des fonctions d'IA
│   ├── utils.c      # Fonctions utilitaires
│   └── utils.h      # Définitions des fonctions utilitaires
├── obj/             # Fichiers objets générés lors de la compilation
├── bin/             # Exécutable du jeu
├── Makefile         # Instructions de compilation
└── README.md        # Ce fichier
```

## Stratégies des Bots

### Bot Facile (Easy)
- Continue à lancer si le score du tour est inférieur à 5 points
- Décision aléatoire après 5 points

### Bot Moyen (Medium)
- Banque systématiquement après avoir accumulé 10 points dans un tour
- Banque immédiatement s'il peut gagner avec le score actuel

### Bot Difficile (Hard)
- Prend en compte le score des adversaires pour adapter sa stratégie
- Prend plus de risques si un adversaire a une avance significative
- Devient plus prudent s'il est proche de gagner
- Ajuste son comportement si un adversaire est proche de gagner

### Bot Aléatoire (Random)
- Prend des décisions aléatoires, mais banque toujours s'il peut gagner

## Commandes Make

- `make`: Compile le projet
- `make clean`: Supprime les fichiers objets
- `make mrproper`: Supprime les fichiers objets et l'exécutable
- `make rebuild`: Nettoie et recompile le projet
- `make run`: Compile et exécute le jeu

## Auteur

Ce projet a été réalisé dans le cadre d'un travail demandé par Julien Palard.