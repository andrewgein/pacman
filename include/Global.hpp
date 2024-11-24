#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <SDL2/SDL.h>

static const int GAME_HEIGHT = 36;
static const int GAME_WIDTH = 28;
static const int BOARD_WIDTH = 28;
static const int BOARD_HEIGHT = 31;
static const int WINDOW_HEIGHT = GAME_HEIGHT * 24;
static const int WINDOW_WIDTH = GAME_WIDTH * 24;
static const int BLOCK_SIZE = 24;

static const char SPRITE_SHEET_FILE[] = "resources/images/Map.png";
static const char FONT_FILE[] = "resources/fonts/Emulogic.ttf";
static const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};
static const SDL_Color RED = {.r = 255, .g = 0, .b = 0, .a = 255};
static const SDL_Color PINK = {.r = 252, .g = 181, .b = 255, .a = 255};
static const SDL_Color BLUE = {.r = 3, .g = 255, .b = 255, .a = 255};
static const SDL_Color YELLOW = {.r = 248, .g = 186, .b = 86, .a = 255};

enum Direction { Right = 0, Left = 1, Up = 2, Down = 3, None = -1 };

enum CollisionFlags { CanUseDoor = 2, CanUseTeleport = 1, Nothing = 0 };

static const int POINTER_DISTANCE = 5;

static const int MAP_CLIP_X_POSITION = 0;
static const int MAP_CLIP_Y_POSITION = 0;
static const int MAP_CLIP_WIDTH = 672;
static const int MAP_CLIP_HEIGHT = 744;
static const int MAP_WIDTH = 672;
static const int MAP_HEIGHT = 744;
static const int OFFSET_X = -12;
static const int OFFSET_Y = -12;
static const int MAP_OFFSET_X = 0;
static const int MAP_OFFSET_Y = 3 * 24;
static const SDL_Rect MAP_CLIP = {MAP_CLIP_X_POSITION, MAP_CLIP_Y_POSITION,
                                  MAP_CLIP_WIDTH, MAP_CLIP_HEIGHT};

static const int ANIMATION_TICKS = 100;
static const int GHOST_UPDATE_TICKS = 17;
static const int PLAYER_UPDATE_TICKS = 0;

static const short BLINKY_TEXTURE_OFFSET_X = 674;
static const short BLINKY_TEXTURE_OFFSET_Y = 178;
static const short PINKY_TEXTURE_OFFSET_X = 674;
static const short PINKY_TEXTURE_OFFSET_Y = 221;
static const short INKY_TEXTURE_OFFSET_X = 674;
static const short INKY_TEXTURE_OFFSET_Y = 264;
static const short CLYDE_TEXTURE_OFFSET_X = 674;
static const short CLYDE_TEXTURE_OFFSET_Y = 307;

static const char startMapSchematic[GAME_HEIGHT][GAME_WIDTH + 1] = {
    "############################", "#............##............#",
    "#.####.#####.##.#####.####.#", "#0####.#####.##.#####.####0#",
    "#.####.#####.##.#####.####.#", "#..........................#",
    "#.####.##.########.##.####.#", "#.####.##.########.##.####.#",
    "#......##....##....##......#", "######.##### ## #####.######",
    "######.##### ## #####.######", "######.##          ##.######",
    "######.## ######## ##.######", "######.## #      # ##.######",
    "      .   #      #   .      ", "######.## #      # ##.######",
    "######.## ######## ##.######", "######.##          ##.######",
    "######.## ######## ##.######", "######.## ######## ##.######",
    "#............##............#", "#.####.#####.##.#####.####.#",
    "#.####.#####.##.#####.####.#", "#0..##.......  .......##..0#",
    "###.##.##.########.##.##.###", "###.##.##.########.##.##.###",
    "#......##....##....##......#", "#.##########.##.##########.#",
    "#.##########.##.##########.#", "#..........................#",
    "############################"};

static const short MAP_TELEPORT_Y = 14;
static const short MAP_TELEPORT_X1 = 0;
static const short MAP_TELEPORT_X2 = (BOARD_WIDTH - 1);

static const short TELEPORT_Y = 14;
static const short TELEPORT_X1 = 0;
static const short TELEPORT_X2 = BOARD_WIDTH - 1;

#endif
