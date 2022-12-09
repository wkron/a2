#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

double* read_points(FILE *f, int* n_out, int *d_out) {
  double* arr = malloc((*n_out +1) * (*d_out +1) * sizeof(double));
  assert(fread(n_out, sizeof(int), 1, f)==1);
  assert(fread(d_out, sizeof(int), 1, f)==1);
  assert(fread(arr, sizeof(double), (*n_out)* (*d_out), f)>0);
  return arr;
}

int* read_indexes(FILE *f, int *n_out, int *k_out) {
  int* arr = malloc((*n_out +1)* (*k_out +1) * sizeof(int));
  assert(fread(n_out, sizeof(int), 1, f)==1);
  assert(fread(k_out, sizeof(int), 1, f)==1);
  assert(fread(arr, sizeof(int), (*n_out)* (*k_out), f)>0);
  return arr;
}

// Both of these should return 1 on error and 0 on success
int write_points(FILE *f, int32_t n, int32_t d, double *data) {
  assert(fwrite(&n, sizeof(int), 1, f)==1);
  assert(fwrite(&d, sizeof(int), 1, f)==1);
  assert(fwrite(data, sizeof(double), n*d, f)>0);
  return 0;
}

int write_indexes(FILE *f, int32_t n, int32_t k, int *data) {
  assert(fwrite(&n, sizeof(int), 1, f)==1);
  assert(fwrite(&k, sizeof(int), 1, f)==1);
  assert(fwrite(data, sizeof(int), n*k, f)>0);
  return 0;
}
