/*
Copyright (C) 2016-2020 Digital Image Processing Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "lib/util.h"
#include "lib/core.h"

cluster KMeans(int image[][MAX_IMAGE_WIDTH], int n) {
    pair prev_mean[MAX_IMAGE_HEIGHT], curr_mean[MAX_IMAGE_HEIGHT];
    int i, a, b;
    
    // initial mean (random)
    for(i=0; i<n; i++) {
        curr_mean[i].x = rand(0,MAX_IMAGE_HEIGHT);
        curr_mean[i].y = rand(0,MAX_IMAGE_WIDTH);
    }

    // loop until  current mean == previous mean
    while(prev_mean!=curr_mean) {
        
    }  


}








int processing(struct color *image, int width, int height, struct bmpheader h0, struct dibheader h1, int pts) {
    int *pixel_arr = (int *) malloc(width*height*sizeof(int));
    int i;
    int total_pixels = width*height;
    FILE *fp;
    char filename[100];
    int image_arr[MAX_IMAGE_HEIGHT][MAX_IMAGE_WIDTH];

    #ifdef DEBUG
        // printf("Size of pixel array: %d\n\n", sizeof(*pixel_arr));
    #endif
    
    // for(i=0;i<height*width; i++) {
    //     *(pixel_arr+i) = (image[i].r*0.2126) + (image[i].g*0.7152) + (image[i].b*0.0722);    
    // }

    // Write image data into matrix
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            image_arr[i][j] = ((image+i*width+j)->r*0.2126) + ((image+i*width+j)->g*0.2126) + ((image+i*width+j)->b*0.2126);
        }
    }

    // Generate random KMeans points (x,y)
    pair initial_points[] = GenerateInitialPixels(pts);

















    /* Prepare image file code starts now */

    // // Write equalized image to file...
    // printf("\nEnter name of histogram equalized image file: ");
    // scanf("%s", filename);
    // if((fp=fopen(filename, "wb")) == NULL) {
    //     printf("\nError, creating BMP file\n");
    //     return -1;
    // }

    // fwrite(&h0.id1, 1, sizeof(h0.id1), fp);
    // fwrite(&h0.id2, 1, sizeof(h0.id2), fp);
    // fwrite(&h0.size, 1, sizeof(h0.size), fp);
    // fwrite(&h0.app_spec_1, 1, sizeof(h0.app_spec_1), fp);
    // fwrite(&h0.app_spec_2, 1, sizeof(h0.app_spec_2), fp);
    // fwrite(&h0.offset, 1, sizeof(h0.offset), fp);
    
    // fwrite(&h1.size, 1, sizeof(h1.size), fp);
    // fwrite(&h1.width, 1, sizeof(h1.width), fp);
    // fwrite(&h1.height, 1, sizeof(h1.height), fp);
    // fwrite(&h1.color_planes, 1, sizeof(h1.color_planes), fp);
    // fwrite(&h1.bits_per_pixel, 1, sizeof(h1.bits_per_pixel), fp);
    // fwrite(&h1.compression, 1, sizeof(h1.compression), fp);
    // fwrite(&h1.size_with_padding, 1, sizeof(h1.size_with_padding), fp);
    // fwrite(&h1.resolution_horizontal, 1, sizeof(h1.resolution_horizontal), fp);
    // fwrite(&h1.resolution_vertical, 1, sizeof(h1.resolution_vertical), fp);
    // fwrite(&h1.color_palette, 1, sizeof(h1.color_palette), fp);
    // fwrite(&h1.important_colors, 1, sizeof(h1.important_colors), fp);

    // printf("Debug: File pointer is at %d bytes\n", ftell(fp));


    // // Copy modified pixels...
    // fseek(fp,54,SEEK_SET);

    // for(i=0; i<height; i++)
    //     for(j=0; j<width; j++)
    //         fwrite((image+i*width+j),1,sizeof(struct color),fp);


    free(pixel_arr);
    return 0;
}


void printColor(struct color *image, int width, int height) {
    long long int image_size = sizeof(*image)*width*height;
    int i=0;
    while(i<image_size) {
        printf("%d ", (image+i)->r);
        i++;
    }
    printf("\n\nTotal count of pixels: %d\n", i);
}



