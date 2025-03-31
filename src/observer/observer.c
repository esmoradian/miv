#include "observer.h"
#include "../editor/editor.h"

void editor_register_callback(enum EDITOR_EVENT event, callback_fn callback) {
    if (editor.num_observers >= MAX_CALLBACKS) return;
    
    editor.observers[editor.num_observers].event = event;
    editor.observers[editor.num_observers].callback = callback;
    editor.num_observers++;
}

void editor_notify(enum EDITOR_EVENT event) {
    for (int i = 0; i < editor.num_observers; i++) {
        if (editor.observers[i].event == event) {
            editor.observers[i].callback();
        }
    }
} 
