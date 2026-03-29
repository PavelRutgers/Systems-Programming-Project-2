#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include "wfd.h"

static char *copy_string(const char *s){
    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (copy == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(copy, s);
    return copy;
}

word_entry *insert_or_increment(word_entry *head, const char *word){
    word_entry *prev = NULL;
    word_entry *curr = head;
    
    while (curr != NULL && strcmp(curr->word,word) < 0){
        prev = curr;
        curr = curr->next;
    }
    if (curr != NULL && strcmp(curr->word, word) == 0){
        curr->count++;
        return head;
    }
    word_entry *new_node = malloc(sizeof(word_entry));
    if (new_node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_node->word = copy_string(word);
    new_node->count = 1;
    new_node->freq = 0.0;
    new_node->next = curr;

    if (prev == NULL) {
        return new_node;
    }

    prev->next = new_node;
    return head;
}

void compute_frequencies(word_entry *head, size_t total_words) {
    for (word_entry *curr = head; curr != NULL; curr = curr->next) {
        curr->freq = (double)curr->count / total_words;
    }
}

void free_wfd(word_entry *head) {
    while (head!= NULL){
        word_entry *next = head->next;
        free(head->word);
        free(head);
        head = next;
    }
}

void print_wfd(const word_entry *head) {
    while (head != NULL) {
        printf("%s: count=%zu freq=%f\n", head->word, head->count, head->freq);
        head = head->next;
    }
}

static int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '-';
}

word_entry *build_wfd_from_file(const char *path, size_t *total_words) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror(path);
        return NULL;
    }

    char buffer[4096];
    ssize_t bytes_read;

    word_entry *head = NULL;
    *total_words = 0;

    // dynamic word buffer
    char *word = NULL;
    size_t word_len = 0;
    size_t word_cap = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            char c = buffer[i];

            if (is_word_char(c)) {
                // grow word buffer if needed
                if (word_len + 1 >= word_cap) {
                    word_cap = (word_cap == 0) ? 16 : word_cap * 2;
                    char *tmp = realloc(word, word_cap);
                    if (!tmp) {
                        perror("realloc");
                        close(fd);
                        exit(EXIT_FAILURE);
                    }
                    word = tmp;
                }

                word[word_len++] = tolower((unsigned char)c);
            } else if (isspace((unsigned char)c)) {
                // end of a word
                if (word_len > 0) {
                    word[word_len] = '\0';
                    head = insert_or_increment(head, word);
                    (*total_words)++;
                    word_len = 0;
                }
            }
            // ignore punctuation (do nothing)
        }
    }

    if (bytes_read < 0) {
        perror(path);
    }

    // handle last word if file doesn't end with whitespace
    if (word_len > 0) {
        word[word_len] = '\0';
        head = insert_or_increment(head, word);
        (*total_words)++;
    }

    free(word);
    close(fd);

    // compute frequencies
    if (*total_words > 0) {
        compute_frequencies(head, *total_words);
    }

    return head;
}