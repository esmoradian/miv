#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "terminal.h"
#include "../editor/editor.h"
#include "../input/error.h"

void disable_raw_mode(void) {
    if (!isatty(STDIN_FILENO)) {
        return;
    }
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &editor.orig_termios) == -1) {
        die("tcsetattr");
    }
}

void enable_raw_mode(void) {
    if (!isatty(STDIN_FILENO)) {
        return;
    }

    if (tcgetattr(STDIN_FILENO, &editor.orig_termios) == -1) {
        die("tcgetattr");
    }

    atexit(disable_raw_mode);

    struct termios raw = editor.orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        die("tcsetattr");
    }
}

int get_window_size(int *rows, int *cols) {
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
} 
