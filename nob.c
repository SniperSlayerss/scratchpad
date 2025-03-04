#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "/usr/include/nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"

#define PKGS    "sdl3", "sdl3-ttf"
#define CFLAGS  "-Wall", "-O2", "-I/usr/include/SDL3", "-I/usr/include/SDL3_ttf"
#define LDFLAGS "-L", "/usr/include/lib", "-lSDL3", "-lSDL3_ttf"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd cmd = {0};
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    nob_cmd_append(&cmd, "clang", CFLAGS, "-o", BUILD_FOLDER"scratchpad", SRC_FOLDER"main.c", LDFLAGS);
    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;

    return 0;
}
