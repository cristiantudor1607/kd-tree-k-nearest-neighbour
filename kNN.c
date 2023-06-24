#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "utils.h"
#include "structs.h"
#include "kd_tree.h"

/**
 * @brief Associates an unique id to some specific strings. If the string
 * hasn't an associated id, it gives back 0, meaning that it is an
 * unrecognized command / invalid command. Because there are few strings and
 * we don't need many ids the function returns an 8-bit unsigned integer.
 *
 * @param string The string we want to associate and id.
 * @return u8_t The id associated, or 0 in case the string isn't in the list.
 */
/* Disclaimer: it's copy-pasted from the mk.c, sorry */
u8_t parse_operation(char *input)
{
	if (strncmp(input, "LOAD", 4) == 0)
		return 1;

	if (strncmp(input, "NN", 2) == 0)
		return 2;

	if (strncmp(input, "RS", 2) == 0)
		return 3;

	if (strncmp(input, "EXIT", 4) == 0)
		return 4;

	return 0;
}

int main(void)
{
	char input[MAX_IN], string[MAX_STR];
	kd_tree_t *kdtree = init_kdtree();
	int *buff1;
	int *buff2;
	u8_t cmd = 0;
	do {
		scanf("%s", input);
		cmd = parse_operation(input);
		switch (cmd) {
		case 1:
			scanf("%s", string);
			FILE *file = fopen(string, "rt");
			DIE(!file, "Couldn't open the file. Please try again\n");
			kdtree->kdim = get_kdim(file);
			buff1 = malloc(kdtree->kdim * sizeof(int));
			DIE(!buff1, MEMFAIL);
			buff2 = malloc(kdtree->kdim * sizeof(int));
			DIE(!buff2, MEMFAIL);
			kdtree->nnodes = get_points_number(file);
			load_file(file, kdtree);
			fclose(file);
			break;
		case 2:
			for (unsigned int i = 0; i < kdtree->kdim; i++)
				scanf("%d", &buff1[i]);
			perform_nn(kdtree, buff1);
			break;
		case 3:
			for (unsigned int i = 0; i < kdtree->kdim; i++) {
				scanf("%d", &buff1[i]);
				scanf("%d", &buff2[i]);
			}
			perform_rs(kdtree, buff1, buff2);
			break;
		case 4:
			free_kdtree(kdtree->root);
			free(kdtree);
			free(buff1);
			free(buff2);
			break;
		default:
			break;
		}
	} while (cmd != 4);

	return 0;
}
