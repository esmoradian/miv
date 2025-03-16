#ifndef _SCREEN_H
#define _SCREEN_H

#include "../buffer/abuf.h"

void editor_draw_rows(struct abuf *ab);
void editor_refresh_screen(void);

#endif

