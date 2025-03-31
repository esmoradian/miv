#ifndef COMMAND_H
#define COMMAND_H

#include "../editor/editor.h"
#include <stddef.h>

// Under consideration with plugins in mind
typedef struct InputContext {
    int key;
    int argc;
    char *argv[8];  // Fixed-size argv-like array, adjust later
    const char *cli_args; // Raw CLI-style string ex ":write notes.txt"
} InputContext;

typedef struct EventListener {
    int key;
    void (*execute)(InputContext *context);
    enum EDITOR_EVENT *events;
    const char *name;
} EventListener;

extern const EventListener event_listeners[];
extern const size_t num_event_listeners;

#endif
