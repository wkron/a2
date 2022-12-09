#include "bruteforce.h"
#include "util.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>



int* knn(int k, int d, int n, const double *points, const double* query) {
  // Allocate memory for the array
  int* closest = malloc(k * sizeof(int));

  // Set all elements to 0
  for (int i = 0; i < k; i++){
    closest[i] = -1;
  }

  // Go through all elements, insert if closer
  for (int i = 0; i < n; i ++){ 
    insert_if_closer(k, d, points, closest, query, i);
  }
  return closest;
}
