#ifndef OBSERVER_H
#define OBSERVER_H

#include "../editor/editor.h"

void editor_register_callback(enum editor_event event, callback_fn callback);
void editor_notify(enum editor_event event);

#endif

