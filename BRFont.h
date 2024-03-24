#ifndef BRFONT_HPP
#define BRFONT_HPP

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <iostream>

class Font_BR{
    public:
        void Font_BR_Init(SDL_Renderer* Frenderer,int fontSize,char* text,int x,int y,int w,int h, SDL_Color color);
        void Font_Br_Draw(SDL_Renderer* renderer);
        SDL_Texture* getTexture();
        SDL_Rect getRect();
        int Font_BR_getxVal();
        int Font_BR_getyVal();
        int Font_BR_getwVal();
        int Font_BR_gethVal();
        ~Font_BR();

    private:
        TTF_Font* m_Font;
        SDL_Surface* m_Surface;
        SDL_Texture* m_Texture;
        SDL_Rect m_FontRect;
}; 

#endif