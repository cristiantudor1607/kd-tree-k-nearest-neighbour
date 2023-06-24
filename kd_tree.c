#include "kd_tree.h"

/**
 * Disclaimer: I'm sorry, but I ran out of time, and I can't add enough
 * comments here, in such a small time. I finished this one week ago, and
 * I completely forgot about this. In case you see this first, I tried my
 * best with the other files. I will let the code as it is, with the comments
 * written when I did it first. Thank you and keep up the good work!
 */
kd_tree_t *init_kdtree(void)
{
	kd_tree_t *new_kdtree = malloc(sizeof(kd_tree_t));
	DIE(!new_kdtree, MEMFAIL);

	new_kdtree->root = NULL;
	new_kdtree->kdim = 0;
	new_kdtree->nnodes = 0;

	return new_kdtree;
}

unsigned int get_kdim(FILE *file)
{
	fseek(file, SEEK_SET, 0);
	size_t irrelevant;
	unsigned int kdim;
	fscanf(file, "%lu %u", &irrelevant, &kdim);

	return kdim;
}

size_t get_points_number(FILE *file)
{
	fseek(file, SEEK_SET, 0);
	size_t num = 0;
	fscanf(file, "%lu", &num);

	return num;
}

kd_node_t *init_kdnode(int *point, unsigned int kdim)
{
	kd_node_t *new_node = (kd_node_t *)malloc(sizeof(kd_node_t));
	DIE(!new_node, MEMFAIL);

	new_node->data = (int *)malloc(kdim * sizeof(int));
	DIE(!new_node->data, MEMFAIL);

	memcpy(new_node->data, point, kdim * sizeof(int));

	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = NULL;

	return new_node;
}

kd_node_t *insert_node(kd_node_t *root, unsigned int kdim, int *point,
					   unsigned int level)
{
	if (!root) {
		kd_node_t *new_node = init_kdnode(point, kdim);
		return new_node;
	}

	unsigned int idx = level % kdim;
	int curr_coord = point[idx];
	int root_coord = ((int *)root->data)[idx];

	if (curr_coord < root_coord) {
		root->left = insert_node(root->left, kdim, point, level + 1);
		root->left->parent = root;
	} else {
		root->right = insert_node(root->right, kdim, point, level + 1);
		root->right->parent = root;
	}

	// Actually, it returns the same root every time, the root given as param
	return root;
}

void load_file(FILE *file, kd_tree_t *kdtree)
{
	fseek(file, SEEK_SET, 0);
	unsigned int irrelevant;
	fscanf(file, "%u", &irrelevant);
	fscanf(file, "%u", &irrelevant);

	int *temp_arr = malloc(kdtree->kdim * sizeof(int));
	DIE(!temp_arr, MEMFAIL);

	for (size_t i = 0; i < kdtree->nnodes; i++) {
		for (unsigned int j = 0; j < kdtree->kdim; j++)
			fscanf(file, "%d", &temp_arr[j]);

		kdtree->root = insert_node(kdtree->root, kdtree->kdim, temp_arr, 0);
	}

	free(temp_arr);
}

void free_kdtree(kd_node_t *root)
{
	if (!root)
		return;

	free_kdtree(root->left);
	free_kdtree(root->right);

	free(root->data);
	free(root);
}

double get_distance(int *point1, int *point2, unsigned int kdim)
{
	unsigned int sum = 0;
	for (unsigned int i = 0; i < kdim; i++)
		sum += pow(point1[i] - point2[i], 2);

	return sqrt(sum);
}

void print_matrix(int **matrix, size_t m, size_t n)
{
	for (size_t i = 0; i < m; i++) {
		for (size_t j = 0; j < n; j++)
			printf("%d ", matrix[i][j]);

		printf("\n");
	}
}

int compare_points(const void *point1, const void *point2, unsigned int kdim)
{
	int *p1 = (int *)point1;
	int *p2 = (int *)point2;

	for (unsigned int i = 0; i < kdim; i++) {
		if (p1[i] > p2[i])
			return 1;

		if (p1[i] < p2[i])
			return -1;
	}

	return 0;
}

void sort_points(int **points, int pts, unsigned int k)
{
	int *aux = malloc(k * sizeof(int));
	DIE(!aux, MEMFAIL);

	for (int i = 0; i < pts - 1; i++) {
		for (int j = 0; j < pts; j++) {
			if (compare_points(points[i], points[j], k) == 1) {
				memcpy(aux, points[i], k * sizeof(int));
				memcpy(points[i], points[j], k * sizeof(int));
				memcpy(points[j], aux, k * sizeof(int));
			}
		}
	}

	free(aux);
}

