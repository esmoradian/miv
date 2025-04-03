#include "editor/editor.h"
#include "terminal/terminal.h"
#include "output/screen.h"
#include "input/keyboard.h"
#include "commands/command_registry.h"
#include "core/plugin_manager.h"

struct Editor editor;

int main(int argc, char **argv) {
    enable_raw_mode();
    init_editor();

    if (argc >= 2) {
        editor_open_file(argv[1]);
    }

    editor_refresh_screen();
    init_command_registry();

    // TODO: Move into miv init
    PluginManager *plugin_manager = plugin_manager_create();
    Plugin *plugin = plugin_spawn("plugins/test.py");
    plugin_manager_register(plugin_manager, plugin);

    while (1) {
        // TODO: Move into miv start
        plugin_manager_poll(plugin_manager);
        editor_process_keypress();
    }

    return 0;
}
