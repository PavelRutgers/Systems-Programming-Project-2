#ifndef JSD_H
#define JSD_H

#include <stddef.h>
#include "file_set.h"
#include "wfd.h"

typedef struct {
    const char *path1;
    const char *path2;
    size_t combined_words;
    double jsd;
} comparison;

double compute_jsd(const word_entry *a, const word_entry *b);
comparison *build_comparisons(const file_set *set, size_t *out_count);
int cmp_comparisons(const void *a, const void *b);

#endif