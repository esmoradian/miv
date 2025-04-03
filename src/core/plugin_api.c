#include "plugin_api.h"
#include <string.h>
#include <stdio.h>

cJSON *plugin_api_dispatch(const char *method, cJSON *params, int id) {
    if (strcmp(method, "miv.log") == 0) {
        const char *msg = cJSON_GetArrayItem(params, 0)->valuestring;
        printf("[plugin log] %s\n", msg);

        cJSON *res = cJSON_CreateObject();
        cJSON_AddStringToObject(res, "jsonrpc", "2.0");
        cJSON_AddStringToObject(res, "result", "ok");
        cJSON_AddNumberToObject(res, "id", id);
        return res;
    }

    cJSON *err = cJSON_CreateObject();
    cJSON_AddStringToObject(err, "jsonrpc", "2.0");
    cJSON_AddStringToObject(err, "error", "unknown method");
    cJSON_AddNumberToObject(err, "id", id);
    return err;
}
