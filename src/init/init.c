#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "init.h"
#include "../events/observer.h"
#include "../terminal/terminal.h"
#include "../output/screen.h"
#include "../input/error.h"

static void init_editor_state(void) {
    Editor.cx = 0;
    Editor.cy = 0;
    Editor.numrows = 0;
    Editor.row = NULL;
    Editor.num_observers = 0;
}

static void init_event_handlers(void) {
    editor_register_callback(EVENT_CURSOR_MOVE, editor_refresh_screen);
    editor_register_callback(EVENT_INSERT, editor_refresh_screen);
    editor_register_callback(EVENT_DELETE, editor_refresh_screen);
    editor_register_callback(EVENT_WINDOW_RESIZE, editor_refresh_screen);
    editor_register_callback(EVENT_FILE_OPEN, editor_refresh_screen);
    editor_register_callback(EVENT_NEWLINE, editor_refresh_screen);
}

static void init_terminal_size(void) {
    if (get_window_size(&Editor.screenrows, &Editor.screencols) == -1) {
        die("getWindowSize");
    }
}

void init_editor(void) {
    init_editor_state();
    init_event_handlers();
    init_terminal_size();
} 
