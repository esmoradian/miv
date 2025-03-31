#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "keyboard.h"
#include "events.h"
#include "error.h"
#include "../editor/editor.h"
#include "../observer/observer.h"
#include "../buffer/row.h"

void handle_arrow_key(InputContext *ctx) {
    editor_move_cursor(ctx);
}

void handle_delete(InputContext *ctx) {
    (void)ctx;
    editor_delete_char();
}

void handle_backspace(InputContext *ctx) {
    (void)ctx;
    editor_backspace_char();
}

void handle_newline(InputContext *ctx) {
    (void)ctx;
    editor_insert_newline();
}

void handle_char_insert(InputContext *ctx) {
    if (!iscntrl(ctx->key)) {
        editor_insert_char(ctx);
    }
}

int editor_read_key(void) {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) die("read");
    }

    if (c == '\x1b') {
        char seq[3];

        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                if (seq[2] == '~') {
                    switch (seq[1]) {
                        case '3': return DEL_KEY;
                    }
                }
            } else {
                switch (seq[1]) {
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                }
            }
        }

        return '\x1b';
    } else {
        return c;
    }
}

void editor_move_cursor(InputContext *ctx) {
    EditorRow *row = (editor.cy >= editor.numrows) ? NULL : &editor.row[editor.cy];

    int key = ctx->key;

    switch (key) {
        case ARROW_LEFT:
            if (editor.cx != 0) {
                editor.cx--;
            } else if (editor.cy > 0) {
                editor.cy--;
                editor.cx = editor.row[editor.cy].size;
            }
            break;
        case ARROW_RIGHT:
            if (row && editor.cx < row->size) {
                editor.cx++;
            } else if (row && editor.cx == row->size) {
                editor.cy++;
                editor.cx = 0;
            }
            break;
        case ARROW_UP:
            if (editor.cy != 0) {
                editor.cy--;
            }
            break;
        case ARROW_DOWN:
            if (editor.cy < editor.numrows) {
                editor.cy++;
            }
            break;
    }

    // Snap cursor to end of line if it's beyond the current line
    row = (editor.cy >= editor.numrows) ? NULL : &editor.row[editor.cy];
    int rowlen = row ? row->size : 0;
    if (editor.cx > rowlen) {
        editor.cx = rowlen;
    }
}

void editor_process_keypress(void) {
    int c = editor_read_key();
    
    // Handle special case for exit first
    if (c == CTRL_KEY('c')) {
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0);
    }

    // Find and execute the command
    const EventListener *command = NULL;
    
    // First try to find an exact match
    for (size_t i = 0; i < num_event_listeners; i++) {
        if (event_listeners[i].key == c) {
            command = &event_listeners[i];
            break;
        }
    }
    
    // If no exact match found, use the default handler
    if (command == NULL) {
        command = &event_listeners[num_event_listeners - 1];  // Last command is the default handler
    }

    InputContext ctx = { .key = c, .cli_args = NULL };

    command->execute(&ctx);
    
    if (command->events) {
        for (int i = 0; command->events[i] != EVENT_NULL; i++) {
            editor_notify(command->events[i]);
        }
    }
}
