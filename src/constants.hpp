#ifndef DEF_CONSTANTS
#define DEF_CONSTANTS

// views id
#define UNREACHABLE_VIEW_ID -1
#define DEFAULT_VIEW_ID 1
#define INVENTORY_VIEW_ID 2
#define MAP_VIEW_ID 3
#define MYCREATURES_VIEW_ID 4
#define DEX_VIEW_ID 5
#define SAVING_VIEW_ID 6

// HUD
#define MENU_VIEW_ID 2

// specials define
#define DISPLAY_FPS_IN_TITLE 1

// tiles
#define TILE_SIZE 16
#define TILE_SIZE_IN_TILESET 17

// window
#define WIN_W 640
#define WIN_H 540

#define COLLIDING_LAYER 1

// platform (useful for some functions)
#define PLATFORM_WIN
// #define PLATFORM_POSIX

// directions
enum class DIR {
    up,
    down,
    left,
    right
};

#endif // DEF_CONSTANTS
