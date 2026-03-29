#ifndef WFD_H
#define WFD_H

#include <stddef.h>

typedef struct word_entry {
    char *word;
    size_t count;
    double freq;
    struct word_entry *next;
} word_entry;

word_entry *insert_or_increment(word_entry *head, const char *word);
word_entry *build_wfd_from_file(const char *path, size_t *total_words);
void compute_frequencies(word_entry *head, size_t total_words);
void free_wfd(word_entry *head);
void print_wfd(const word_entry *head);

#endif