#include <stdio.h>
#include <stdlib.h>

#include "file_set.h"
#include "walker.h"
#include "jsd.h"

int main(int argc, char *argv[]) {
    file_set set;
    file_set_init(&set);

    int had_error = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s file_or_directory...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        if (process_path(argv[i], &set) < 0) {
            had_error = 1;
        }
    }

    if (set.size < 2) {
        fprintf(stderr, "Error: fewer than two files to compare\n");
        file_set_free(&set);
        return EXIT_FAILURE;
    }

    size_t count = 0;
    comparison *arr = build_comparisons(&set, &count);
    qsort(arr, count, sizeof(comparison), cmp_comparisons);

    for (size_t i = 0; i < count; i++) {
        printf("%.5f %s %s\n",
               arr[i].jsd,
               arr[i].path1,
               arr[i].path2);
    }

    free(arr);
    file_set_free(&set);

    if (had_error) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
