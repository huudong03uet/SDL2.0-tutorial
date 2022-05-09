#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_
#include "commonFunction.h"

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    void setRect(const int& x, const int& y)
    {
        rect_.x = x; rect_.y = y;
    }
    SDL_Rect getRect() const
    {
        return rect_;
    }
    SDL_Texture* getObject() const
    {
        return p_object_;
    }
    virtual bool loadImg(std::string path, SDL_Renderer* screen);
    // load anh, duong dan, man hinh
    void render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
    void Free();


protected:
    SDL_Texture* p_object_; // lưu trữ hình ảnh
    SDL_Rect rect_;  //kích thước tấm ảnh đó
};
#endif // BASE_OBJECT_H_
