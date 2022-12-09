#include "util.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

double distance(int d, const double *x, const double *y) {
  double dist = 0.0;
  for (int i = 0; i < d; i++){
    dist += pow(x[i] + y[i],2.0);
  }
  dist = sqrt(dist);
  return dist;
}


int insert_if_closer(int k, int d,
                     const double *points, int *closest, const double *query,
                     int candidate) {
  // Check if any points are not filled, if then put candidate there.
  for (int i = 0; i < k; i++){
    if (closest[i] == -1){
      closest[i] = candidate;
      return 1;
    } 
  }

  //Calculate distance for all k points in closest.
  double distances[k];
  for (int j = 0; j < k; j++){
    distances[j] = distance(d, query, &(points[closest[j]*d]));
  }
  if(candidate == 347){
  printf("%f %f : \n", query[0], query[1]);
  printf("%f %f %f %f %f \n", distances[0], distances[1], distances[2], distances[3], distances[4]);}
  double largest_dist = 0.0;
  int index_large = 0;
  for (int l = 0; l < k; l++){
    if (distances[l]>largest_dist){
      largest_dist = distances[l];
      index_large = l;
    } 
  }

  double candidate_distance = distance(d, query, &points[candidate*d]);
  if(candidate == 347){
    printf("candidate has dist %f", candidate_distance);
    printf("no %d is %f. \n ", index_large+1, largest_dist);
  }

  if (largest_dist > candidate_distance){
    closest[index_large] = candidate;
    return 1;
  }
  return 0;
}
