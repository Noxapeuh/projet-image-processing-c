#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>

t_bmp8 * bmp8_loadImage(const char * filename){
    FILE * file = fopen(filename, "rb");
    t_bmp8 *img;
    fread(img->header, 1, 54, file);
    img->width = *(unsigned int *)(img->header + 18);
    img->height = *(unsigned int *)(img->header + 22);
    img->colorDepth = *(unsigned int *)(img->header + 28);
    img->data = *(unsigned int *)(img->header + 34);
    fclose(file);
    return img;
}

void bmp8_printInfo(t_bmp8 * img) {
    printf("Image Info :\n");
    printf("\tWidth : %d\n", img->width);
    printf("\tHeight : %d\n", img->height);
    printf("\tColor Depth : %d\n", img->colorDepth);
    printf("\tSize : %d\n", img->height*img->width);
}


int main(){
    t_bmp8 * bmp8;
    bmp8 = bmp8_loadImage( "lena_color.bmp" );
    bmp8_printInfo( bmp8 );


  return 0;
}