#ifndef OBSERVER_H
#define OBSERVER_H

#include "../editor/editor.h"

void editor_register_callback(enum EDITOR_EVENT event, callback_fn callback);
void editor_notify(enum EDITOR_EVENT event);

#endif

