# ROUSSELLE Cyril et OUMEZZAOUCHE Rayan

___

# Projet de Traitement d’Images BMP en C

## ⚙️ Description du projet

Ce projet a pour objectif de manipuler des images au format BMP en C sans bibliothèques externes. Il s’articule en trois parties principales :
1. **Traitement d’images en niveaux de gris (8 bits)**
2. **Traitement d’images en couleur (24 bits)**
3. **Égalisation d’histogramme**

### Objectif
L’utilisateur interagit avec le programme via une **interface en ligne de commande (menu)** permettant de charger, modifier et enregistrer des images.

___

## 🏗️ Fonctionnalités implémentées

### Partie 1 – Images 8 bits en niveau de gris
- Lecture et écriture d’images BMP 8 (`bmp8_loadImage`, `bmp8_saveImage`)
- Inversion des couleurs (`bmp8_negative`)
- Modification de la luminosité (`bmp8_brightness`)
- Binarisation (`bmp8_threshold`)
- Libération de mémoire (`bmp8_free`)
- Affichage d'infos (`bmp8_printInfo`)
- Application de filtres par convolution (`bmp8_applyFilter`) :
    - Flou moyen et gaussien
    - Contours
    - Relief
    - Netteté

### Partie 2 – Images 24 bits en couleur
- Lecture et écriture d’images BMP 24 bits (`bmp24_loadImage`, `bmp24_saveImage`)
- Inversion des couleurs (`bmp24_negative`)
- Conversion en niveaux de gris (`bmp24_grayscale`)
- Modification de la luminosité (`bmp24_brightness`)
- Application de filtres par convolution (`bmp24_convolution`, `bmp24_boxBlur`, etc.)

### Partie 3 – Égalisation d’histogramme
- Calcul de l’histogramme d'une image 8 bits (`bmp8_computeHistogram`)
- Construction et normalisation de la CDF (`bmp8_computeCDF`)
- Égalisation de l’image (`bmp8_equalize`)
- Égalisation couleur via conversion RGB → YUV (optionnel)

___

## ⚙️ Instructions d’exécution

Suivre le menu :

```
1. Ouvrir une image
2. Sauvegarder une image
3. Appliquer un filtre
4. Afficher les informations de l'image
5. Quitter
```

___

## Auteurs et rôles

| Nom                | Rôle principal             |
|-|-
| ROUSELLE Cyril     | Chargé du codage & correctifs |
| OUMEZZAOUCHE Rayan | chargé du codage & optimisation |


___

## Évaluation attendue

Ce dépôt respecte les critères du dépot final :
- Programme fonctionnel et bien structuré
- README présent et esthétique
- Utilisation du dépôt Git collaborative

## Feed-back du projet

- Le projet nous a permis de comprendre le fonctionnement des fichier bmp8 et bmp24
- Nous avons appris comment exploiter de l'IA (pour trouver le problème dans notre code ou bien pour nous donner des méthodes qui sont plus efficaces et simplifié)