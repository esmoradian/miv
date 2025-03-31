#include "editor/editor.h"
#include "terminal/terminal.h"
#include "output/screen.h"
#include "input/keyboard.h"
#include "commands/command_registry.h"

struct Editor editor;

int main(int argc, char **argv) {
    enable_raw_mode();
    init_editor();

    if (argc >= 2) {
        editor_open_file(argv[1]);
    }

    editor_refresh_screen();
    init_command_registry();

    while (1) {
        editor_process_keypress();
    }

    return 0;
}
