#ifndef ROW_H
#define ROW_H

#include <stddef.h>
#include "../editor/editor.h"

void editor_row_insert_char(editor_row *row, int at, int c);
void editor_row_delete_char(editor_row *row, int at);
void editor_insert_row(int at, char *s, size_t len);
void editor_insert_char(int c);
void editor_insert_newline(void);
void editor_delete_char(void);
void editor_backspace_char(void);

#endif

