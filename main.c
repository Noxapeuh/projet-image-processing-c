#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"
#include <math.h>

void afficherMenuPrincipal() {
    printf("\nVeuillez choisir une option :\n");
    printf("1. Ouvrir une image\n");
    printf("2. Sauvegarder une image\n");
    printf("3. Appliquer un filtre\n");
    printf("4. Afficher les informations de l'image\n");
    printf("5. Quitter\n>>> Votre choix : ");
}

void afficherMenuFiltres() {
    printf("\nVeuillez choisir un filtre :\n");
    printf("1. Négatif\n");
    printf("2. Luminosité\n");
    printf("3. Binarisation\n");
    printf("4. Flou\n");
    printf("5. Flou gaussien\n");
    printf("6. Netteté\n");
    printf("7. Contours\n");
    printf("8. Relief\n");
    printf("9. Retourner au menu précédent\n>>> Votre choix : ");
}

unsigned short lireProfondeurCouleur(const char* chemin) {
    FILE* f = fopen(chemin, "rb");
    if (!f) return 0;

    fseek(f, 28, SEEK_SET); // Offset 28 = biBitCount
    unsigned short profondeur = 0;
    fread(&profondeur, sizeof(unsigned short), 1, f);
    fclose(f);

    return profondeur;
}

float** createKernel(const int src[3][3], float facteur) {
    float **kernel = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = src[i][j] * facteur;
        }
    }
    return kernel;
}

void freeKernel(float **kernel) {
    for (int i = 0; i < 3; i++) {
        free(kernel[i]);
    }
    free(kernel);
}


int main() {
    t_bmp8* image = NULL;
    t_bmp24* image24 = NULL;
    int choix, filtre, value;
    char chemin[256];

    do {
        afficherMenuPrincipal();
        scanf("%d", &choix);
        getchar(); // Pour éviter les effets de retour à la ligne

        switch (choix) {
            case 1:
                printf("Chemin du fichier : ");
                fgets(chemin, sizeof(chemin), stdin);
                chemin[strcspn(chemin, "\n")] = '\0';
                unsigned short profondeur = lireProfondeurCouleur(chemin);
                if (profondeur == 8) {
                    image = bmp8_loadImage(chemin);
                    printf(image ? "Image 8 bits chargée avec succès !\n" : "Échec du chargement de l'image.\n");
                } else if (profondeur == 24) {
                    image24 = bmp24_loadImage(chemin);
                    printf(image24 ? "Image 24 bits chargée avec succès !\n" : "Échec du chargement de l'image.\n");
                } else {
                    printf("Format non pris en charge (%d bits).\n", profondeur);
                }
                break;



            case 2:
                if (image) {
                    printf("Chemin du fichier : ");
                    fgets(chemin, sizeof(chemin), stdin);
                    chemin[strcspn(chemin, "\n")] = '\0';
                    bmp8_saveImage(chemin, image);
                    printf("Image sauvegardée avec succès !\n");
                } else {
                    printf("Aucune image chargée.\n");
                }
                break;

            case 3:
                if (image) {
                    do {
                        afficherMenuFiltres();
                        scanf("%d", &filtre);
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF); // vide le buffer


                        switch (filtre) {
                            case 1: // Negatif
                                if (image->colorDepth == 8) {
                                    bmp8_negative(image);
                                } else {
                                    bmp24_negative(image);
                                }

                                printf("Filtre appliqué avec succès !\n");
                                break;
                            case 2: { // Luminosité
                                int value;
                                printf("Entrez une valeur de luminosité (ex: +50 ou -30) : ");
                                scanf("%d", &value);
                                if (image->colorDepth == 8) {
                                    bmp8_brightness(image, value);
                                } else {
                                    bmp24_brightness(image, value);
                                }

                                printf("Luminosité ajustée avec succès !\n");
                                break;
                            }
                            case 3: { // Threshold
                                int seuil;
                                printf("Entrez la valeur du seuil (0 à 255) : ");
                                scanf("%d", &seuil);
                                if (seuil < 0) seuil = 0;
                                if (seuil > 255) seuil = 255;
                                bmp8_threshold(image, seuil);
                                printf("Filtre de seuillage appliqué avec succès !\n");
                                break;
                            }
                            case 4: { // Box Blur
                                int boxBlur[3][3] = {
                                    {1, 1, 1},
                                    {1, 1, 1},
                                    {1, 1, 1}
                                };
                                float **kernel = createKernel(boxBlur, 1.0f / 9.0f);
                                bmp8_applyFilter(image, kernel, 3);
                                freeKernel(kernel);
                                printf("Filtre flou appliqué avec succès !\n");
                                break;
                            }

                            case 5: { // Flou Gaussien
                                int gauss[3][3] = {
                                    {1, 2, 1},
                                    {2, 4, 2},
                                    {1, 2, 1}
                                };
                                float **kernel = createKernel(gauss, 1.0f / 16.0f);
                                bmp8_applyFilter(image, kernel, 3);
                                freeKernel(kernel);
                                printf("Filtre flou gaussien appliqué avec succès !\n");
                                break;
                            }

                            case 6: { // Netteté
                                int sharpen[3][3] = {
                                    {0, -1, 0},
                                    {-1, 5, -1},
                                    {0, -1, 0}
                                };
                                float **kernel = createKernel(sharpen, 1.0f);
                                bmp8_applyFilter(image, kernel, 3);
                                freeKernel(kernel);
                                printf("Filtre netteté appliqué avec succès !\n");
                                break;
                            }

                            case 7: { // Contours
                                int outline[3][3] = {
                                    {-1, -1, -1},
                                    {-1, 8, -1},
                                    {-1, -1, -1}
                                };
                                float **kernel = createKernel(outline, 1.0f);
                                bmp8_applyFilter(image, kernel, 3);
                                freeKernel(kernel);
                                printf("Filtre contours appliqué avec succès !\n");
                                break;
                            }

                            case 8: { // Relief
                                int emboss[3][3] = {
                                    {-2, -1, 0},
                                    {-1, 1, 1},
                                    {0, 1, 2}
                                };
                                float **kernel = createKernel(emboss, 1.0f);
                                bmp8_applyFilter(image, kernel, 3);
                                freeKernel(kernel);
                                printf("Filtre relief appliqué avec succès !\n");
                                break;
                            }
                            default:
                                break;
                        }
                    } while (filtre != 9);
                } else {
                    printf("Aucune image chargée.\n");
                }
                break;

            case 4:
                if (image) {
                    bmp8_printInfo(image);
                } else {
                    printf("Aucune image chargée.\n");
                }
                break;

            case 5:
                printf("Fermeture du programme...\n");
                break;

            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 5);

    if (image)
        bmp8_free(image);

    return 0;
}
