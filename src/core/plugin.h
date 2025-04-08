#ifndef PLUGIN_H
#define PLUGIN_H

#include <stdio.h>
#include <unistd.h>

typedef struct Plugin {
    const char *name;
    FILE *stdin_fp;
    FILE *stdout_fp;
    pid_t pid;
} Plugin;

#endif
