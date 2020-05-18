#ifndef UTIL_H
#include "core.h"
#define PAIR_LIMIT 10
#define INIT_CLUSTER_LIM 50
pair* GenerateInitialPixels(int);

typedef struct pair {
    int x;
    int y;
} pair;

// Compares two pairs for equality
bool pairCompare(pair A, pair B) {
    if(A.x==B.x && A.y==B.y) return true;
    return false;
}

// Overloads above pairCompare() to work with arrays
bool pairCompare(pair A[], pair B[]) {
    for(int i=0; i<PAIR_LIMIT; i++) {
        if(A[i].x!=B[i].x || A[i].y!=B[i].y) return false; 
    }
    return true;
}

// Generates random initial pixel coordinates [max. clusters 50]
pair* GenerateInitialPixels(int pts) {
    pair idx[INIT_CLUSTER_LIM];
    // Some random f(x,y) to generate initial random points...
    for(int i=0; i<pts; i++) {
        idx[i].x=pts*i + 2*i + i;
        idx[i].y=pts*i + 2*pts + i;
    }
    return idx;
}

// Wrapper returns gray level for pair
int getGrayLevel(pair p) {
    return image_arr[p.x][p.y];
}



#endif