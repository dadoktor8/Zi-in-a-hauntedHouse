#include"include/headers/Enemy.h"
void Enemy::initEnemyObj(SDL_Renderer* renderer, char* loc, int rX, int rY, int rW, int rH)
{
    enemyImage.Image_BR_Init(renderer,loc,rX,rY,rW,rH);
}
void Enemy::renderEnemyObj(SDL_Renderer* renderer)
{
    enemyImage.Image_BR_Draw(renderer); 
}
int Enemy::getEnemyObjXpos()
{
    return enemyImage.Image_BR_getxVal();
}
int Enemy::getEnemyObjYpos()
{
    return enemyImage.Image_BR_getyVal();
}
int Enemy::getEnemyObjW()
{
    return enemyImage.Image_BR_getwVal();
}
int Enemy::getEnemyObjH()
{
    return enemyImage.Image_BR_gethVal();
}
void Enemy::setObstacleObjXpos(int x)
{
    enemyImage.Image_BR_setxValstatic(x); 
}
void Enemy::setObstacleObjYpos(int y)
{
    enemyImage.Image_BR_setyValstatic(y); 
}
void Enemy::setObstacleObjW(int w)
{
    enemyImage.Image_BR_setwVal(w);
}
void Enemy::setObstacleObjH(int h)
{
    enemyImage.Image_BR_sethVal(h);
}
void Enemy::preventMovement()
{
    canMove = false;
}
void Enemy::startMovement()
{
    canMove = true;
}
void Enemy::updateEnemyPos(float deltaTime,int playerPosX, int playerPosY)
{

    //Might configure around with deltaTime later!
    if(canMove == true)
    {
        double distX = playerPosX - enemyImage.Image_BR_getxVal();
        double distY = playerPosY - enemyImage.Image_BR_getyVal();

        double D = sqrt((distX * distX) + (distY * distY)); 
        double Nx = (distX / D); //Sin
        double Ny = (distY / D); //Cos

    //std::cout<<Nx<<" "<<Ny<<std::endl;
        enemyImage.Image_BR_setxVal(((Nx + (double) 0.2f) ) * 3);
        enemyImage.Image_BR_setyVal(((Ny + (double) 0.2f) ) * 3);
    } 

    //std::cout<<"Current enemy x pos is "<< enemyImage.Image_BR_getxVal() <<" the y pos is "<<enemyImage.Image_BR_getyVal()<<std::endl;

}