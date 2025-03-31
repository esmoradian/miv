#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>
#include <limits.h>
#include "editor.h"
#include "../buffer/row.h"
#include "../observer/observer.h"
#include "../terminal/terminal.h"
#include "../output/screen.h"
#include "../input/error.h"


static void init_editor_state(void) {
    if (!editor.filename) {
        editor.filename = malloc(256);
        if (!editor.filename) die("malloc");

        time_t now = time(NULL);
        struct tm* tm = localtime(&now);
        snprintf(editor.filename, 256, 
                "untitled_%04d%02d%02d.txt",
                tm->tm_year + 1900,
                tm->tm_mon + 1,
                tm->tm_mday);
    }
    
    editor.cx = 0;
    editor.cy = 0;
    editor.numrows = 0;
    editor.row = NULL;
    editor.num_observers = 0;
}

static void init_event_handlers(void) {
    editor_register_callback(EVENT_CURSOR_MOVE, editor_refresh_screen);
    editor_register_callback(EVENT_INSERT, editor_refresh_screen);
    editor_register_callback(EVENT_DELETE, editor_refresh_screen);
    editor_register_callback(EVENT_WINDOW_RESIZE, editor_refresh_screen);
    editor_register_callback(EVENT_FILE_OPEN, editor_refresh_screen);
    editor_register_callback(EVENT_NEWLINE, editor_refresh_screen);
}

static void init_terminal_size(void) {
    if (get_window_size(&editor.screenrows, &editor.screencols) == -1) {
        die("getWindowSize");
    }
}

void init_editor(void) {
    init_editor_state();
    init_event_handlers();
    init_terminal_size();
} 

void editor_load_file(FILE* fp) {
    char* line = NULL;
    size_t cap = 0;
    ssize_t len;

    while ((len = getline(&line, &cap, fp)) != -1) {
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            len--;
        }
        editor_insert_row(editor.numrows, line, len);
    }

    free(line);
}

void editor_open_file(const char* rel_path) {
    char abs_path[PATH_MAX];

    if (!realpath(rel_path, abs_path)) {
        if (rel_path[0] != '/') {
            getcwd(abs_path, sizeof(abs_path));
            strncat(abs_path, "/", sizeof(abs_path) - strlen(abs_path) - 1);
            strncat(abs_path, rel_path, sizeof(abs_path) - strlen(abs_path) - 1);
        } else {
            strncpy(abs_path, rel_path, sizeof(abs_path) - 1);
            abs_path[sizeof(abs_path) - 1] = '\0';
        }
    }

    char path_copy[PATH_MAX];
    strncpy(path_copy, abs_path, sizeof(path_copy));
    path_copy[sizeof(path_copy) - 1] = '\0';
    char* dir = dirname(path_copy);
    struct stat sb;
    if (stat(dir, &sb) != 0 || !S_ISDIR(sb.st_mode)) {
        // fprintf(stderr, "Error: Directory does not exist or is invalid: %s\n", dir);
        // TODO: Error handling after data display
        exit(1);
    }

    FILE* fp = fopen(abs_path, "r+");
    if (!fp) {
        fp = fopen(abs_path, "w+");
        if (!fp) {
            // fprintf(stderr, "Error: Cannot create file: %s\n", abs_path);
            // TODO: Error handling after data display
            exit(1);
        }
    }

    editor.filename = malloc(strlen(abs_path) + 1);
    strcpy(editor.filename, abs_path);

    editor_load_file(fp);

    fclose(fp);
}
