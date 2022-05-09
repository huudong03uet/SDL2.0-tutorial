#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <windows.h>
#include <iostream>
#include <string>
#include <SDL.h>        // nói chung
#include <SDL_image.h>   // hình ảnh
#include <SDL_mixer.h>   // âm thanh
#include <SDL_ttf.h>
#include <vector>

static SDL_Window* g_window = NULL; // font window chung
static SDL_Renderer* g_screen = NULL; // font chung màn hình
static SDL_Event g_event; // sự kiện bàn phím, chuột

// màn hình
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

// màu chung
const int FRAME_PER_SECOND = 25;
const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0xFF;

#define TILE_SIZE 64 // định nghĩa kích thước của mắt lưới

#define MAX_MAP_X 400
#define MAX_MAP_Y 10
#define BLANK_TILE 0
#define STATE_MONEY 4

typedef struct Input
{
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
};
typedef struct Map
{
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;
};

namespace SDLCommonFunc
{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
}
#endif // COMMON_FUNCTION_H_
