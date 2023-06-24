#ifndef UTILS_H_
#define UTILS_H_

#include <errno.h>

#define DIE(assertion, call_description)                                       \
	do {                                                                       \
		if (assertion) {                                                       \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                 \
			perror(call_description);                                          \
			exit(errno);                                                       \
		}                                                                      \
	} while (0)

#define MEMFAIL "Oops! Memory allocation failed. Please try again.\n"
#define ALPH 26
#define MAX_BUFF 100
#define INF 1000000
#define MAX_IN 20
#define MAX_STR 100
#define PTS 5

#endif  // UTILS_H_
