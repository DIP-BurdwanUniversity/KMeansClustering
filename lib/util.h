#ifndef UTIL_H
#pragma once
#include "core.h"
#define PAIR_LIMIT 10
#define INIT_CLUSTER_LIM 50

typedef struct pair {
    int x;
    int y;
} pair;

pair idx[INIT_CLUSTER_LIM];     // initial random indexes

// Compares two pairs for equality
bool pairCompare(pair A, pair B) {
    if(A.x==B.x && A.y==B.y) return true;
    return false;
}

// Similar to pairCompare(): works with arrays
bool pairArrayCompare(pair A[], pair B[]) {
    int i;
    for(i=0; i<PAIR_LIMIT; i++) {
        if(A[i].x!=B[i].x || A[i].y!=B[i].y) return false; 
    }
    return true;
}

// Generates random initial pixel coordinates [max. clusters 50]
void GenerateInitialPixels(int pts) {
    int i;
    // Some random f(x,y) to generate initial random points...
    for(i=0; i<pts; i++) {
        idx[i].x=pts*i + 2*i + i;
        idx[i].y=pts*i + 2*pts + i;
    }
}

// Wrapper returns gray level for pair
int getGrayLevel(pair p) {
    return image_arr[p.x][p.y];
}



#endif