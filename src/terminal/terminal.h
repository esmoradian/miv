#ifndef TERMINAL_H
#define TERMINAL_H

void enable_raw_mode(void);
void disable_raw_mode(void);
int get_window_size(int *rows, int *cols);

#endif

