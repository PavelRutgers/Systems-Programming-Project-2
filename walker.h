#ifndef WALKER_H
#define WALKER_H

#include <stddef.h>
#include "file_set.h"
#include "wfd.h"

int process_path(const char *path, file_set *set);
int walk_directory(const char *dirpath, file_set *set);

#endif
