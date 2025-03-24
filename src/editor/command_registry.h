#ifndef COMMAND_REGISTRY_H
#define COMMAND_REGISTRY_H

typedef struct Command {
    const char* name;
    const char* description;
    void (*callback)(char** argv);
} Command;

void cli_register_command(const char* name, Command* command);
Command* cli_lookup_command(const char* name);
void cli_execute_input(const char* line);
void cli_list_commands(void);
void init_command_registry(void);

#endif
