#include "util.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

double distance(int d, const double *x, const double *y) {
  double dist = 0.0;
  for (int i = 0; i < d; i++){
    dist += pow(x[i] - y[i],2.0);
  }
  dist = sqrt(dist);
  return dist;
}

int insert_if_closer(int k, int d, const double *points, int *closest, const double *query, int candidate) {
  double candidate_distance = distance(d, query, &(points[candidate*d]));
  //Check in order if closer than any in closest.
  for (int i = 0; i < k; i++){
    double dist = 0.0;
    if (closest[i] == -1) { 
       dist = INFINITY;
    } else {
      dist = distance(d, query, &points[closest[i]*d]);
    }
    if (dist>candidate_distance) { 
      // Inserts candidate at first point where closer, and pushes all elements behind one back.
      for (int j = k-1; j > i; j--){
        closest[j] = closest[j-1];
      }
      closest[i] = candidate;
      return 1;
    } 
  }
  return 0;
}

