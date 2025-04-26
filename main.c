#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "bmp8.h"

int main() {
    t_bmp8 * bmp8;
    bmp8 = bmp8_loadImage( "images/lena_gray.bmp" );
    bmp8_printInfo( bmp8 );
    bmp8_saveImage("./images/test2.bmp", bmp8);
    bmp8_negative(bmp8);
    bmp8_saveImage("./images/test3.bmp", bmp8);
    bmp8 = bmp8_loadImage( "images/test2.bmp" );
    // Exemple pour initialiser ton kernel en float**

    float **kernel;
    int kernelSize = 3; // car ton noyau est 3x3

    // Allocation dynamique
    kernel = malloc(kernelSize * sizeof(float *));
    for (int i = 0; i < kernelSize; i++) {
        kernel[i] = malloc(kernelSize * sizeof(float));
    }

    // Maintenant on remplit le noyau
    kernel[0][0] = -1;  kernel[0][1] = -1; kernel[0][2] = -1;
    kernel[1][0] = -1; kernel[1][1] = 8;  kernel[1][2] = -1;
    kernel[2][0] = -1;  kernel[2][1] = -1; kernel[2][2] = -1;

    bmp8_applyFilter(bmp8,kernel,3);
    bmp8_saveImage("./images/test3.bmp", bmp8);
    return 0;
}
