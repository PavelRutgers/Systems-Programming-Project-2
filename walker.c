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
        // regular file → always include
        file_set_add(set, path);
    } 
    else if (S_ISDIR(st.st_mode)) {
        // directory → recurse
        walk_directory(path, set);
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

    while ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;

        // skip . and .. and hidden files
        if (name[0] == '.') {
            continue;
        }

        // build full path
        char fullpath[4096];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, name);

        struct stat st;
        if (stat(fullpath, &st) < 0) {
            perror(fullpath);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            walk_directory(fullpath, set);
        }
        else if (S_ISREG(st.st_mode)) {
            if (has_txt_suffix(name)) {
                file_set_add(set, fullpath);
            }
        }
    }

    closedir(dir);
    return 0;
}