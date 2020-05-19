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

// Makes clusters using KMeans - One iteration only
void make_clusters(int img_height, int img_width, int pts) {
    int i, j, k, temp_diff, difference;
    pair temp_centroid;
    
    // Prepare clusters by placing centroids...
    for(i=0; i<pts; i++) {
        KMeansCluster[i].__centroid = idx[i];
        // KMeansCluster[i].next = NULL;
    }


    for(i=0; i<img_height; i++) {
        for(j=0; j<img_width; j++) {

            difference=INT_MAX;
            // Find closest centroid for every pixel using difference in gray levels
            for(k=0; k<pts; k++) {
                temp_diff = abs(image_arr[i][j] - getGrayLevel(idx[k]));
                if(temp_diff  < difference) {
                    difference = temp_diff;
                    temp_centroid = idx[k];
                }
            }
            // printf("\n%d  :  (%d, %d)", difference, temp_centroid.x, temp_centroid.y);

            for(k=0; k<pts; k++) {
                if(pairCompare(KMeansCluster[k].__centroid, temp_centroid)) {
                    // Add the point to cluster, after making it a pair
                    pair point;
                    point.x = i, point.y = j;
                    KMeansCluster[k].add_ptr = add;     // Implemented function pointer
                    KMeansCluster[k].add_ptr(&KMeansCluster[k], point);
                }
            }

            // Modifying global image array with gray level from centroid pixel
            image_arr[i][j] = getGrayLevel(temp_centroid);
        }
    }
}


// Spits data in cluster for a particular centroid
void printClusterStats(int pts) {
    int i,j,gray;
    for(i=0; i<pts; i++) {
        printf("\n{%d, %d}\n", KMeansCluster[i].__centroid.x, KMeansCluster[i].__centroid.y);
        for(j=0; j< KMeansCluster[i].freePosCounter-1; j++) {
            gray = getGrayLevel(KMeansCluster[i].points[j]);
            printf("%d  ", gray);
        }
    }
}



// Prints color map from image file [not from program's global array]
void printColorMap(struct color *image, int width, int height) {
    long long int image_size = sizeof(*image)*width*height;
    int i=0;
    while(i<image_size) {
        printf("%d ", (image+i)->r);
        i++;
    }
    printf("\n\nTotal count of pixels: %d\n", i);
}





int processing(struct color *image, int width, int height, struct bmpheader h0, struct dibheader h1, int pts) {
    int i, j, divisions, total_pixels, iterations;
    FILE *fp;
    char filename[100];
    
    int *pixel_arr = (int *) malloc(width*height*sizeof(int));
    total_pixels = width*height;
    divisions = MAX_INTENSITY/pts;
    iterations = 1;

    // Write image data into matrix
    for(i=0; i<height; i++) {
        for(j=0; j<width; j++) {
            image_arr[i][j] = ((image+i*width+j)->r*0.2126) + ((image+i*width+j)->g*0.2126) + ((image+i*width+j)->b*0.2126);
        }
    }

    // Generate random points (x,y)
    GenerateInitialCentroids(pts);

    make_clusters(height, width, pts);

    // printClusterStats(pts);

    /* Prepare image file code starts now */

    // Write modified image array to file...
    printf("\nEnter name of histogram equalized image file: ");
    scanf("%s", filename);
    if((fp=fopen(filename, "wb")) == NULL) {
        printf("\nError, creating BMP file\n");
        return -1;
    }

    fwrite(&h0.id1, 1, sizeof(h0.id1), fp);
    fwrite(&h0.id2, 1, sizeof(h0.id2), fp);
    fwrite(&h0.size, 1, sizeof(h0.size), fp);
    fwrite(&h0.app_spec_1, 1, sizeof(h0.app_spec_1), fp);
    fwrite(&h0.app_spec_2, 1, sizeof(h0.app_spec_2), fp);
    fwrite(&h0.offset, 1, sizeof(h0.offset), fp);
    
    fwrite(&h1.size, 1, sizeof(h1.size), fp);
    fwrite(&h1.width, 1, sizeof(h1.width), fp);
    fwrite(&h1.height, 1, sizeof(h1.height), fp);
    fwrite(&h1.color_planes, 1, sizeof(h1.color_planes), fp);
    fwrite(&h1.bits_per_pixel, 1, sizeof(h1.bits_per_pixel), fp);
    fwrite(&h1.compression, 1, sizeof(h1.compression), fp);
    fwrite(&h1.size_with_padding, 1, sizeof(h1.size_with_padding), fp);
    fwrite(&h1.resolution_horizontal, 1, sizeof(h1.resolution_horizontal), fp);
    fwrite(&h1.resolution_vertical, 1, sizeof(h1.resolution_vertical), fp);
    fwrite(&h1.color_palette, 1, sizeof(h1.color_palette), fp);
    fwrite(&h1.important_colors, 1, sizeof(h1.important_colors), fp);

    printf("Debug: File pointer is at %d bytes\n", ftell(fp));


    // Copy modified pixels...
    fseek(fp,54,SEEK_SET);

    for(i=0; i<height; i++)
        for(j=0; j<width; j++)
            fwrite((image+i*width+j),1,sizeof(struct color),fp);


    free(pixel_arr);
    return 0;
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
        // printColorMap(image, header1.width, header1.height);
    #endif
    
    printf("Enter cluster points for KMeans : ");
    scanf("%d", &points);
    
    status = processing(image, header1.width, header1.height, header0, header1, points);
    if(status == -1) printf("\nFailed to successfully convert image\n");
    else printf("\nSuccessfully equalized image\n");

    return 0;
}