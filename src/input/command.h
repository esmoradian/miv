#ifndef COMMAND_H
#define COMMAND_H

#include "../editor/editor.h"
#include <stdbool.h>

struct editor_command {
    int key;
    void (*execute)(int);
    enum editor_event event;
    const char* name;
};

void process_command(const char *command);
void editor_find(void);
void editor_save(int key);
void editor_quit(void);

extern const struct editor_command EDITOR_COMMANDS[];
extern const size_t NUM_EDITOR_COMMANDS;

#endif

