#include <stdio.h>
#include <ctype.h>
#include "command.h"
#include "../buffer/row.h"
#include "../input/keyboard.h"
#include "../editor/editor.h"

void handle_arrow_key(int key) {
    editor_move_cursor(key);
}

void handle_delete(int key) {
    (void)key;
    editor_delete_char();
}

void handle_backspace(int key) {
    (void)key;
    editor_backspace_char();
}

void handle_newline(int key) {
    (void)key;
    editor_insert_newline();
}

void handle_char_insert(int key) {
    if (!iscntrl(key)) {
        editor_insert_char(key);
    }
}

void editor_save(int key) {
    (void) key;
    FILE* file = fopen(Editor.filename, "w");
    if (!file) return;
    
    for (int i = 0; i < Editor.numrows; i++) {
        if (fputs(Editor.row[i].chars, file) == EOF) {
            fclose(file);
            return;
        }
        if (i < Editor.numrows - 1) {
            if (fputc('\n', file) == EOF) {
                fclose(file);
                return;
            }
        }
    } 

    fclose(file);
}

const struct editor_command EDITOR_COMMANDS[] = {
    {ARROW_UP, handle_arrow_key, EVENT_CURSOR_MOVE, "move_up"},
    {ARROW_DOWN, handle_arrow_key, EVENT_CURSOR_MOVE, "move_down"},
    {ARROW_LEFT, handle_arrow_key, EVENT_CURSOR_MOVE, "move_left"},
    {ARROW_RIGHT, handle_arrow_key, EVENT_CURSOR_MOVE, "move_right"},
    {DEL_KEY, handle_delete, EVENT_DELETE, "delete"},
    {BACKSPACE, handle_backspace, EVENT_DELETE, "backspace"},
    {CTRL_KEY('h'), handle_backspace, EVENT_DELETE, "backspace_ctrl_h"},
    {127, handle_backspace, EVENT_DELETE, "backspace_127"},
    {'\r', handle_newline, EVENT_NEWLINE, "newline"},
    {CTRL_KEY('s'), editor_save, EVENT_SAVE, "save"},
    {0, handle_char_insert, EVENT_INSERT, "char_insert"}  // Special case for default handler
};

const size_t NUM_EDITOR_COMMANDS = sizeof(EDITOR_COMMANDS) / sizeof(EDITOR_COMMANDS[0]); 

