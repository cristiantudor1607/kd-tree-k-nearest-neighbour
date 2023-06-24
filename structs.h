#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <inttypes.h>

// I wanted to use some types from inttypes, but the coding style checker is
// against me
typedef unsigned char u8_t;
typedef signed int s32_t;
typedef unsigned long u64_t;

enum state { END, NOT_END, ROOT };
typedef enum state state_t;

typedef struct g_node_t g_node_t;
struct g_node_t {
	void *data; // data stored in node
	g_node_t *parent; // parent of the node
	g_node_t **children; // children of the node
	u8_t children_num; // number of children of the node
};

typedef struct g_tree_t g_tree_t;
struct g_tree_t {
	g_node_t *root;	// root of the generic tree
	u64_t data_size; // the size of the data stored in the nodes
	u64_t keys_no; // the number of keys stored in the tree
	void (*free_func)(void *data);	// function that frees the data
									// within the node
};

typedef struct key_t key_t;
struct key_t {
	char key; // the actual letter within the node
	state_t ending; // a state variable, to check if the node is the end of
					// a key
	s32_t freq;	// frequency of the key
	size_t key_len;	// the length of the key
};

typedef struct kd_node_t kd_node_t;
struct kd_node_t {
	void *data;	// data stored in the node
	kd_node_t *left; // the left child of the node
	kd_node_t *right; // the right child of the node
	kd_node_t *parent; // the parent of the node
};

typedef struct kd_tree_t kd_tree_t;
struct kd_tree_t {
	kd_node_t *root; // root of the k-d tree
	unsigned int kdim;	// the number of elements that forms a point
					// actually, the k value
	size_t nnodes;	// number of nodes = number of points
};

#endif	// STRUCTS_H_
