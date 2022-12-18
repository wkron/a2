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
  //Check in order if candidate is closer than any point in closest.
  for (int i = 0; i < k; i++){
    double dist = (closest[i] == -1) ? INFINITY : distance(d, query, &points[closest[i]*d]); //if closest is -1 set distance to inf - otherwise calculate it
    if (dist>candidate_distance) { 
      // Insert candidate at first point where closer, after pushing all elements behind one back.
      for (int j = k-1; j > i; j--){
        closest[j] = closest[j-1];
      }
      closest[i] = candidate;
      return 1;
    } 
  }
  return 0;
}

