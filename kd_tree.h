#ifndef KD_TREE_H_
#define KD_TREE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include "structs.h"
#include "utils.h"

/**
 * @brief Initializes a k-d tree.
 *
 * @return kd_tree_t* The newly created k-d tree
 */
kd_tree_t *init_kdtree(void);

/**
 * @brief Gets the k from the file (the dimmension of the coordinates).
 *
 * @param file The file already opened, well-formated.
 * @return unsigned int The number of components of a point / k.
 */
unsigned int get_kdim(FILE *file);

/**
 * @brief Gets the number of points that have to be loaded.
 *
 * @param file The file already opened, well-formated.
 * @return size_t The number of points that needs to be loaded.
 */
size_t get_points_number(FILE *file);

/**
 * @brief Initializes a k-d tree node.
 *
 * @param point An array containing the coordinates of the point.
 * @param kdim The k dimmension of a point.
 * @return kd_node_t* The newly created node.
 */
kd_node_t *init_kdnode(int *point, unsigned int kdim);

/**
 * @brief Inserts a node into a k-d tree.
 *
 * @param root The root of the k-d tree.
 * @param kdim The k dimmension of the points.
 * @param point The point we want to store.
 * @param level The level of searching. It should always start from 0.
 * @return kd_node_t* Returns the root of the k-d tree.
 */
kd_node_t *insert_node(kd_node_t *root, unsigned int kdim, int *point,
					   unsigned int level);

/**
 * @brief Loads all the points from the well-formated file into the k-d
 * structure, using the insert function.
 *
 * @param file The file where are the points, already opened.
 * @param kdtree The k-d tree where to insert the points.
 */
void load_file(FILE *file, kd_tree_t *kdtree);

/**
 * @brief Frees all the k-d tree nodes.
 *
 * @param root The root of the k-d tree.
 */
void free_kdtree(kd_node_t *root);

/**
 * @brief Calculate the euclidean distance between 2 points.
 *
 * @param point1 The first point.
 * @param point2 The second point.
 * @param kdim The number of coordinates a point have.
 * @return double The distance between the 2 points.
 */
double get_distance(int *point1, int *point2, unsigned int kdim);

/**
 * @brief Prints a m x n matrix.
 *
 * @param matrix The matrix we want to print.
 * @param m The number of lines.
 * @param n The number of columns.
 */
void print_matrix(int **matrix, size_t m, size_t n);

/**
 * @brief Creates an ordering criteria between points.
 *
 * @param point1 One point to compare.
 * @param point2 The second point to compare.
 * @param kdim The k - number of coordinates.
 * @return int : 1, if point1 shoud be before point2, -1, otherwise, or 0,
 * it they're equal.
 */
int compare_points(const void *point1, const void *point2, unsigned int kdim);

/**
 * @brief Sorts the points stored as a matrix.
 *
 * @param points The matrix where the points are stored (each line is a point).
 * @param pts The number of points.
 * @param k The k (from k-d).
 */
void sort_points(int **points, int pts, unsigned int k);

/**
 * @brief Performs Nearest-Neighbour Search
 *
 * @param kdtree The k-d tree that contains the neighbours.
 * @param point The point which neighbour we want to find.
 */
void perform_nn(kd_tree_t *kdtree, int *point);

/**
 * @brief Gets the Nearest-Neighbours situated at the same minimum distance.
 *
 * @param root The root of k-d tree.
 * @param point The point which neighbour we want to find.
 * @param points The matrix where the found points will be stored.
 * @param pts_num The number of found points.
 * @param d The searching depth / level.
 * @param k The k.
 * @param min_dist The minimum distance found.
 * @param searches The number of searches.
 */
void get_nnpoints(kd_node_t *root, int *point, int **points, int *pts_num,
				  unsigned int d, unsigned int k, double *min_dist,
				  int *searches);

/**
 * @brief Performs Range Search
 *
 * @param kdtree The k-d tree where the range search is performed.
 * @param lower_bounds The lower bounds for the coordinates.
 * @param upper_bounds The upper bounds for the coordinates.
 */
void perform_rs(kd_tree_t *kdtree, int *lower_bounds, int *upper_bounds);

/**
 * @brief Gets the points in the given range.
 *
 * @param root The root of the k-d tree.
 * @param lower The lower bounds.
 * @param upper The upper bounds.
 * @param points The adrress of the matrix that will store the points, because
 * it will be reallocated inside the function.
 * @param pts_num The number of found points.
 * @param max_pts The maximum number of points that can be stored in the matrix
 * @param d The depth / level.
 * @param k The k.
 * @param searches The number of searches made.
 */
void get_rspoints(kd_node_t *root, int *lower, int *upper, int ***points,
				  int *pts_num, int *max_pts, unsigned int d, unsigned int k,
				  int *searches);

/**
 * @brief Check if the node is in the range given by the lower and upper.
 *
 * @param node The node we want to check.
 * @param lower The lower limits.
 * @param upper The upper limits.
 * @param k The k.
 * @return int Returns 1, if it's in range, 0, otherwise.
 */
int node_in_range(kd_node_t *node, int *lower, int *upper, unsigned int k);

#endif  // KD_TREE_H_
