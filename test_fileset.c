#include <stdio.h>
#include "file_set.h"

int main(void) {
    file_set set;
    file_set_init(&set);

    file_set_add(&set, "test.txt");
    file_set_add(&set, "test2.txt");

    for (size_t i = 0; i < set.size; i++) {
        printf("File: %s\n", set.files[i].path);
        printf("Total words: %zu\n", set.files[i].total_words);
        print_wfd(set.files[i].wfd);
        printf("\n");
    }

    file_set_free(&set);
    return 0;
}