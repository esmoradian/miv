#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "keyboard.h"
#include "command.h"
#include "error.h"
#include "../editor/editor.h"
#include "../events/observer.h"

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

void editor_move_cursor(int key) {
    editor_row *row = (Editor.cy >= Editor.numrows) ? NULL : &Editor.row[Editor.cy];

    switch (key) {
        case ARROW_LEFT:
            if (Editor.cx != 0) {
                Editor.cx--;
            } else if (Editor.cy > 0) {
                Editor.cy--;
                Editor.cx = Editor.row[Editor.cy].size;
            }
            break;
        case ARROW_RIGHT:
            if (row && Editor.cx < row->size) {
                Editor.cx++;
            } else if (row && Editor.cx == row->size) {
                Editor.cy++;
                Editor.cx = 0;
            }
            break;
        case ARROW_UP:
            if (Editor.cy != 0) {
                Editor.cy--;
            }
            break;
        case ARROW_DOWN:
            if (Editor.cy < Editor.numrows) {
                Editor.cy++;
            }
            break;
    }

    // Snap cursor to end of line if it's beyond the current line
    row = (Editor.cy >= Editor.numrows) ? NULL : &Editor.row[Editor.cy];
    int rowlen = row ? row->size : 0;
    if (Editor.cx > rowlen) {
        Editor.cx = rowlen;
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
    const struct editor_command *cmd = NULL;
    
    // First try to find an exact match
    for (size_t i = 0; i < NUM_EDITOR_COMMANDS; i++) {
        if (EDITOR_COMMANDS[i].key == c) {
            cmd = &EDITOR_COMMANDS[i];
            break;
        }
    }
    
    // If no exact match found, use the default handler
    if (cmd == NULL) {
        cmd = &EDITOR_COMMANDS[NUM_EDITOR_COMMANDS - 1];  // Last command is the default handler
    }

    cmd->execute(c);
    
    if (cmd->event != EVENT_NULL) {
        editor_notify(cmd->event);
    }
} 

