#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "jsd.h"

double compute_jsd(const word_entry *a, const word_entry *b) {
    double kld1 = 0.0;
    double kld2 = 0.0;

    const word_entry *curr1 = a;
    const word_entry *curr2 = b;

    while (curr1 != NULL && curr2 != NULL) {
        int cmp = strcmp(curr1->word, curr2->word);

        if (cmp == 0) {
            double f1 = curr1->freq;
            double f2 = curr2->freq;
            double mean = 0.5 * (f1 + f2);

            if (f1 > 0.0) {
                kld1 += f1 * log2(f1 / mean);
            }
            if (f2 > 0.0) {
                kld2 += f2 * log2(f2 / mean);
            }

            curr1 = curr1->next;
            curr2 = curr2->next;
        }
        else if (cmp < 0) {
            double f1 = curr1->freq;
            double mean = 0.5 * f1;

            if (f1 > 0.0) {
                kld1 += f1 * log2(f1 / mean);
            }

            curr1 = curr1->next;
        }
        else {
            double f2 = curr2->freq;
            double mean = 0.5 * f2;

            if (f2 > 0.0) {
                kld2 += f2 * log2(f2 / mean);
            }

            curr2 = curr2->next;
        }
    }

    while (curr1 != NULL) {
        double f1 = curr1->freq;
        double mean = 0.5 * f1;

        if (f1 > 0.0) {
            kld1 += f1 * log2(f1 / mean);
        }

        curr1 = curr1->next;
    }

    while (curr2 != NULL) {
        double f2 = curr2->freq;
        double mean = 0.5 * f2;

        if (f2 > 0.0) {
            kld2 += f2 * log2(f2 / mean);
        }

        curr2 = curr2->next;
    }

    return sqrt(0.5 * kld1 + 0.5 * kld2);
}
comparison *build_comparisons(const file_set *set, size_t *out_count) {
    size_t n = set->size;

    // number of unordered pairs
    size_t total = n * (n - 1) / 2;
    *out_count = total;

    comparison *arr = malloc(total * sizeof(comparison));
    if (!arr) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    size_t idx = 0;

    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            const file_info *f1 = &set->files[i];
            const file_info *f2 = &set->files[j];

            arr[idx].path1 = f1->path;
            arr[idx].path2 = f2->path;
            arr[idx].combined_words = f1->total_words + f2->total_words;
            arr[idx].jsd = compute_jsd(f1->wfd, f2->wfd);

            idx++;
        }
    }

    return arr;
}
int cmp_comparisons(const void *a, const void *b) {
    const comparison *c1 = a;
    const comparison *c2 = b;

    if (c2->combined_words > c1->combined_words) return 1;
    if (c2->combined_words < c1->combined_words) return -1;
    return 0;
}