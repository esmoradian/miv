#include "editor/editor.h"
#include "terminal/terminal.h"
#include "output/screen.h"
#include "input/keyboard.h"

struct editor Editor;

int main(void) {
    enable_raw_mode();
    init_editor();
    editor_refresh_screen();

    while (1) {
        editor_process_keypress();
    }

    return 0;
} 
