#include <ctype.h>
#include "command.h"
#include "../buffer/row.h"
#include "../input/keyboard.h"
#include "../init/init.h"

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

const struct editor_command EDITOR_COMMANDS[] = {
    {ARROW_UP, handle_arrow_key, true, "move_up"},
    {ARROW_DOWN, handle_arrow_key, true, "move_down"},
    {ARROW_LEFT, handle_arrow_key, true, "move_left"},
    {ARROW_RIGHT, handle_arrow_key, true, "move_right"},
    {DEL_KEY, handle_delete, true, "delete"},
    {BACKSPACE, handle_backspace, true, "backspace"},
    {CTRL_KEY('h'), handle_backspace, true, "backspace_ctrl_h"},
    {127, handle_backspace, true, "backspace_127"},
    {'\r', handle_newline, true, "newline"},
    {0, handle_char_insert, true, "char_insert"}  // Special case for default handler
};

const size_t NUM_EDITOR_COMMANDS = sizeof(EDITOR_COMMANDS) / sizeof(EDITOR_COMMANDS[0]); 

