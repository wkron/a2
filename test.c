#include "io.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char** argv) {
  if (argc != 4 && argc != 5) {
    fprintf(stderr, "Usage: %s <points> <queries> <indexes> <k>\n", argv[0]);
    exit(1);
  }

  FILE * points_f = fopen(argv[1], "r");
  assert(points_f != NULL);
  FILE * queries_f = fopen(argv[2], "r");
  assert(queries_f != NULL);
  FILE * indexes_f = fopen(argv[3], "r");
  assert(indexes_f!=NULL);
  int32_t k = atoi(argv[4]);
  int n_points = -1;
  int d;
  double* points = read_points(points_f, &n_points, &d);
  if (points == NULL) {
    fprintf(stderr, "Failed reading data from %s\n", argv[1]);
    exit(1);
  }
  fclose(points_f);

  int n_queries = -1;
  int d_queries;
  double* queries = read_points(queries_f, &n_queries, &d_queries);
  if (queries == NULL) {
    fprintf(stderr, "Failed reading data from %s\n", argv[2]);
    exit(1);
  }
  fclose(queries_f);
  int n_indexes = -1;
  int d_indexes;
  int* indexes = read_indexes(indexes_f,&n_indexes,&d_indexes);

  if (d != d_queries) {
    fprintf(stderr, "Reference points have %d dimensions, but query points have %d dimensions\n",
            (int)d, (int)d_queries);
    exit(1);
  }
 int testint = 0;
 for (int i = 0; i < n_queries; i++) {
        double indexdist = distance(d,&queries[i*d],&points[d*indexes[i*k]]);
        for (int j =0; j<n_points;j++){
            double pointdist = distance(d,&points[j*d],&queries[i*d]);
            
            if (indexdist> pointdist){
                testint = 1;
          }
        }   
  
 }
  if (testint==1){
    printf("Test failed: The first index is not the closest point for every query\n");
  }
  if (testint==0){
    printf("Test suceeded:The first index is the closest point for every query\n");
  }
  free(indexes);
  free(points);
  free(queries);

  return 0;
}
