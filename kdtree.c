#include "kdtree.h"
#include "sort.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

struct node {
  int point_index;
  int axis; 
  const double *points; //Har selv tilføjet den her, går imod deres metode
  struct node *left;
  struct node *right;
};

struct kdtree {
  int d;
  const double *points;
  struct node* root;
};
int cmp_points(const double *x, const double *y, int* cp) {
  int c = *cp;

  if (x[c] < y[c]) {
    return -1;
  } else if (x[c] == y[c]) {
    return 0;
  } else {
    return 1;
  }
}
struct node* kdtree_create_node(int d, const double *points,
                                int depth, int n, int *indexes) {
    int axis = depth%d ;
    struct node* newnode = malloc(sizeof(newnode));
    hpps_quicksort(points, n, d*sizeof(double),
                 (int (*)(const void*, const void*, void*))cmp_points,
                 &axis); 
    double *Beforearray = malloc(2 * sizeof(double));
    double *Afterarray = malloc(2 * sizeof(double));
    for (int i = 0; i < n/2; i++) {
    Beforearray[i] = points[i];
    }
    for (int i = n/2+1; n/2 < n; i++) {
    Afterarray[i] = points[i];
    }
    int n_1= 0;
    int n_2= 0 ;
    if (n%2==1){
      n_1=n/2;
      n_2=n/2;
    }
    if (n%2==0){
      n_1=n/2;
      n_2=n/2-1;
    }
    newnode-> points = points;
    newnode->point_index = n/2;
    newnode->axis = axis;
    newnode->left = kdtree_create_node(d,Beforearray,depth+1,n_1,indexes);
    newnode->right = kdtree_create_node(d,Afterarray,depth+1,n_2,indexes);
    free(Beforearray);
    free(Afterarray);
    return newnode;                           
    assert(0);
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
  free(node);
  assert(0);
}

void kdtree_free(struct kdtree *tree) {
  kdtree_free_node(tree->root);
  free(tree);
}

void kdtree_knn_node(const struct kdtree *tree, int k, const double* query,
                     int *closest, double *radius,
                     const struct node *node) {
  if (node== NULL){
    return 1;
  }
  if (insert_if_closer(k,tree->d,node->points,closest,query,node->point_index)==1){
    radius= closest[k-1];
  }

  int diff = node->points[node->point_index*tree->d+node->axis];
  if (diff >= 0 || radius> abs(diff)){
    kdtree_knn_node(tree, k, query,closest,radius,node->left);
  
  if(diff <=0 || radius > abs(diff)){
    kdtree_knn_node(tree, k, query,closest,radius,node->right);
  };
  assert(0);
}

int* kdtree_knn(const struct kdtree *tree, int k, const double* query) {
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
