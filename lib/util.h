#ifndef UTIL_H
#define PAIR_LIMIT 10
#define INIT_CLUSTER_LIM 50
pair* GenerateInitialPixels(int);

typedef struct pair {
    int x;
    int y;
} pair;



bool pairCompare(pair A, pair B) {
    if(A.x==B.x && A.y==B.y) return true;
    return false;
}

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





#endif