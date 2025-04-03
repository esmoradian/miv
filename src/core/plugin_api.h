#ifndef PLUGIN_API_H
#define PLUGIN_API_H

#include "../external/cjson/cJSON.h"
#include "plugin.h"

cJSON *plugin_api_dispatch(const char *method, cJSON *params, int id);

#endif
