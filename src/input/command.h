#ifndef COMMAND_H
#define COMMAND_H

#include "../init/init.h"
#include <stdbool.h>

struct editor_command {
    int key;
    void (*execute)(int);
    bool moves_cursor;
    const char* name;
};

void process_command(const char *command);
void editor_find(void);
void editor_save(void);
void editor_quit(void);

extern const struct editor_command EDITOR_COMMANDS[];
extern const size_t NUM_EDITOR_COMMANDS;

#endif

