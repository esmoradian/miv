#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "screen.h"
#include "../editor/editor.h"

void editor_draw_rows(struct abuf *ab) {
    int y;
    for (y = 0; y < editor.screenrows; y++) {
        if (y >= editor.numrows) {
            if (editor.numrows == 0 && y == editor.screenrows / 3) {
                char welcome[80];
                int welcomelen = snprintf(welcome, sizeof(welcome),
                    "MIV editor -- version %s", MIV_VERSION);
                if (welcomelen > editor.screencols) welcomelen = editor.screencols;
                int padding = (editor.screencols - welcomelen) / 2;
                if (padding) {
                    abuf_append(ab, "~", 1);
                    padding--;
                }
                while (padding--) abuf_append(ab, " ", 1);
                abuf_append(ab, welcome, welcomelen);
            } else {
                abuf_append(ab, "~", 1);
            }
        } else {
            int len = editor.row[y].size;
            if (len > editor.screencols) len = editor.screencols;
            abuf_append(ab, editor.row[y].chars, len);
        }

        abuf_append(ab, "\x1b[K", 3);
        if (y < editor.screenrows - 1) {
            abuf_append(ab, "\r\n", 2);
        }
    }
}

void editor_refresh_screen(void) {
    struct abuf ab = ABUF_INIT;

    abuf_append(&ab, "\x1b[?25l", 6);  // Hide cursor
    abuf_append(&ab, "\x1b[H", 3);     // Move cursor to top-left

    editor_draw_rows(&ab);

    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", editor.cy + 1, editor.cx + 1);
    abuf_append(&ab, buf, strlen(buf));

    abuf_append(&ab, "\x1b[?25h", 6);  // Show cursor

    write(STDOUT_FILENO, ab.b, ab.len);
    abuf_free(&ab);
}
