
#ifndef HINH_VE_H_
#define HINH_VE_H_

#include "commonFunction.h"

typedef struct HinhVeFormat
{
public:
    HinhVeFormat(int left, int top, int width, int height)
    {
        left_ = left; top_ = top;
        width_ = width; height_ = height;
    }
    int left_;
    int top_;
    int width_;
    int height_;
};

typedef struct ColorData
{

    ColorData(Uint8 r, Uint8 g, Uint8 b){red_ = r; green_ = g; blue_ = b;}

public:
    Uint8 red_;
    Uint8 green_;
    Uint8 blue_;
};

class HinhVe
{
public:
    static void RenderRectangle(const HinhVeFormat& geo_size, const ColorData& color_data, SDL_Renderer* screen);
    static void RenderOutline(const HinhVeFormat& geo_size, const ColorData& color_data, SDL_Renderer* screen);
private:

};
#endif // HINH_VE_H_
