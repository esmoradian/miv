#include <stdlib.h>
#include <string.h>
#include "row.h"

void editor_row_insert_char(editor_row *row, int at, int c) {
    if (at < 0 || at > row->size) at = row->size;
    row->chars = realloc(row->chars, row->size + 2);
    memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
    row->size++;
    row->chars[at] = c;
}

void editor_row_delete_char(editor_row *row, int at) {
    if (at < 0 || at >= row->size) return;
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
}

void editor_insert_row(int at, char *s, size_t len) {
    if (at < 0 || at > Editor.numrows) return;

    Editor.row = realloc(Editor.row, sizeof(editor_row) * (Editor.numrows + 1));
    memmove(&Editor.row[at + 1], &Editor.row[at], sizeof(editor_row) * (Editor.numrows - at));

    Editor.row[at].size = len;
    Editor.row[at].chars = malloc(len + 1);
    memcpy(Editor.row[at].chars, s, len);
    Editor.row[at].chars[len] = '\0';
    Editor.numrows++;
}

void editor_insert_char(int c) {
    if (Editor.cy == Editor.numrows) {
        editor_insert_row(Editor.numrows, "", 0);
    } 

    editor_row_insert_char(&Editor.row[Editor.cy], Editor.cx, c);
    Editor.cx++;
}

void editor_insert_newline(void) {
    if (Editor.cx == 0) {
        editor_insert_row(Editor.cy, "", 0);
    } else {
        editor_row *row = &Editor.row[Editor.cy];
        editor_insert_row(Editor.cy + 1, &row->chars[Editor.cx], row->size - Editor.cx);
        row = &Editor.row[Editor.cy];
        row->size = Editor.cx;
        row->chars[row->size] = '\0';
    }
    Editor.cy++;
    Editor.cx = 0;
}

void editor_delete_char(void) {
    if (Editor.cy == Editor.numrows) return;
    if (Editor.cx == Editor.row[Editor.cy].size && Editor.cy == Editor.numrows - 1) return;

    editor_row *row = &Editor.row[Editor.cy];
    if (Editor.cx < row->size) {
        editor_row_delete_char(row, Editor.cx);
    } else {
        // At end of line, join with next line
        Editor.cx = row->size;
        editor_row *next_row = &Editor.row[Editor.cy + 1];
        row->chars = realloc(row->chars, row->size + next_row->size + 1);
        memcpy(&row->chars[row->size], next_row->chars, next_row->size);
        row->size += next_row->size;
        row->chars[row->size] = '\0';
        
        // Remove the next line
        memmove(&Editor.row[Editor.cy + 1], &Editor.row[Editor.cy + 2], sizeof(editor_row) * (Editor.numrows - Editor.cy - 2));
        Editor.numrows--;
    }
}

void editor_backspace_char(void) {
    if (Editor.cx == 0 && Editor.cy == 0) return;

    if (Editor.cx > 0) {
        Editor.cx--;
        editor_delete_char();
    } else {
        // At start of line, join with previous line
        Editor.cy--;
        Editor.cx = Editor.row[Editor.cy].size;
        editor_delete_char();
    }
} 
