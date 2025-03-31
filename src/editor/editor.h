#ifndef EDITOR_H
#define EDITOR_H

#include <termios.h>
#include <stdbool.h>

#define CTRL_KEY(k) ((k) & 0x1f)
#define MIV_VERSION "0.0.1"
#define MAX_CALLBACKS 32

enum EDITOR_KEY {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    BACKSPACE,
    SAVE_KEY,
};

enum EDITOR_EVENT {
    EVENT_CURSOR_MOVE,
    EVENT_INSERT,
    EVENT_DELETE,
    EVENT_WINDOW_RESIZE,
    EVENT_FILE_OPEN,
    EVENT_NEWLINE,
    EVENT_COUNT,
    EVENT_SAVE,
    EVENT_BUFFER_SAVED,
    EVENT_STATUS_UPDATED,
    EVENT_NULL
};

typedef struct EditorRow {
    int size;
    char *chars;
} EditorRow;

typedef void (*callback_fn)(void);

typedef struct EditorObserver {
    enum EDITOR_EVENT event;
    callback_fn callback;
} EditorObserver;

typedef struct Editor {
    char *filename;
    int cx, cy;
    int screenrows;
    int screencols;
    int numrows;
    EditorRow *row;
    struct termios orig_termios;
    EditorObserver observers[MAX_CALLBACKS];
    int num_observers;
} Editor;

extern struct Editor editor;

void init_editor(void);
void editor_open_file(const char* rel_path);

#endif

