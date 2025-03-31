#include <stdio.h>
#include <ctype.h>
#include "events.h"
#include "keyboard.h"
#include "../buffer/row.h"
#include "../commands/commands.h"

static enum EDITOR_EVENT cursor_move_events[] = {
    EVENT_CURSOR_MOVE,
    EVENT_NULL
};

static enum EDITOR_EVENT keypress_delete_events[] = {
    EVENT_DELETE,
    EVENT_NULL
};

static enum EDITOR_EVENT keypress_newline_events[] = {
    EVENT_NEWLINE,
    EVENT_NULL
};

static enum EDITOR_EVENT keypress_save_events[] = {
    EVENT_SAVE,
    EVENT_NULL
};

static enum EDITOR_EVENT insert_events[] = {
    EVENT_INSERT,
    EVENT_NULL
};

const EventListener event_listeners[] = {
    {ARROW_UP, handle_arrow_key, cursor_move_events, "move_up"},
    {ARROW_DOWN, handle_arrow_key, cursor_move_events, "move_down"},
    {ARROW_LEFT, handle_arrow_key, cursor_move_events, "move_left"},
    {ARROW_RIGHT, handle_arrow_key, cursor_move_events, "move_right"},
    {DEL_KEY, handle_delete, keypress_delete_events, "delete"},
    {BACKSPACE, handle_backspace, keypress_delete_events, "backspace"},
    {CTRL_KEY('h'), handle_backspace, keypress_delete_events, "backspace_ctrl_h"},
    {127, handle_backspace, keypress_delete_events, "backspace_127"},
    {'\r', handle_newline, keypress_newline_events, "newline"},
    {CTRL_KEY('s'), cmd_write, keypress_save_events, "save"},
    {0, handle_char_insert, insert_events, "char_insert"}  // Special case for default handler
};

const size_t num_event_listeners = sizeof(event_listeners) / sizeof(event_listeners[0]); 

