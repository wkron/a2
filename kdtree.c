#include "kdtree.h"
#include "sort.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>

struct node {
  int point_index;
  int axis; 
  struct node *left;
  struct node *right;
};

struct kdtree {
  int d;
  const double *points;
  struct node* root;
};
struct sortdata
{
  int d;
  const double *points;
  int axis;
};

int cmp_points( int *x, int *y, struct sortdata *sortdata) {
  int c = sortdata-> axis;
  int d = sortdata-> d;
  const double *points = sortdata -> points;
  if (points[*x * d+c] < points[*y * d +c]) { 
    return -1;
  } else if (points[*x * d+c] == points[*y * d+c]) {
    return 0;
  } else {
    return 1;
  }
}


struct node* kdtree_create_node(int d, const double *points,
                                int depth, int n, int *indexes) {
    if(n > 1){
      int axis = depth % d;
      struct sortdata sortdata = {.d = d, .points = points, .axis = axis};

      hpps_quicksort((void *) indexes, n, sizeof(int),
                    (int (*)(const void*, const void*, void*))cmp_points,
                    &sortdata);
      int n_before = n/2;
      int n_after = (n-1)/2;
      int *before_array  = &indexes[0];
      int *after_array = &indexes[n/2+1];

      struct node* newnode = malloc(sizeof(struct node));
      newnode->point_index = indexes[n/2];
      newnode->axis = axis;
      newnode->left = kdtree_create_node(d,points,depth+1,n_before, before_array);
      newnode->right = kdtree_create_node(d,points,depth+1,n_after, after_array);

      return newnode;   
    }  
    if(n == 1){
      int axis = depth % d;
      struct node* newnode = malloc(sizeof(struct node));
      newnode->point_index = indexes[0];
      newnode->axis = axis;
      newnode->left = NULL;
      newnode->right = NULL;
      return newnode;    
    }
    else {
      return NULL;
    }  
}

struct kdtree *kdtree_create(int d, int n, const double *points) {
  struct kdtree *tree = malloc(sizeof(struct kdtree));
  tree->d = d;
  tree->points = points;
  int *indexes = malloc(sizeof(int) * n);

  for (int i = 0; i < n; i++) {
    indexes[i] = i;
  }
  tree->root = kdtree_create_node(d, points, 0, n, indexes);
  free(indexes);

  return tree;
}

void kdtree_free_node(struct node *node) {
  if(node){
    kdtree_free_node(node->left);
    kdtree_free_node(node->right);
    free(node);
  }
}

void kdtree_free(struct kdtree *tree) {
  kdtree_free_node(tree->root);
  free(tree);
}

void kdtree_knn_node(const struct kdtree *tree, int k, const double* query,
                     int *closest, double *radius,
                     const struct node *node) {
  int d = tree->d;
  if (node== NULL){
    return;
  }
  if (insert_if_closer(k,d,tree->points,closest,query,node->point_index)){
    *radius= distance(d, query, &(tree->points)[closest[k-1]*d]);
  }

  int diff = tree->points[node->point_index*d+node->axis]-query[node->axis];
  if (diff >= 0 || *radius> abs(diff)){
    kdtree_knn_node(tree, k, query,closest,radius,node->left);
  }
  if(diff <=0 || *radius > abs(diff)){
    kdtree_knn_node(tree, k, query,closest,radius,node->right);
  }
  return;
}

int* kdtree_knn(const struct kdtree *tree, int k, const double* query){
  int* closest = malloc(k * sizeof(int));
  double radius = INFINITY;

  for (int i = 0; i < k; i++) {
    closest[i] = -1;
  }

  kdtree_knn_node(tree, k, query, closest, &radius, tree->root);

  return closest;
}

void kdtree_svg_node(double scale, FILE *f, const struct kdtree *tree,
                     double x1, double y1, double x2, double y2,
                     const struct node *node) {
  if (node == NULL) {
    return;
  }

  double coord = tree->points[node->point_index*2+node->axis];
  if (node->axis == 0) {
    // Split the X axis, so vertical line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            coord*scale, y1*scale, coord*scale, y2*scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, coord, y2,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    coord, y1, x2, y2,
                    node->right);
  } else {
    // Split the Y axis, so horizontal line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            x1*scale, coord*scale, x2*scale, coord*scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, x2, coord,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    x1, coord, x2, y2,
                    node->right);
  }
}

void kdtree_svg(double scale, FILE* f, const struct kdtree *tree) {
  assert(tree->d == 2);
  kdtree_svg_node(scale, f, tree, 0, 0, 1, 1, tree->root);
}

