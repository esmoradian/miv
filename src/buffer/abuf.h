#ifndef ABUF_H
#define ABUF_H

struct abuf {
    char *b;
    int len;
};

#define ABUF_INIT {NULL, 0}

void abuf_append(struct abuf *ab, const char *s, int len);
void abuf_free(struct abuf *ab);

#endif

