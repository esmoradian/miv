#include "commands.h"
#include "command_registry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 128

typedef struct CommandEntry {
    const char* key;
    Command* command;
    struct CommandEntry* next;
} CommandEntry;

static CommandEntry* command_table[HASH_TABLE_SIZE] = {0};

// djb2 hash function
static unsigned long hash(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_TABLE_SIZE;
}

void cli_register_command(const char* name, Command* command) {
    unsigned long index = hash(name);
    CommandEntry* entry = malloc(sizeof(CommandEntry));
    entry->key = name;
    entry->command = command;
    entry->next = command_table[index];
    command_table[index] = entry;
}

Command* cli_lookup_command(const char* name) {
    unsigned long index = hash(name);
    CommandEntry* entry = command_table[index];
    while (entry) {
        if (strcmp(entry->key, name) == 0) {
            return entry->command;
        }
        entry = entry->next;
    }
    return NULL;
}

void cli_execute_input(const char* line) {
    char buffer[256];
    strncpy(buffer, line, sizeof(buffer));
    buffer[sizeof(buffer)-1] = '\0';

    char* argv[16] = {0};
    int argc = 0;
    char* token = strtok(buffer, " \t\n");
    while (token && argc < 15) {
        argv[argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    argv[argc] = NULL;

    if (argc == 0) return;

    Command* cmd = cli_lookup_command(argv[0]);
    if (cmd) {
        cmd->callback(argv);
    } else {
        printf("Unknown command: %s\n", argv[0]);
    }
}

void cli_list_commands(void) {
    printf("Available commands:\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        CommandEntry* entry = command_table[i];
        while (entry) {
           printf("- %s: %s\n", entry->key, entry->command->description);
            entry = entry->next;
        }
    }
}

void init_command_registry(void) {
    for (size_t i = 0; i < num_builtins; i++) {
        cli_register_command(builtins[i].name, &builtins[i]);
    }
}

