#include <stdio.h>
#include <stdlib.h>
#include "wfd.h"

int main(void) {
    size_t total_words = 0;
    word_entry *head = build_wfd_from_file("test.txt", &total_words);

    if (head == NULL && total_words == 0) {
        printf("No words found or file could not be read.\n");
        return 1;
    }

    printf("total words: %zu\n", total_words);
    print_wfd(head);

    free_wfd(head);
    return 0;
}