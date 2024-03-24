#include"include/headers/Door.h"
void Door::initDoorObj(SDL_Renderer* renderer, char* loc, int rX, int rY, int rW, int rH)
{
    doorImage.Image_BR_Init(renderer,loc,rX,rY,rW,rH);
}
void Door::renderDoorObj(SDL_Renderer* renderer)
{
    doorImage.Image_BR_Draw(renderer);
}

int Door::getDoorObjXpos()
{
    return doorImage.Image_BR_getxVal();
}
int Door::getDoorObjYpos()
{
    return doorImage.Image_BR_getyVal();
}
int Door::getDoorObjW()
{
    return doorImage.Image_BR_getwVal();
}
int Door::getDoorObjH()
{
    return doorImage.Image_BR_gethVal();
}

void Door::setDoorXpos(int x)
{
    doorImage.Image_BR_setxValstatic(x);
}
void Door::setDoorYpos(int y)
{
    doorImage.Image_BR_setyValstatic(y);
}
void Door::setDoorW(int w)
{
    doorImage.Image_BR_setwVal(w);
}
void Door::setDoorH(int h)
{
    doorImage.Image_BR_sethVal(h);
}