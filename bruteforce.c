#include "bruteforce.h"
#include "util.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>


// Example from chatGPT
//Comment 2 og 3
int* knn(int k, int d, int n, const double *points, const double* query) {
    // Create an array to store the distances between the query and each point
  double distances[n];

  // Loop over each point and calculate the distance between it and the query
  for (int i = 0; i < n; i++) {
    double sum = 0;
    for (int j = 0; j < d; j++) {
      double diff = query[j] - points[i * d + j];
      sum += diff * diff;
    }
    distances[i] = sqrt(sum);
  }

  // Create an array to store the k nearest points
  int nearest[k];

  // Loop over each point and find the k points with the smallest distances
  for (int i = 0; i < k; i++) {
    // Find the point with the smallest distance
    double min_distance = INFINITY;
    int min_index = -1;
    for (int j = 0; j < n; j++) {
      if (distances[j] < min_distance) {
        min_distance = distances[j];
        min_index = j;
      }
    }

    // Add the point with the smallest distance to the nearest array and set its distance to infinity
    nearest[i] = min_index;
    distances[min_index] = INFINITY;
  }

  // Return the array of k nearest points
  return nearest;
}
