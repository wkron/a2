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
  int largest_dist_index = 0; //Index(in closest) for largest distance
  double largest_dist = 0.0; //Largest distance in closest
  //Check in order if closer than any in closest, if then put there.
  for (int l = 0; l < k; l++){
    double dist = 0.0;
    if (closest[l] == -1) {
       dist = INFINITY;
    } else {
      dist = distance(d, query, &points[closest[l]*d]);
    }
    if (dist>candidate_distance) { 
      largest_dist_index=0; //Slet herfra ned til closest hvis du vil implementere en sorteret løsning
      largest_dist= 0.0;
      for (int i = 0; i<k; i++){
        if (distance(d,query , &points[closest[i]*d])>largest_dist){ //Calculating the largest distance
          largest_dist = distance(d,query , &points[closest[i]*d]);
          largest_dist_index = i;
        }
      }
      closest[largest_dist_index] = candidate; 
      return 1;
      break;
    } 
  }
  return 0;
}



// det her er bare noget rod / mislykket kode

  // Check if any points are not filled, if then put candidate there.
  // for (int i = 0; i < k; i++){
  //   if (closest[i] == -1){
  //     closest[i] = candidate;
  //     return 1;
  //   } 
  // }

  //Calculate distance for all k points in closest.
  // double distances[k];
  // for (int j = 0; j < k; j++){
  //   distances[j] = distance(d, query, &(points[closest[j]*d]));
  // }

        // printf("%f is smaller than %f. Swapping in index %i! \n", candidate_distance, distances[l], candidate);
      // printf("The points are %f, %f and %f, %f. \n", points[candidate*d],  points[candidate*d+1],points[closest[l]*d], points[closest[l]*d+1]);
