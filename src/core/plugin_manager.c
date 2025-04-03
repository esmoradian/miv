#include "plugin_manager.h"
#include "plugin_api.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

PluginManager *plugin_manager_create(void) {
    PluginManager *pm = malloc(sizeof(PluginManager));
    pm->count = 0;
    pm->capacity = 4;
    pm->plugins = malloc(sizeof(Plugin *) * pm->capacity);
    return pm;
}

void plugin_manager_register(PluginManager *pm, Plugin *plugin) {
    if (pm->count >= pm->capacity) {
        pm->capacity *= 2;
        pm->plugins = realloc(pm->plugins, sizeof(Plugin *) * pm->capacity);
    }
    pm->plugins[pm->count++] = plugin;
}

Plugin *plugin_spawn(const char *path) {
    int in_pipe[2], out_pipe[2];
    if (pipe(in_pipe) == -1 || pipe(out_pipe) == -1) return NULL;

    pid_t pid = fork();
    if (pid == -1) return NULL;

    if (pid == 0) {
        dup2(out_pipe[0], STDIN_FILENO);
        dup2(in_pipe[1], STDOUT_FILENO);
        close(in_pipe[0]);
        close(out_pipe[1]);
        execl(path, path, NULL);
        exit(1);
    }

    close(in_pipe[1]);
    close(out_pipe[0]);

    Plugin *plugin = malloc(sizeof(Plugin));
    plugin->stdin_fp = fdopen(out_pipe[1], "w");
    plugin->stdout_fp = fdopen(in_pipe[0], "r");
    plugin->pid = pid;
    plugin->name = strdup(path);
    return plugin;
}

void plugin_send_json(FILE *fp, cJSON *json) {
    char *str = cJSON_PrintUnformatted(json);
    fprintf(fp, "%s\n", str);
    fflush(fp);
    free(str);
}

void plugin_manager_poll(PluginManager *pm) {
    for (int i = 0; i < pm->count; ++i) {
        Plugin *plugin = pm->plugins[i];
        char buffer[2048];
        if (fgets(buffer, sizeof(buffer), plugin->stdout_fp)) {
            cJSON *msg = cJSON_Parse(buffer);
            if (msg) {
                const char *method = cJSON_GetObjectItem(msg, "method")->valuestring;
                cJSON *params = cJSON_GetObjectItem(msg, "params");
                int id = cJSON_GetObjectItem(msg, "id")->valueint;

                cJSON *response = plugin_api_dispatch(method, params, id);
                plugin_send_json(plugin->stdin_fp, response);
                cJSON_Delete(response);
                cJSON_Delete(msg);
            }
        }
    }
}
