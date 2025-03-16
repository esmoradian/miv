#include "observer.h"
#include "../init/init.h"

void editor_register_callback(enum editor_event event, callback_fn callback) {
    if (Editor.num_observers >= MAX_CALLBACKS) return;
    
    Editor.observers[Editor.num_observers].event = event;
    Editor.observers[Editor.num_observers].callback = callback;
    Editor.num_observers++;
}

void editor_notify(enum editor_event event) {
    for (int i = 0; i < Editor.num_observers; i++) {
        if (Editor.observers[i].event == event) {
            Editor.observers[i].callback();
        }
    }
} 
