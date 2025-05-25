#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Partie 1

// Fonction permettant de charger une image BMP depuis un fichier filename, renvoyant l'image chargée

t_bmp8 * bmp8_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier '%s'\n", filename);
        return NULL;
    }

    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8));

    fread(img->header, 1, 54, file);

    img->width = *(unsigned int *)(img->header + 18);
    img->height = *(unsigned int *)(img->header + 22);
    img->colorDepth = *(unsigned short *)(img->header + 28);
    img->dataSize = *(unsigned int *)(img->header + 34);
    fread(img->colorTable, sizeof(unsigned char), 1024, file);

    img->data = (unsigned char *)malloc(img->dataSize * sizeof(unsigned char));
    fread(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
    return img;
}

// Fonction permettant de sauvegarder une image BMP dans un fichier filename
void bmp8_saveImage(const char * filename, t_bmp8 * img) {
    FILE * file = fopen(filename,"wb");

    if ( file ==  NULL ) {
        printf("Erreur : Impossible d'ouvrir le fichier\n");
        return;
    }

    printf("Fichier ouvert avec succes\n");

    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
}

// Fonction permettant de libérer la mémoire allouée à une image BMP
void bmp8_free(t_bmp8 * img) {
    free(img);
}

// Fonction permettant d'afficher des informations sur une image BMP
void bmp8_printInfo(t_bmp8 * img) {
    printf("Image Info :\n");
    printf("\tWidth : %d\n", img->width);
    printf("\tHeight : %d\n", img->height);
    printf("\tColor Depth : %d\n", img->colorDepth);
    printf("\tSize : %d\n", img->height*img->width);
}

// Fonction permettant d'appliquer un efet negatif sur une image BMP passée en paramètre
void bmp8_negative(t_bmp8 * img) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            img->data[i*img->width + j] = 255 - img->data[i*img->width + j];
        }
    }
}

// Fonction permettant de changer la luminosité sur une image BMP passée en paramètre
void bmp8_brightness(t_bmp8 * img, int value){
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            if (img->data[i*img->width + j] + value > 255) {
                img->data[i*img->width + j] = 255;
            } else if (img->data[i*img->width + j] + value < 0){
                img->data[i*img->width + j] = 0;
            } else {
                img->data[i*img->width + j] = img->data[i*img->width + j] + value;
            }
        }
    }
}

// Fonction permettant d'appliquer un efet threshold sur une image BMP passée en paramètre
void bmp8_threshold(t_bmp8 * img, int threshold) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            if (img->data[i*img->width + j] >= threshold) {
                img->data[i*img->width + j] = 255;
            } else {
                img->data[i*img->width + j] = 0;
            }
        }
    }
}

// Fonction permettant d'appliquer n'importe quel filtre à partir de son noyau sur une image BMP passée en paramètre
void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize) {
    int n = kernelSize/2;

    unsigned char *temp = malloc(img->width * img->height *sizeof(unsigned char));
    for (int i = 1; i < img->height-1; i++) {
        for (int j = 1; j < img->width-1; j++) {
            float somme = 0.0;
            for (int ki = -n; ki <= n; ki++) {
                for (int kj = -n; kj <= n; kj++) {
                    int pixel = img->data[(i + ki) * img->width + (j + kj)];
                    float coeff = kernel[ki + n][kj + n];
                    somme += pixel * coeff;
                }
            }
            temp[i * img->width + j] = (unsigned char)somme;
        }
    }

    // Copier le résultat dans l'image
    for (int i = 1; i < img->height - 1; i++) {
        for (int j = 1; j < img->width - 1; j++) {
            img->data[i * img->width + j] = temp[i * img->width + j];
        }
    }

    free(temp);
}

//Partie 2

t_pixel ** bmp24_allocateDataPixels (int width, int height) {
    t_pixel ** pixels;
    pixels = (t_pixel**)malloc(width * height * sizeof(t_pixel *));
    if (pixels == NULL) {
        printf("Erreur : Allocation de mémoire impossible");
        return NULL;
    }
}

t_bmp24 * bmp24_loadImage (const char * filename){

}
void bmp24_saveImage (t_bmp * img, const char * filename);



void bmp24_freeDataPixels (t_pixel ** pixels, int height){
  if (pixels == NULL) return;
    for (int i = 0; i < height; ++i) {
        free(pixels[i]);
    }
}

t_bmp24 * bmp24_allocate (int width, int height, int colorDepth){

}

void bmp24_free (t_bmp24 * img){
  if (img == NULL) return NULL;
    if (img->pixels != NULL){
        for (int i=0; i<img->height;++i) {
            free(img->pixels[i]);
        }
        free(img->pixels);
    }
}