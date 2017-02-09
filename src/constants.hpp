#ifndef DEF_CONSTANTS
#define DEF_CONSTANTS

#define DEV_MODE  // delete this line before sending in production

// views id
#define UNREACHABLE_VIEW_ID -1
#define LAST_VIEW_ID 0
#define DEFAULT_VIEW_ID 1
#define INVENTORY_VIEW_ID 2
#define MAP_VIEW_ID 3
#define MYCREATURES_VIEW_ID 4
#define DEX_VIEW_ID 5
#define SAVING_VIEW_ID 6
#define FIGHT_VIEW_ID 7

// HUD
#define MENU_VIEW_ID 2

// specials define
#define DISPLAY_FPS_IN_TITLE 1

// tiles
#define TILE_SIZE 16
#define TILE_SIZE_IN_TILESET 17

// window
#define WIN_W 640
#define WIN_H 640

#define FRAMERATE_LIM 120

#define MINIMAP_X 128
#define MINIMAP_Y 128

#define COLLIDING_LAYER 1

// platform (useful for some functions)
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    #define PLATFORM_WIN
#endif
#if defined(__unix__) || defined(__linux__) || defined(__APPLE__) && defined(__MACH__)
    // linux, unix and apple systems should be recognized this way
    #define PLATFORM_POSIX
#endif

// directions
enum class DIRECTION {
    up,
    down,
    left,
    right
};

enum class MvState {
    idle,
    walking,
    idle2,
    walking2
};

enum class ChState {
    idle,
    walking,
    running
};

#endif // DEF_CONSTANTS
