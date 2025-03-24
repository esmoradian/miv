#ifndef EDITOR_H
#define EDITOR_H

#include <termios.h>
#include <stdbool.h>

#define CTRL_KEY(k) ((k) & 0x1f)
#define MIV_VERSION "0.0.1"
#define MAX_CALLBACKS 32

enum editor_key {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    BACKSPACE,
    SAVE_KEY,
};

enum editor_event {
    EVENT_NULL,
    EVENT_CURSOR_MOVE,
    EVENT_INSERT,
    EVENT_DELETE,
    EVENT_WINDOW_RESIZE,
    EVENT_FILE_OPEN,
    EVENT_NEWLINE,
    EVENT_SAVE,
    EVENT_COUNT
};

typedef struct editor_row {
    int size;
    char *chars;
} editor_row;

typedef void (*callback_fn)(void);

typedef struct editor_observer {
    enum editor_event event;
    callback_fn callback;
} editor_observer;

struct editor {
    char *filename;
    int cx, cy;
    int screenrows;
    int screencols;
    int numrows;
    editor_row *row;
    struct termios orig_termios;
    editor_observer observers[MAX_CALLBACKS];
    int num_observers;
};

extern struct editor Editor;

void init_editor(void);
void editor_open_file(const char* rel_path);

#endif

