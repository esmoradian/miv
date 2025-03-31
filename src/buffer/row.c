#include <stdlib.h>
#include <string.h>
#include "row.h"
#include "../input/events.h"

void editor_row_insert_char(EditorRow *row, int at, int c) {
    if (at < 0 || at > row->size) at = row->size;
    row->chars = realloc(row->chars, row->size + 2);
    memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
    row->size++;
    row->chars[at] = c;
}

void editor_row_delete_char(EditorRow *row, int at) {
    if (at < 0 || at >= row->size) return;
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
}

void editor_insert_row(int at, char *s, size_t len) {
    if (at < 0 || at > editor.numrows) return;

    editor.row = realloc(editor.row, sizeof(EditorRow) * (editor.numrows + 1));
    memmove(&editor.row[at + 1], &editor.row[at], sizeof(EditorRow) * (editor.numrows - at));

    editor.row[at].size = len;
    editor.row[at].chars = malloc(len + 1);
    memcpy(editor.row[at].chars, s, len);
    editor.row[at].chars[len] = '\0';
    editor.numrows++;
}

void editor_insert_char(InputContext *ctx) {
    int c = ctx->key;
    if (editor.cy == editor.numrows) {
        editor_insert_row(editor.numrows, "", 0);
    } 

    editor_row_insert_char(&editor.row[editor.cy], editor.cx, c);
    editor.cx++;
}

void editor_insert_newline(void) {
    if (editor.cx == 0) {
        editor_insert_row(editor.cy, "", 0);
    } else {
        EditorRow *row = &editor.row[editor.cy];
        editor_insert_row(editor.cy + 1, &row->chars[editor.cx], row->size - editor.cx);
        row = &editor.row[editor.cy];
        row->size = editor.cx;
        row->chars[row->size] = '\0';
    }
    editor.cy++;
    editor.cx = 0;
}

void editor_delete_char(void) {
    if (editor.cy == editor.numrows) return;
    if (editor.cx == editor.row[editor.cy].size && editor.cy == editor.numrows - 1) return;

    EditorRow *row = &editor.row[editor.cy];
    if (editor.cx < row->size) {
        editor_row_delete_char(row, editor.cx);
    } else {
        // At end of line, join with next line
        editor.cx = row->size;
        EditorRow *next_row = &editor.row[editor.cy + 1];
        row->chars = realloc(row->chars, row->size + next_row->size + 1);
        memcpy(&row->chars[row->size], next_row->chars, next_row->size);
        row->size += next_row->size;
        row->chars[row->size] = '\0';
        
        // Remove the next line
        memmove(&editor.row[editor.cy + 1], &editor.row[editor.cy + 2], sizeof(EditorRow) * (editor.numrows - editor.cy - 2));
        editor.numrows--;
    }
}

void editor_backspace_char(void) {
    if (editor.cx == 0 && editor.cy == 0) return;

    if (editor.cx > 0) {
        editor.cx--;
        editor_delete_char();
    } else {
        // At start of line, join with previous line
        editor.cy--;
        editor.cx = editor.row[editor.cy].size;
        editor_delete_char();
    }
} 
