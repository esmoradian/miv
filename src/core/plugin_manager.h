#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "plugin.h"
#include "../external/cjson/cJSON.h"

typedef struct PluginManager {
    Plugin **plugins;
    int count;
    int capacity;
} PluginManager;

PluginManager *plugin_manager_create(void);
void plugin_manager_register(PluginManager *pm, Plugin *plugin);
void plugin_manager_poll(PluginManager *pm);
Plugin *plugin_spawn(const char *path);
void plugin_send_json(FILE *fp, cJSON *json);

#endif
