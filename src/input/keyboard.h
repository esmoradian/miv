#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "events.h"

int editor_read_key(void);
void editor_process_keypress(void);
void editor_move_cursor(InputContext *ctx);
void handle_arrow_key(InputContext *ctx);
void handle_delete(InputContext *ctx);
void handle_backspace(InputContext *ctx);
void handle_newline(InputContext *ctx);
void handle_char_insert(InputContext *ctx);

#endif

