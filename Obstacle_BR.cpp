#include"include/headers/Obstacle_BR.h"
void Obstacle_BR::initObstacleObj(SDL_Renderer* renderer, char* loc, int rX, int rY, int rW, int rH)
{
    obstacleImage.Image_BR_Init(renderer,loc,rX,rY,rW,rH);
}
void Obstacle_BR::renderObstacleObj(SDL_Renderer* renderer)
{
    obstacleImage.Image_BR_Draw(renderer);
}
void Obstacle_BR::isColliding(Player_BR getPlayer, Collision_BR collisionCheck)
{
    if(collisionCheck.checkObjCollision(getPlayer.getPlayerObjXpos(),getPlayer.getPlayerObjYpos(),getPlayer.getPlayerObjW(),getPlayer.getPlayerObjH(),
    obstacleImage.Image_BR_getxVal(),obstacleImage.Image_BR_getyVal(),obstacleImage.Image_BR_getwVal(),obstacleImage.Image_BR_gethVal()) == true)
    {
        //return 1;
        std::cout<<"In contact"<<std::endl;
    }    
   // else
        //return 0;
}

int Obstacle_BR::getObstacleObjXpos()
{
    return obstacleImage.Image_BR_getxVal();
}
int Obstacle_BR::getObstacleObjYpos()
{
    return obstacleImage.Image_BR_getyVal();
}
int Obstacle_BR::getObstacleObjW()
{
    return obstacleImage.Image_BR_getwVal();
}
int Obstacle_BR::getObstacleObjH()
{
    return obstacleImage.Image_BR_gethVal();
}

void Obstacle_BR::setObstacleObjXpos(int x)
{
    obstacleImage.Image_BR_setxValstatic(x); 
}
void Obstacle_BR::setObstacleObjYpos(int y)
{
    obstacleImage.Image_BR_setyValstatic(y); 
}
void Obstacle_BR::setObstacleObjW(int w)
{
    obstacleImage.Image_BR_setwVal(w); 
}
void Obstacle_BR::setObstacleObjH(int h)
{
    obstacleImage.Image_BR_sethVal(h); 
}