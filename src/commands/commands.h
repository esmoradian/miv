#ifndef COMMANDS_H
#define COMMANDS_H 

#include <stddef.h>
#include "../input/events.h"

typedef struct Command {
    const char* name;
    const char* description;
    void (*callback)(InputContext *ctx);
} Command;

extern Command builtins[];
extern size_t num_builtins;

void cmd_write(InputContext *ctx);
void cmd_quit(InputContext *ctx);
void cmd_open(InputContext *ctx);

#endif
