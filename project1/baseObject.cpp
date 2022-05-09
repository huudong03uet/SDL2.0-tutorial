#include "baseObject.h"

BaseObject::BaseObject()
{
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.h = 0;
    rect_.w = 0;
}

BaseObject::~BaseObject()
{
    Free();
}

bool BaseObject::loadImg(std::string path, SDL_Renderer* screen)
{
    Free();
    SDL_Texture* new_texture = NULL;
    SDL_Surface* load_surface = IMG_Load(path.c_str()); // load ảnh từ file dẫn đến surface
    if(load_surface != NULL)
    {
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        // set màu nền cho load_surface, 3 màu rgb đã set sẵn trong commonFunction
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
        // tạo ảnh từ surface (kéo lên màn hính)
        if(new_texture != NULL)
        {
            rect_.w = load_surface->w;
            rect_.h = load_surface->h;
            // toàn bộ thông tin hình ảnh đã nằm trong new_texture
        }

        SDL_FreeSurface(load_surface);
        // không cần nữa vì toàn bộ đã chuyển sang new_texture;
    }
    p_object_ = new_texture;
    return p_object_ != NULL;
}

void BaseObject::render(SDL_Renderer* des, const SDL_Rect* clip)
{
    SDL_Rect renderquad = {rect_.x, rect_.y, rect_.w, rect_.h};

    SDL_RenderCopy(des, p_object_, clip, &renderquad);
    // đẩy toàn bộ thông số p_object lên des với kích thước và vị trí nằm trong renderquad
}



void BaseObject::Free()
{
    if(p_object_ != NULL)
    {
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        rect_.w = 0; rect_.h = 0;
    }
}
