#ifndef UTIL_H
#define PAIR_LIMIT 10


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



#endif