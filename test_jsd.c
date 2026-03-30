#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "jsd.h"

int main(void) {
    size_t total1 = 0, total2 = 0;
    word_entry *wfd1 = build_wfd_from_file("f1.txt", &total1);
    word_entry *wfd2 = build_wfd_from_file("f2.txt", &total2);

    double d = compute_jsd(wfd1, wfd2);
    printf("JSD: %.4f\n", d);

    free_wfd(wfd1);
    free_wfd(wfd2);
    return 0;
}