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

void bmp24_freeDataPixels (t_pixel ** pixels, int height){
  if (pixels == NULL) return;
    for (int i = 0; i < height; ++i) {
        free(pixels[i]);
    }
}

t_bmp24 * bmp24_allocate (int width, int height, int colorDepth){
  t_bmp24 *img = malloc(sizeof(t_bmp24));
    if (!img) return NULL;
    img->data = bmp24_allocateDataPixels(width, height);
    if (!img->data) {
        free(img);
        printf("Erreur d'allocation mémoire");
        return NULL;
    }
    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    return img;
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

t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) return NULL;

    t_bmp24 *img = malloc(sizeof(t_bmp24));
    if (!img) {
        fclose(file);
        return NULL;
    }

    file_rawRead(BITMAP_HEADER, &img->header, sizeof(t_bmp_header), 1, file);
    file_rawRead(BITMAP_HEADER_INFO, &img->header_info, sizeof(t_bmp_info), 1, file);

    img->width = img->header_info.width;
    img->height = img->header_info.height;
    img->colorDepth = img->header_info.bits;

    t_pixel **data = bmp24_allocateDataPixels(img->width, img->height);
    if (!data) {
        free(img);
        fclose(file);
        return NULL;
    }

    img->data = data;
    bmp24_readPixelData(img, file);
    fclose(file);
    return img;
}

void bmp24_saveImage(t_bmp *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) return;
    file_rawWrite(BITMAP_HEADER, &img->header, sizeof(t_bmp_header), 1, file);
    file_rawWrite(BITMAP_HEADER_INFO, &img->header_info, sizeof(t_bmp_info), 1, file);
    bmp24_writePixelData(img, file);
    fclose(file);
}

void bmp24_negative(t_bmp *img) {
    for (int y = 0; y < img->height; y++)
        for (int x = 0; x < img->width; x++) {
            img->data[y][x].r = 255 - img->data[y][x].r;
            img->data[y][x].g = 255 - img->data[y][x].g;
            img->data[y][x].b = 255 - img->data[y][x].b;
        }
}


void bmp24_brightness(t_bmp *img, int value) {
    for (int y = 0; y < img->height; y++)
        for (int x = 0; x < img->width; x++) {
            int r = img->data[y][x].r + value;
            int g = img->data[y][x].g + value;
            int b = img->data[y][x].b + value;
            img->data[y][x].r = r > 255 ? 255 : r < 0 ? 0 : r;
            img->data[y][x].g = g > 255 ? 255 : g < 0 ? 0 : g;
            img->data[y][x].b = b > 255 ? 255 : b < 0 ? 0 : b;
        }
}

t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
    int n = kernelSize / 2;
    float r = 0, g = 0, b = 0;
    for (int i = -n; i <= n; i++) {
        for (int j = -n; j <= n; j++) {
            int xi = x + i;
            int yj = y + j;
            if (xi >= 0 && xi < img->width && yj >= 0 && yj < img->height) {
                r += img->data[yj][xi].r * kernel[i + n][j + n];
                g += img->data[yj][xi].g * kernel[i + n][j + n];
                b += img->data[yj][xi].b * kernel[i + n][j + n];
            }
        }
    }
    t_pixel p;
    p.r = r > 255 ? 255 : r < 0 ? 0 : r;
    p.g = g > 255 ? 255 : g < 0 ? 0 : g;
    p.b = b > 255 ? 255 : b < 0 ? 0 : b;
    return p;
}

void bmp24_boxBlur(t_bmp24 *img, float **kernel, int kernelSize) {
    for (int y = 1; y < img->height - 1; y++)
        for (int x = 1; x < img->width - 1; x++)
            img->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
}

void bmp24_gaussianBlur(t_bmp24 *img, float **kernel, int kernelSize) {
    for (int y = 1; y < img->height - 1; y++)
        for (int x = 1; x < img->width - 1; x++)
            img->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
}

void bmp24_outline(t_bmp24 *img, float **kernel, int kernelSize) {
    for (int y = 1; y < img->height - 1; y++)
        for (int x = 1; x < img->width - 1; x++)
            img->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
}

void bmp24_emboss(t_bmp24 *img, float **kernel, int kernelSize) {
    for (int y = 1; y < img->height - 1; y++)
        for (int x = 1; x < img->width - 1; x++)
            img->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
}

void bmp24_sharpen(t_bmp24 *img, float **kernel, int kernelSize) {
    for (int y = 1; y < img->height - 1; y++)
        for (int x = 1; x < img->width - 1; x++)
            img->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
}