#ifndef FILE_SET_H
#define FILE_SET_H

#include <stddef.h>
#include "wfd.h"

typedef struct {
    char *path;
    size_t total_words;
    word_entry *wfd;
} file_info;

typedef struct {
    file_info *files;
    size_t size;
    size_t capacity;
} file_set;

void file_set_init(file_set *set);
void file_set_add(file_set *set, const char *path);
void file_set_free(file_set *set);

#endif