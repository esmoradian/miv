#include "commands.h"
#include <stdio.h>
#include "../editor/editor.h"
#include "../input/events.h"

void cmd_write(InputContext *ctx) {
  (void)ctx;
  char *fileName = editor.filename;

  FILE* file = fopen(editor.filename, "w");
  if (!file) return;
  
  for (int i = 0; i < editor.numrows; i++) {
      if (fputs(editor.row[i].chars, file) == EOF) {
          fclose(file);
          return;
      }
      if (i < editor.numrows - 1) {
          if (fputc('\n', file) == EOF) {
              fclose(file);
              return;
          }
      }
  } 

  fclose(file);
}
void cmd_quit(InputContext *ctx) { printf("[quit] Quit editor\n"); }
void cmd_open(InputContext *ctx) { printf("[open] Open file\n"); }

// some duplication here, consider alias
Command builtins[] = {
  {"write", "Save the file", cmd_write},
  {"w", "Save to file", cmd_write},
  {"quit", "Quit the editor", cmd_quit},
  {"q", "Quit the editor", cmd_quit},
  {"open", "Open a file", cmd_open},
  {"o", "Open a file", cmd_open}
};

size_t num_builtins = sizeof(builtins) / sizeof(builtins[0]);