void get_nnpoints(kd_node_t *root, int *point, int **points, int *pts_num,
				  unsigned int d, unsigned int k, double *min_dist,
				  int *searches)
{
	if (!root)
		return;

	double dist = get_distance(point, (int *)root->data, k);
	*searches += 1;

	if (dist == *min_dist) {
		for (unsigned int i = 0; i < k; i++)
			points[*pts_num][i] = ((int *)root->data)[i];

		*pts_num += 1;
	}

	if (dist < *min_dist) {
		*pts_num = 0;
		for (unsigned int i = 0; i < k; i++)
			points[*pts_num][i] = ((int *)root->data)[i];

		*pts_num += 1;

		*min_dist = dist;
	}

	unsigned int idx = d % k;

	// calculate the minimum distance possible from the point to the box
	int relative_position = point[idx] - ((int *)root->data)[idx];
	double possible_min = abs(relative_position);

	// Now, search in the box where the point is, otherwise, continue the
	// search in the other box

	if (relative_position < 0)
		get_nnpoints(root->left, point, points, pts_num, d + 1, k, min_dist,
					 searches);
	else
		get_nnpoints(root->right, point, points, pts_num, d + 1, k, min_dist,
					 searches);

	if (possible_min < *min_dist && relative_position < 0)
		get_nnpoints(root->right, point, points, pts_num, d + 1, k, min_dist,
					 searches);
	else if (possible_min < *min_dist)
		get_nnpoints(root->left, point, points, pts_num, d + 1, k, min_dist,
					 searches);
}

void perform_nn(kd_tree_t *kdtree, int *point)
{
	// consider that there are maximum PTS points situated at equal distance
	int **found_points = malloc(PTS * sizeof(int *));
	DIE(!found_points, MEMFAIL);

	for (int i = 0; i < PTS; i++) {
		found_points[i] = calloc(kdtree->kdim, sizeof(int));
		DIE(!found_points[i], MEMFAIL);
	}

	double min_dist = INF;
	int pts = 0;
	int searches = 0;

	get_nnpoints(kdtree->root, point, found_points, &pts, 0,
				 kdtree->kdim, &min_dist, &searches);

	sort_points(found_points, pts, kdtree->kdim);
	print_matrix(found_points, pts, kdtree->kdim);

	for (int i = 0; i < PTS; i++)
		free(found_points[i]);

	free(found_points);
}

int node_in_range(kd_node_t *node, int *lower, int *upper, unsigned int k)
{
	int *v = (int *)node->data;
	for (unsigned int i = 0; i < k; i++) {
		if (v[i] < lower[i] || v[i] > upper[i])
			return 0;
	}

	return 1;
}

void get_rspoints(kd_node_t *root, int *lower, int *upper, int ***points,
				  int *pts_num, int *max_pts, unsigned int d, unsigned int k,
				  int *searches)
{
	if (!root)
		return;

	*searches += 1;
	unsigned int idx = d % k;
	int is_in_range = node_in_range(root, lower, upper, k);

	if (*pts_num == *max_pts) {
		*max_pts = *max_pts + PTS;
		*points = (int **)realloc(*points, *max_pts * sizeof(int *));
		DIE(!(*points), MEMFAIL);
		for (int i = *pts_num; i < *max_pts; i++) {
			(*points)[i] = malloc(k * sizeof(int));
			DIE(!(*points)[i], MEMFAIL);
		}
	}

	// if the point is in range
	if (is_in_range == 1) {
		for (unsigned int i = 0; i < k; i++)
			(*points)[*pts_num][i] = ((int *)root->data)[i];

		*pts_num += 1;

		// i should search in both sides for other points
		get_rspoints(root->left, lower, upper, points, pts_num, max_pts, d + 1,
					 k, searches);

		get_rspoints(root->right, lower, upper, points, pts_num, max_pts, d + 1,
					 k, searches);

		return;
	}

	// if it is not in range
	int relative_position = ((int *)root->data)[idx] - lower[idx];

	// if current value is less than the lower limit, I should search
	// in the right side
	if (relative_position <= 0) {
		get_rspoints(root->right, lower, upper, points, pts_num, max_pts, d + 1,
					 k, searches);
		return;
	}

	relative_position = ((int *)root->data)[idx] - upper[idx];

	if (relative_position >= 0) {
		get_rspoints(root->left, lower, upper, points, pts_num, max_pts, d + 1,
					 k, searches);
		return;
	}

	get_rspoints(root->left, lower, upper, points, pts_num, max_pts, d + 1,
				 k, searches);

	get_rspoints(root->right, lower, upper, points, pts_num, max_pts, d + 1,
				 k, searches);
}

void perform_rs(kd_tree_t *kdtree, int *lower_bounds, int *upper_bounds)
{
	int pts = 0;

	int **found_points = malloc(PTS * sizeof(int *));
	DIE(!found_points, MEMFAIL);

	// the array of points (actually a matrix), will grow from PTS to PTS
	for (unsigned int i = 0; i < PTS; i++) {
		found_points[i] = calloc(kdtree->kdim, sizeof(int));
		DIE(!found_points[i], MEMFAIL);
	}

	int max_pts = PTS;
	int serch = 0;
	get_rspoints(kdtree->root, lower_bounds, upper_bounds, &found_points, &pts,
				 &max_pts, 0, kdtree->kdim, &serch);

	sort_points(found_points, pts, kdtree->kdim);
	print_matrix(found_points, pts, kdtree->kdim);

	for (int i = 0; i < max_pts; i++)
		free(found_points[i]);

	free(found_points);
}