int main() {
    char filename[100];                 // Holds input BMP Image filename
    FILE *fp;
    int i,j;                            // Loop variables
    int status, points;
    printf("*****************************************************************\n");
    printf("\tKMeans Clustering on BMP Image (24-Bit non alpha)\n");
    printf("*****************************************************************\n\n");
    printf("\nEnter a BMP image filename: ");
    scanf("%s", filename);

    if((fp = fopen(filename, "rb"))==NULL) {    // Open file in read as byte mode
        fclose(fp);
        printf("\nError, cannot open file.\nTerminating...\n");
        return 0;
    }


    struct bmpheader header0;
    struct dibheader header1;
    struct color *image;

    fread(&header0.id1, sizeof(header0.id1), 1, fp);
    fread(&header0.id2, sizeof(header0.id2), 1, fp);
    fread(&header0.size, sizeof(header0.size), 1, fp);
    fread(&header0.app_spec_1, sizeof(header0.app_spec_1), 1, fp);
    fread(&header0.app_spec_2, sizeof(header0.app_spec_2), 1, fp);
    fread(&header0.offset, sizeof(header0.offset), 1, fp);
    
    fread(&header1.size, sizeof(header1.size), 1, fp);
    fread(&header1.width, sizeof(header1.width), 1, fp);
    fread(&header1.height, sizeof(header1.height), 1, fp);
    fread(&header1.color_planes, sizeof(header1.color_planes), 1, fp);
    fread(&header1.bits_per_pixel, sizeof(header1.bits_per_pixel), 1, fp);
    fread(&header1.compression, sizeof(header1.compression), 1, fp);
    fread(&header1.size_with_padding, sizeof(header1.size_with_padding), 1, fp);
    fread(&header1.resolution_horizontal, sizeof(header1.resolution_horizontal), 1, fp);
    fread(&header1.resolution_vertical, sizeof(header1.resolution_vertical), 1, fp);
    fread(&header1.color_palette, sizeof(header1.color_palette), 1, fp);
    fread(&header1.important_colors, sizeof(header1.important_colors), 1, fp);

    

    printf("\n\nBMP Header Statistics:");
    printf("\nID feild = %c%c\nSize of BMP file = %lu\nApplication specific = %d\nApplication specific = %d\nOffset where the pixel array (bitmap data) can be found = %lu\n", header0.id1, header0.id2, header0.size, header0.app_spec_1, header0.app_spec_2, header0.offset);

    printf("\n\nDIB Header Statistics:");
    printf("\nNumber of bytes in the DIB header = %lu\nWidth of the bitmap in pixels = %d\nHeight of the bitmap in pixels = %d\nNumber of color planes being used = %d\nNumber of bits per pixel = %d\nNumber of color planes being used = %d\nNumber of bits per pixel = %d\nBI_RGB, pixel array compression used = %lu\nSize of the raw bitmap data (including padding) = %lu\nResolution of the image (horizontal) = %lu\nResolution of the image (vertical) = %lu\nNumber of colors in the palette = %lu\nNumber of important colors; 0 means all colors are important = %lu\n", header1.size, header1.width, header1.height, header1.color_planes, header1.bits_per_pixel, header1.size_with_padding, header1.resolution_horizontal, header1.resolution_vertical, header1.color_palette, header1.important_colors);

    printf("Debug: File pointer is at %d bytes\n", ftell(fp));


    image=(struct color *)malloc(header1.width*header1.height*sizeof(struct color));    // Allocate memory for pixel array
    fseek(fp,54,SEEK_SET);      // Start of pixel array (bitmap data) -- Optional statement; already at 54th bytes

    // Populating image data...
    for(i=0;i<header1.height;i++) {
        for(j=0;j<header1.width;j++)
            fread((image+i*header1.width+j),sizeof(struct color),1,fp);
    }
    fclose(fp);                  // Close file pointer
    
    #ifdef DEBUG
        // printColor(image, header1.width, header1.height);
    #endif
    
    printf("Enter cluster points for KMeans : ");
    scanf("%d", &points);
    
    status = processing(image, header1.width, header1.height, header0, header1, points);
    if(status == -1) printf("\nFailed to successfully convert image\n");
    else printf("\nSuccessfully equalized image\n");

    return 0;
}