#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_set.h"

void file_set_init(file_set *set) {
    set->files = NULL;
    set->size = 0;
    set->capacity = 0;
}
static char *copy_string(const char *s) {
    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (!copy) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(copy, s);
    return copy;
}

void file_set_add(file_set *set, const char *path) {
    // grow array if needed
    if (set->size == set->capacity) {
        size_t new_cap = (set->capacity == 0) ? 4 : set->capacity * 2;
        file_info *tmp = realloc(set->files, new_cap * sizeof(file_info));
        if (!tmp) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        set->files = tmp;
        set->capacity = new_cap;
    }

    // build WFD
    size_t total_words = 0;
    word_entry *wfd = build_wfd_from_file(path, &total_words);

    if (!wfd) {
        // error already printed in build_wfd
        return;
    }

    // store data
    set->files[set->size].path = copy_string(path);
    set->files[set->size].total_words = total_words;
    set->files[set->size].wfd = wfd;

    set->size++;
}
void file_set_free(file_set *set) {
    for (size_t i = 0; i < set->size; i++) {
        free(set->files[i].path);
        free_wfd(set->files[i].wfd);
    }

    free(set->files);
}