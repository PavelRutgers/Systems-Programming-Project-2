#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "walker.h"

static int has_txt_suffix(const char *name) {
    size_t len = strlen(name);
    return (len >= 4 && strcmp(name + len - 4, ".txt") == 0);
}

int process_path(const char *path, file_set *set) {
    struct stat st;

    if (stat(path, &st) < 0) {
        perror(path);
        return -1;
    }

    if (S_ISREG(st.st_mode)) {
        return file_set_add(set, path);
    }

    if (S_ISDIR(st.st_mode)) {
        return walk_directory(path, set);
    }

    return 0;
}

int walk_directory(const char *dirpath, file_set *set) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        perror(dirpath);
        return -1;
    }

    struct dirent *entry;
    int had_error = 0;

    while ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;

        if (name[0] == '.') {
            continue;
        }

        char fullpath[4096];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, name);

        struct stat st;
        if (stat(fullpath, &st) < 0) {
            perror(fullpath);
            had_error = -1;
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            if (walk_directory(fullpath, set) < 0) {
                had_error = -1;
            }
        } else if (S_ISREG(st.st_mode) && has_txt_suffix(name)) {
            if (file_set_add(set, fullpath) < 0) {
                had_error = -1;
            }
        }
    }

    closedir(dir);
    return had_error;
}
