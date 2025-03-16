#ifndef EDITOR_H
#define EDITOR_H

#include <termios.h>
#include <stdbool.h>

/*** defines ***/
#define CTRL_KEY(k) ((k) & 0x1f)
#define MIV_VERSION "0.0.1"
#define MAX_CALLBACKS 32

/*** enums ***/
enum editor_key {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    BACKSPACE
};

enum editor_event {
    EVENT_CURSOR_MOVE,
    EVENT_INSERT,
    EVENT_DELETE,
    EVENT_WINDOW_RESIZE,
    EVENT_FILE_OPEN,
    EVENT_NEWLINE,
    EVENT_COUNT
};

/*** data structures ***/
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
    int cx, cy;          // Cursor x and y position
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

#endif

