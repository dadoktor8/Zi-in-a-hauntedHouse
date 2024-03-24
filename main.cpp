// On linux compile with:
// g++ -std=c++17 main.cpp glad/src/glad.c -I./glad/include -o prog -lSDL2 -ldl
// On windows compile with (if using mingw)
// g++ main.cpp SDL_Init_BR.cpp BRFont.cpp Image_BR.cpp TileMap_BR.cpp Collision_BR.cpp DeltaTime_BR.cpp Player_BR.cpp Obstacle_BR.cpp Door.cpp ./glad/src/glad.c -I./glad/include -o prog.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image 
// C++ Standard Libraries
//all:
//g++ -Iinclude -Iinclude/SDL2 -Iinclude/headers -Llib *.cpp ./glad/src/glad.c -I./glad/include -o Zi.exe  -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image 
	//g++ -Iinclude -Iinclude/SDL2 -Iinclude/headers -Llib *.cpp ./glad/src/glad.c -I./glad/include -o Zi.exe  -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image 
#include <iostream>
#include <cmath>
#include <csignal>
#include <emscripten.h>
// Third-party library
#include <SDL2/SDL.h>
//User Defined Classes
#include "include/headers/SDL_Init_BR.h"
#include "include/headers/BRFont.h"
#include "include/headers/Image_BR.h"
#include "include/headers/TileMap_BR.h"
#include "include/headers/Collision_BR.h"
#include "include/headers/DeltaTime_BR.h"
#include "include/headers/Player_BR.h"
#include "include/headers/Obstacle_BR.h"
#include "include/headers/Enemy.h"
#include "include/headers/Door.h"
// Include GLAD
//#include <glad/glad.h>

//using namespace std; 

//Define the resolution in here
#define WIDTH 1280
#define HEIGHT 768


//volatile sig_atomic_t isRunning = false;
//Initialize all the neccesary variables
struct context
{
    SDL_Window* window=nullptr;
    SDL_Renderer* renderer = nullptr; 
    //SDL_GLContext context;
    SDL_Event event;
    bool gameIsRunning = true;

    //Important other variables
    const int FPS = 60;
    const int frameDelay = 1000/ FPS;
    Uint32 frameStart;
    int frameTime;

    //Important Game Initializations
    SDL_Color White = {255,255,255};
    SDL_Color Black = {0,0,0};
    Font_BR titleFont;
    Font_BR menuFont;
    Font_BR creditFont; 
    Font_BR endFont;
    Font_BR tutFont; 
    Font_BR retryFont;
    Font_BR levelFont;
    Font_BR scoreFont;
    Font_BR levelFont_One;
    Font_BR levelFont_Two;
    Font_BR levelFont_Three;
    Image_BR newImage;
    Player_BR playerOne; 
    Image_BR IR_blueWitch;
    TileMap_BR BG_Tile; 
    Collision_BR collision;
    DeltaTime_BR deltaTimeFunc;
    Door mainDoor;
    Obstacle_BR obs[5];
    Obstacle_BR obs_one;
    Enemy enemyOne;
    Enemy enemies[10];

    //GameState Variables
    bool menuState = true;
    bool menuStateTransition = false;
    bool gameplayState = false;
    bool gameplayStateTransition = false;
    bool gameplayState_first = false;
    bool gameplayState_firstTransition = false;
    bool gameplayState_second = false;
    bool gameplayState_secondTransition = false;
    bool gameoverState = false;
    bool gameoverStateTransition = false;
    int currentLevel = 0;

    //Game Rects
    bool enenmyMove = true;
    float moveSpeed = 700; //600 with deltaTIme
    int obsNumber = 5;
    int enenmyNumber = 3;
    int enemyRemaining = 1;
    int level_one_enemyRemaining = 3;
    int level_two_enemyRemaining = 2;
};
//Important Functions
void Init(context *ctx){

    ctx->BG_Tile.TileMap_BR_Init(ctx->renderer,"res/tile.png",64,64);
    ctx->newImage.Image_BR_Init(ctx->renderer,"res/bg.png",0,0,1280,768);

    ctx->titleFont.Font_BR_Init(ctx->renderer,24,"Zi in a Haunted House!",360,100,600,200,ctx->Black);
    ctx->menuFont.Font_BR_Init(ctx->renderer,24,"Start",550,300,200,100,ctx->Black);
    ctx->creditFont.Font_BR_Init(ctx->renderer,24,"Made By DocJenny!",450,500,400,100,ctx->Black);

    ctx->tutFont.Font_BR_Init(ctx->renderer,24,"Lure the apparitions into the red traps",350,600,600,100,ctx->Black);

    ctx->playerOne.initPlayerObjAnim(ctx->renderer,"res/zi.png",WIDTH/2,HEIGHT/2,64,64);
    ctx->levelFont_One.Font_BR_Init(ctx->renderer,16,"Level: 1",600,100,16*10,16 * 5,ctx->Black);
    ctx->levelFont_Two.Font_BR_Init(ctx->renderer,16,"Level: 2",600,100,16*10,16 * 5,ctx->Black);
    ctx->levelFont_Three.Font_BR_Init(ctx->renderer,16,"Level: 3",600,100,16*10,16 * 5,ctx->Black);


    //Level - 2
    for(int i = 0; i < 5; i++)
    {
       ctx->obs[i].initObstacleObj(ctx->renderer,"res/obstaclepencil.png",0, 0,64,64);
       if(i == 0)
       {
            ctx->obs[i].setObstacleObjXpos(100);
            ctx->obs[i].setObstacleObjYpos((200 + rand() % (200 - 100 + 1)));
       }
       if(i == 1)
       {
            ctx->obs[i].setObstacleObjXpos(400);
            ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
       }
       if(i == 2)
       {
            ctx->obs[i].setObstacleObjXpos(764);
            ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
       }
       if(i == 3)
       {
            ctx->obs[i].setObstacleObjXpos(1000);
            ctx->obs[i].setObstacleObjYpos((100 + rand() % (200 - 100 + 1)));
       }
       if(i == 4)
       {
            ctx->obs[i].setObstacleObjXpos(1000);
            ctx->obs[i].setObstacleObjYpos((500 + rand() % (600 - 500 + 1)));
       }
    }
    for(int i = 0; i < ctx->enenmyNumber; i++)
    {
        ctx->enemies[i].initEnemyObj(ctx->renderer,"res/enemy.png",0,0,64,64);
        if(i == 0)
        {
            ctx->enemies[i].setObstacleObjXpos(0);
            ctx->enemies[i].setObstacleObjYpos(0);
        }
        if(i == 1)
        {
            ctx->enemies[i].setObstacleObjXpos(0);
            ctx->enemies[i].setObstacleObjYpos(768);
        }
        if(i == 2)
        {
            ctx->enemies[i].setObstacleObjXpos(1280);
            ctx->enemies[i].setObstacleObjYpos(0);
        }
    }


    ctx->mainDoor.initDoorObj(ctx->renderer,"res/doorpencil.png",900,600,64,64);


//End Credit Fonts
    ctx->endFont.Font_BR_Init(ctx->renderer,24,"You Died!",500,100,300,200,ctx->Black);
    ctx->retryFont.Font_BR_Init(ctx->renderer,24,"Retry",550,300,200,100,ctx->Black);

    
    

}
//void handleSignal(int signal) {
    //isRunning = false;
//}

void Input(context *ctx){
  //  if(gameplayState == true)
  //  {
       
        // Start our event loop
        //obstacles.isColliding(playerOne,collision);
        while(SDL_PollEvent(&ctx->event)){
            // Handle each specific event
            if(ctx->event.type == SDL_QUIT){
                ctx->gameIsRunning= false;
                //signal(SIGINT, handleSignal);
                SDL_Quit();
            }
        }
}
void Update(context *ctx){
    
    if(ctx->menuState == true)
    {
       //cout<<"Current stat is Menu state"<<endl;
       
       ctx->gameplayState_firstTransition = false;
       ctx->menuStateTransition = ctx->collision.checkMouseCollision(ctx->menuFont.Font_BR_getxVal(),ctx->menuFont.Font_BR_getyVal(),ctx->menuFont.Font_BR_getwVal(),ctx->menuFont.Font_BR_gethVal());
       //Input();
    }
    if(ctx->menuStateTransition == true)
    {
        //cout<<"Going into gameplay "<<endl;
        ctx->menuState = false;
        ctx->gameplayState_first = true;
        ctx->gameoverState = false;
    }

    if(ctx->gameplayState_first == true)
    {

        //cout<<"Current stat is Gameplay state"<<endl;
        ctx->menuStateTransition = false;
        ctx->playerOne.updatePlayerObj(ctx->gameplayState_first,true,ctx->deltaTimeFunc.getDeltaTime(),ctx->moveSpeed);
        //Input();
        if(ctx->playerOne.getPlayerObjXpos() <= 65 || ctx->playerOne.getPlayerObjXpos() >= ((1280 - (64* 2)) - 1))
        {
            for(int i = 0; i < 1; i++)
            {
                if(i == 0)
                {
                        ctx->obs[i].setObstacleObjXpos(200);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                        //obs[i].setObstacleObjXpos(100);
                        //obs[i].setObstacleObjYpos((100 + rand() % (300 - 100 + 1)));
                }
                if(i == 1)
                {
                        ctx->obs[i].setObstacleObjXpos(400);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 2)
                {
                        ctx->obs[i].setObstacleObjXpos(764);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 3)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (200 - 100 + 1)));
                }
                if(i == 4)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((500 + rand() % (600 - 500 + 1)));
                }
            }
            for(int i = 0; i < 1; i++)
            {
                ctx->enemies[i].startMovement();
                if(i == 0)
                {
                    ctx->enemies[i].setObstacleObjXpos(0);
                    ctx->enemies[i].setObstacleObjYpos(0);
                }
                if(i == 1)
                {
                    ctx->enemies[i].setObstacleObjXpos(0);
                    ctx->enemies[i].setObstacleObjYpos(768);
                }
                if(i == 2)
                {
                    ctx->enemies[i].setObstacleObjXpos(1280);
                    ctx->enemies[i].setObstacleObjYpos(0);
                }
            }
            ctx->enemyRemaining = 1;
            ctx->gameoverStateTransition = true; 
            ctx->gameplayState_first = false;
            ctx->playerOne.setPlayerObjXpos(WIDTH/2);
            ctx->playerOne.setPlayerObjYpos(HEIGHT/2);
        }
        if(ctx->playerOne.getPlayerObjYpos() <= 65 || ctx->playerOne.getPlayerObjYpos() >= ((768 - (64* 2)) - 1))
        {
            for(int i = 0; i < 1; i++)
            {
                if(i == 0)
                {
                        ctx->obs[i].setObstacleObjXpos(100);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (300 - 100 + 1)));
                }
                if(i == 1)
                {
                        ctx->obs[i].setObstacleObjXpos(400);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 2)
                {
                        ctx->obs[i].setObstacleObjXpos(764);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 3)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (200 - 100 + 1)));
                }
                if(i == 4)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((500 + rand() % (600 - 500 + 1)));
                }
            }
            for(int i = 0; i < 1; i++)
            {
                ctx->enemies[i].startMovement();
                if(i == 0)
                {
                    ctx->enemies[i].setObstacleObjXpos(0);
                    ctx->enemies[i].setObstacleObjYpos(0);
                }
                if(i == 1)
                {
                    ctx->enemies[i].setObstacleObjXpos(0);
                    ctx->enemies[i].setObstacleObjYpos(768);
                }
                if(i == 2)
                {
                    ctx->enemies[i].setObstacleObjXpos(1280);
                    ctx->enemies[i].setObstacleObjYpos(0);
                }
            }
            ctx->enemyRemaining = 1;
            ctx->gameoverStateTransition = true; 
            ctx->gameplayState_first = false;
            ctx->playerOne.setPlayerObjXpos(WIDTH/2);
            ctx->playerOne.setPlayerObjYpos(HEIGHT/2);
        }
        for(int i = 0; i < 1; i++)
        {
            if(ctx->collision.checkObjCollision(ctx->obs[i].getObstacleObjXpos(),ctx->obs[i].getObstacleObjYpos(),ctx->obs[i].getObstacleObjW(),ctx->obs[i].getObstacleObjH()
            ,ctx->playerOne.getPlayerObjXpos(),ctx->playerOne.getPlayerObjYpos(),ctx->playerOne.getPlayerObjW(),ctx->playerOne.getPlayerObjH()
            ) == true)
            {
                //cout<<"Contact with obstacle "<<i<<" x is "<<ctx->obs[i].getObstacleObjXpos()<<" y is "<<ctx->obs[i].getObstacleObjYpos()<<endl;
                ctx->gameoverStateTransition = true;
                ctx->gameplayState_first = false;
                ctx->playerOne.setPlayerObjXpos(WIDTH/2);
                ctx->playerOne.setPlayerObjYpos(HEIGHT/2);
                for(int i = 0; i < 1; i++)
                {
                    ctx->enemies[i].startMovement();
                    if(i == 0)
                    {
                        ctx->enemies[i].setObstacleObjXpos(0);
                        ctx->enemies[i].setObstacleObjYpos(0);
                    }
                    if(i == 1)
                    {
                        ctx->enemies[i].setObstacleObjXpos(0);
                        ctx->enemies[i].setObstacleObjYpos(768);
                    }
                    if(i == 2)
                    {
                        ctx->enemies[i].setObstacleObjXpos(1280);
                        ctx->enemies[i].setObstacleObjYpos(0);
                    }
                }
                if(i == 0)
                {
                        ctx->obs[i].setObstacleObjXpos(100);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (300 - 100 + 1)));
                }
                if(i == 1)
                {
                        ctx->obs[i].setObstacleObjXpos(400);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 2)
                {
                        ctx->obs[i].setObstacleObjXpos(764);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 3)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (200 - 100 + 1)));
                }
                if(i == 4)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((500 + rand() % (600 - 500 + 1)));
                }

            }
        }
        for(int i = 0; i < 1; i++)
        {
            //if(enenmyMove == true)
            ctx->enemies[i].updateEnemyPos(ctx->deltaTimeFunc.getDeltaTime(),ctx->playerOne.getPlayerObjXpos(),ctx->playerOne.getPlayerObjYpos());
            if(ctx->collision.checkObjCollision(ctx->enemies[i].getEnemyObjXpos(),ctx->enemies[i].getEnemyObjYpos(),ctx->enemies[i].getEnemyObjW(),ctx->enemies[i].getEnemyObjH(),ctx->playerOne.getPlayerObjXpos(),ctx->playerOne.getPlayerObjYpos(),ctx->playerOne.getPlayerObjW(),ctx->playerOne.getPlayerObjH()) == true)
            {
                ctx->enemyRemaining = 1;
                ctx->gameoverStateTransition = true; 
                ctx->gameplayState_first = false;
                ctx->playerOne.setPlayerObjXpos(WIDTH/2);
                ctx->playerOne.setPlayerObjYpos(HEIGHT/2);
                for(int i = 0; i < 1; i++)
                {
                    ctx->enemies[i].startMovement();
                    if(i == 0)
                    {
                        ctx->enemies[i].setObstacleObjXpos(0);
                        ctx->enemies[i].setObstacleObjYpos(0);
                    }
                    if(i == 1)
                    {
                        ctx->enemies[i].setObstacleObjXpos(0);
                        ctx->enemies[i].setObstacleObjYpos(768);
                    }
                    if(i == 2)
                    {
                        ctx->enemies[i].setObstacleObjXpos(1280);
                        ctx->enemies[i].setObstacleObjYpos(0);
                    }
                }
            }
        }
        for(int i = 0; i < 1; i++)
        {
            for(int j = 0; j < 1; j++)
            {
                if(ctx->collision.checkObjCollision(ctx->obs[i].getObstacleObjXpos(),ctx->obs[i].getObstacleObjYpos(),ctx->obs[i].getObstacleObjW(),ctx->obs[i].getObstacleObjH(),
                ctx->enemies[j].getEnemyObjXpos(),ctx->enemies[j].getEnemyObjYpos(),ctx->enemies[j].getEnemyObjW(),ctx->enemies[j].getEnemyObjH()) == true)
                {
                    //cout<<"Obj "<<i<<" "<<" is colliding with enemy "<<j<<endl;
                    //enemies[j].updateEnemyPos(deltaTimeFunc.getDeltaTime(),playerOne.getPlayerObjXpos(),playerOne.getPlayerObjYpos());
                    ctx->enemies[j].setObstacleObjXpos(-100);
                    ctx->enemies[j].setObstacleObjYpos(-100);
                    ctx->enemies[j].preventMovement();
                    ctx->enemyRemaining--;
                    if(ctx->enemyRemaining == 0)
                    {
                       // cout<<"No enemies left"<<endl;
                        ctx->mainDoor.setDoorXpos(900);
                        ctx->mainDoor.setDoorYpos(600);
                    }
                    //enenmyMove = false;
                }
            }
        }
        if(ctx->collision.checkObjCollision(ctx->mainDoor.getDoorObjXpos(),ctx->mainDoor.getDoorObjYpos(),ctx->mainDoor.getDoorObjW(),ctx->mainDoor.getDoorObjH()
            ,ctx->playerOne.getPlayerObjXpos(),ctx->playerOne.getPlayerObjYpos(),ctx->playerOne.getPlayerObjW(),ctx->playerOne.getPlayerObjH()
            ) == true && ctx->enemyRemaining == 0)
        {
            ctx->gameplayState_first = false;
            ctx->gameplayState_second = true;
            ctx->gameplayState = false;
            ctx->mainDoor.setDoorXpos(900);
            ctx->mainDoor.setDoorYpos(900);
            ctx->enemyRemaining = 1;
            for(int i = 0; i < 1; i++)
            {
                ctx->enemies[i].startMovement();
                ctx->enemies[i].setObstacleObjXpos(0);
                ctx->enemies[i].setObstacleObjYpos(0);

            }
        }        
    }
    if(ctx->gameplayState_second == true)
    {
       // cout<<"Current level is second level"<<endl;
        ctx->menuStateTransition = false;
        ctx->playerOne.updatePlayerObj(ctx->gameplayState_second,true,ctx->deltaTimeFunc.getDeltaTime(),ctx->moveSpeed);
        //Input();
        if(ctx->playerOne.getPlayerObjXpos() <= 65 || ctx->playerOne.getPlayerObjXpos() >= ((1280 - (64* 2)) - 1))
        {
            for(int i = 0; i < 3; i++)
            {
                if(i == 0)
                {
                        ctx->obs[i].setObstacleObjXpos(200);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                        //obs[i].setObstacleObjXpos(100);
                        //obs[i].setObstacleObjYpos((100 + rand() % (300 - 100 + 1)));
                }
                if(i == 1)
                {
                        ctx->obs[i].setObstacleObjXpos(400);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 2)
                {
                        ctx->obs[i].setObstacleObjXpos(764);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 3)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (200 - 100 + 1)));
                }
                if(i == 4)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((500 + rand() % (600 - 500 + 1)));
                }
            }
            for(int i = 0; i < 2; i++)
            {
                ctx->enemies[i].startMovement();
                if(i == 0)
                {
                    ctx->enemies[i].setObstacleObjXpos(1280);
                    ctx->enemies[i].setObstacleObjYpos(768);
                }
                if(i == 1)
                {
                    ctx->enemies[i].setObstacleObjXpos(0);
                    ctx->enemies[i].setObstacleObjYpos(768);
                }
                if(i == 2)
                {
                    ctx->enemies[i].setObstacleObjXpos(1280);
                    ctx->enemies[i].setObstacleObjYpos(0);
                }
            }
            ctx->level_two_enemyRemaining = 2;
            ctx->gameoverStateTransition = true; 
            ctx->gameplayState_second = false;
            ctx->playerOne.setPlayerObjXpos(WIDTH/2);
            ctx->playerOne.setPlayerObjYpos(HEIGHT/2);
        }
        if(ctx->playerOne.getPlayerObjYpos() <= 65 || ctx->playerOne.getPlayerObjYpos() >= ((768 - (64* 2)) - 1))
        {
            for(int i = 0; i < 3; i++)
            {
                if(i == 0)
                {
                        ctx->obs[i].setObstacleObjXpos(100);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (300 - 100 + 1)));
                }
                if(i == 1)
                {
                        ctx->obs[i].setObstacleObjXpos(400);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 2)
                {
                        ctx->obs[i].setObstacleObjXpos(764);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 3)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (200 - 100 + 1)));
                }
                if(i == 4)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((500 + rand() % (600 - 500 + 1)));
                }
            }
            for(int i = 0; i < 2; i++)
            {
                ctx->enemies[i].startMovement();
                if(i == 0)
                {
                    ctx->enemies[i].setObstacleObjXpos(1280);
                    ctx->enemies[i].setObstacleObjYpos(768);
                }
                if(i == 1)
                {
                    ctx->enemies[i].setObstacleObjXpos(0);
                    ctx->enemies[i].setObstacleObjYpos(768);
                }
                if(i == 2)
                {
                    ctx->enemies[i].setObstacleObjXpos(1280);
                    ctx->enemies[i].setObstacleObjYpos(0);
                }
            }
            ctx->level_two_enemyRemaining = 2;
            ctx->gameoverStateTransition = true; 
            ctx->gameplayState_second = false;
            ctx->playerOne.setPlayerObjXpos(WIDTH/2);
            ctx->playerOne.setPlayerObjYpos(HEIGHT/2);
        }
        for(int i = 0; i < 3; i++)
        {
            if(ctx->collision.checkObjCollision(ctx->obs[i].getObstacleObjXpos(),ctx->obs[i].getObstacleObjYpos(),ctx->obs[i].getObstacleObjW(),ctx->obs[i].getObstacleObjH()
            ,ctx->playerOne.getPlayerObjXpos(),ctx->playerOne.getPlayerObjYpos(),ctx->playerOne.getPlayerObjW(),ctx->playerOne.getPlayerObjH()
            ) == true)
            {
                ctx->level_two_enemyRemaining = 2;
                //cout<<"Contact with obstacle "<<i<<" x is "<<obs[i].getObstacleObjXpos()<<" y is "<<obs[i].getObstacleObjYpos()<<endl;
                ctx->gameoverStateTransition = true;
                ctx->gameplayState_second = false;
                ctx->playerOne.setPlayerObjXpos(WIDTH/2);
                ctx->playerOne.setPlayerObjYpos(HEIGHT/2);
                for(int i = 0; i < 2; i++)
                {
                    ctx->enemies[i].startMovement();
                    if(i == 0)
                    {
                        ctx->enemies[i].setObstacleObjXpos(1280);
                        ctx->enemies[i].setObstacleObjYpos(768);
                    }
                    if(i == 1)
                    {
                        ctx->enemies[i].setObstacleObjXpos(0);
                        ctx->enemies[i].setObstacleObjYpos(768);
                    }
                    if(i == 2)
                    {
                        ctx->enemies[i].setObstacleObjXpos(1280);
                        ctx->enemies[i].setObstacleObjYpos(0);
                    }
                }
                if(i == 0)
                {
                        ctx->obs[i].setObstacleObjXpos(100);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (300 - 100 + 1)));
                }
                if(i == 1)
                {
                        ctx->obs[i].setObstacleObjXpos(400);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 2)
                {
                        ctx->obs[i].setObstacleObjXpos(764);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 3)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (200 - 100 + 1)));
                }
                if(i == 4)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((500 + rand() % (600 - 500 + 1)));
                }

            }
        }
        for(int i = 0; i < 2; i++)
        {
            //if(enenmyMove == true)
            ctx->enemies[i].updateEnemyPos(ctx->deltaTimeFunc.getDeltaTime(),ctx->playerOne.getPlayerObjXpos(),ctx->playerOne.getPlayerObjYpos());
            if(ctx->collision.checkObjCollision(ctx->enemies[i].getEnemyObjXpos(),ctx->enemies[i].getEnemyObjYpos(),ctx->enemies[i].getEnemyObjW(),ctx->enemies[i].getEnemyObjH(),ctx->playerOne.getPlayerObjXpos(),ctx->playerOne.getPlayerObjYpos(),ctx->playerOne.getPlayerObjW(),ctx->playerOne.getPlayerObjH()) == true)
            {
                ctx->level_two_enemyRemaining = 2;
                ctx->gameoverStateTransition = true; 
                ctx->gameplayState_second = false;
                ctx->playerOne.setPlayerObjXpos(WIDTH/2);
                ctx->playerOne.setPlayerObjYpos(HEIGHT/2);
                for(int i = 0; i < 2; i++)
                {
                    ctx->enemies[i].startMovement();
                    if(i == 0)
                    {
                        ctx->enemies[i].setObstacleObjXpos(0);
                        ctx->enemies[i].setObstacleObjYpos(0);
                    }
                    if(i == 1)
                    {
                        ctx->enemies[i].setObstacleObjXpos(0);
                        ctx->enemies[i].setObstacleObjYpos(768);
                    }
                    if(i == 2)
                    {
                        ctx->enemies[i].setObstacleObjXpos(1280);
                        ctx->enemies[i].setObstacleObjYpos(0);
                    }
                }
            }
        }
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 2; j++)
            {
                if(ctx->collision.checkObjCollision(ctx->obs[i].getObstacleObjXpos(),ctx->obs[i].getObstacleObjYpos(),ctx->obs[i].getObstacleObjW(),ctx->obs[i].getObstacleObjH(),
                ctx->enemies[j].getEnemyObjXpos(),ctx->enemies[j].getEnemyObjYpos(),ctx->enemies[j].getEnemyObjW(),ctx->enemies[j].getEnemyObjH()) == true)
                {
                    //cout<<"Obj "<<i<<" "<<" is colliding with enemy "<<j<<endl;
                    //enemies[j].updateEnemyPos(deltaTimeFunc.getDeltaTime(),playerOne.getPlayerObjXpos(),playerOne.getPlayerObjYpos());
                    ctx->enemies[j].setObstacleObjXpos(-100);
                    ctx->enemies[j].setObstacleObjYpos(-100);
                    ctx->enemies[j].preventMovement();
                    ctx->level_two_enemyRemaining--;
                    if(ctx->level_two_enemyRemaining == 0)
                    {
                        //cout<<"No enemies left"<<endl;
                        ctx->mainDoor.setDoorXpos(900);
                        ctx->mainDoor.setDoorYpos(600);
                    }
                    //enenmyMove = false;
                }
            }
        }
        if(ctx->collision.checkObjCollision(ctx->mainDoor.getDoorObjXpos(),ctx->mainDoor.getDoorObjYpos(),ctx->mainDoor.getDoorObjW(),ctx->mainDoor.getDoorObjH()
            ,ctx->playerOne.getPlayerObjXpos(),ctx->playerOne.getPlayerObjYpos(),ctx->playerOne.getPlayerObjW(),ctx->playerOne.getPlayerObjH()
            ) == true && ctx->level_two_enemyRemaining == 0)
        {
            ctx->gameplayState_first = false;
            ctx->gameplayState_second = false;
            ctx->gameplayState = true;
            ctx->mainDoor.setDoorXpos(900);
            ctx->mainDoor.setDoorYpos(900);
            ctx->level_two_enemyRemaining = 2;
            for(int i = 0; i < 2; i++)
            {
                ctx->enemies[i].startMovement();
                if(i == 0)
                {
                    ctx->enemies[i].setObstacleObjXpos(0);
                    ctx->enemies[i].setObstacleObjYpos(0);
                }
                if(i == 1)
                {
                    ctx->enemies[i].setObstacleObjXpos(0);
                    ctx->enemies[i].setObstacleObjYpos(768);
                }
            }
        }  
    }
    if(ctx->gameplayState == true)
    {
        ctx->menuStateTransition = false;
        ctx->playerOne.updatePlayerObj(ctx->gameplayState,true,ctx->deltaTimeFunc.getDeltaTime(),ctx->moveSpeed);
        //Input();
        if(ctx->playerOne.getPlayerObjXpos() <= 65 || ctx->playerOne.getPlayerObjXpos() >= ((1280 - (64* 2)) - 1))
        {
            for(int i = 0; i < 5; i++)
            {
                if(i == 0)
                {
                        ctx->obs[i].setObstacleObjXpos(200);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                        //obs[i].setObstacleObjXpos(100);
                        //obs[i].setObstacleObjYpos((100 + rand() % (300 - 100 + 1)));
                }
                if(i == 1)
                {
                        ctx->obs[i].setObstacleObjXpos(400);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 2)
                {
                        ctx->obs[i].setObstacleObjXpos(764);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 3)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (200 - 100 + 1)));
                }
                if(i == 4)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((500 + rand() % (600 - 500 + 1)));
                }
            }
            for(int i = 0; i < 3; i++)
            {
                ctx->enemies[i].startMovement();
                if(i == 0)
                {
                    ctx->enemies[i].setObstacleObjXpos(1280);
                    ctx->enemies[i].setObstacleObjYpos(768);
                }
                if(i == 1)
                {
                    ctx->enemies[i].setObstacleObjXpos(0);
                    ctx->enemies[i].setObstacleObjYpos(768);
                }
                if(i == 2)
                {
                    ctx->enemies[i].setObstacleObjXpos(1280);
                    ctx->enemies[i].setObstacleObjYpos(0);
                }
            }
            ctx->level_one_enemyRemaining = 3;
            ctx->gameoverStateTransition = true; 
            ctx->gameplayState = false;
            ctx->playerOne.setPlayerObjXpos(WIDTH/2);
            ctx->playerOne.setPlayerObjYpos(HEIGHT/2);
        }
        if(ctx->playerOne.getPlayerObjYpos() <= 65 || ctx->playerOne.getPlayerObjYpos() >= ((768 - (64* 2)) - 1))
        {
            for(int i = 0; i < 5; i++)
            {
                if(i == 0)
                {
                        ctx->obs[i].setObstacleObjXpos(100);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (300 - 100 + 1)));
                }
                if(i == 1)
                {
                        ctx->obs[i].setObstacleObjXpos(400);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 2)
                {
                        ctx->obs[i].setObstacleObjXpos(764);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 3)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (200 - 100 + 1)));
                }
                if(i == 4)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((500 + rand() % (600 - 500 + 1)));
                }
            }
            for(int i = 0; i < 3; i++)
            {
                ctx->enemies[i].startMovement();
                if(i == 0)
                {
                    ctx->enemies[i].setObstacleObjXpos(1280);
                    ctx->enemies[i].setObstacleObjYpos(768);
                }
                if(i == 1)
                {
                    ctx->enemies[i].setObstacleObjXpos(0);
                    ctx->enemies[i].setObstacleObjYpos(768);
                }
                if(i == 2)
                {
                    ctx->enemies[i].setObstacleObjXpos(1280);
                    ctx->enemies[i].setObstacleObjYpos(0);
                }
            }
            ctx->level_one_enemyRemaining = 3;
            ctx->gameoverStateTransition = true; 
            ctx->gameplayState = false;
            ctx->playerOne.setPlayerObjXpos(WIDTH/2);
            ctx->playerOne.setPlayerObjYpos(HEIGHT/2);
        }
        for(int i = 0; i < 5; i++)
        {
            if(ctx->collision.checkObjCollision(ctx->obs[i].getObstacleObjXpos(),ctx->obs[i].getObstacleObjYpos(),ctx->obs[i].getObstacleObjW(),ctx->obs[i].getObstacleObjH()
            ,ctx->playerOne.getPlayerObjXpos(),ctx->playerOne.getPlayerObjYpos(),ctx->playerOne.getPlayerObjW(),ctx->playerOne.getPlayerObjH()
            ) == true)
            {
                //cout<<"Contact with obstacle "<<i<<" x is "<<obs[i].getObstacleObjXpos()<<" y is "<<obs[i].getObstacleObjYpos()<<endl;
                ctx->gameoverStateTransition = true;
                ctx->gameplayState = false;
                ctx->level_one_enemyRemaining = 3;
                ctx->playerOne.setPlayerObjXpos(WIDTH/2);
                ctx->playerOne.setPlayerObjYpos(HEIGHT/2);
                for(int i = 0; i < 3; i++)
                {
                    ctx->enemies[i].startMovement();
                    if(i == 0)
                    {
                        ctx->enemies[i].setObstacleObjXpos(1280);
                        ctx->enemies[i].setObstacleObjYpos(768);
                    }
                    if(i == 1)
                    {
                        ctx->enemies[i].setObstacleObjXpos(0);
                        ctx->enemies[i].setObstacleObjYpos(768);
                    }
                    if(i == 2)
                    {
                        ctx->enemies[i].setObstacleObjXpos(1280);
                        ctx->enemies[i].setObstacleObjYpos(0);
                    }
                }
                if(i == 0)
                {
                        ctx->obs[i].setObstacleObjXpos(100);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (300 - 100 + 1)));
                }
                if(i == 1)
                {
                        ctx->obs[i].setObstacleObjXpos(400);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 2)
                {
                        ctx->obs[i].setObstacleObjXpos(764);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (500 - 100 + 1)));
                }
                if(i == 3)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((100 + rand() % (200 - 100 + 1)));
                }
                if(i == 4)
                {
                        ctx->obs[i].setObstacleObjXpos(1000);
                        ctx->obs[i].setObstacleObjYpos((500 + rand() % (600 - 500 + 1)));
                }

            }
        }
        for(int i = 0; i < 3; i++)
        {
            //if(enenmyMove == true)
            ctx->enemies[i].updateEnemyPos(ctx->deltaTimeFunc.getDeltaTime(),ctx->playerOne.getPlayerObjXpos(),ctx->playerOne.getPlayerObjYpos());
            if(ctx->collision.checkObjCollision(ctx->enemies[i].getEnemyObjXpos(),ctx->enemies[i].getEnemyObjYpos(),ctx->enemies[i].getEnemyObjW(),ctx->enemies[i].getEnemyObjH(),ctx->playerOne.getPlayerObjXpos(),ctx->playerOne.getPlayerObjYpos(),ctx->playerOne.getPlayerObjW(),ctx->playerOne.getPlayerObjH()) == true)
            {
                ctx->level_one_enemyRemaining = 3;
                ctx->gameoverStateTransition = true; 
                ctx->gameplayState = false;
                ctx->playerOne.setPlayerObjXpos(WIDTH/2);
                ctx->playerOne.setPlayerObjYpos(HEIGHT/2);
                for(int i = 0; i < 3; i++)
                {
                    ctx->enemies[i].startMovement();
                    if(i == 0)
                    {
                        ctx->enemies[i].setObstacleObjXpos(0);
                        ctx->enemies[i].setObstacleObjYpos(0);
                    }
                    if(i == 1)
                    {
                        ctx->enemies[i].setObstacleObjXpos(0);
                        ctx->enemies[i].setObstacleObjYpos(768);
                    }
                    if(i == 2)
                    {
                        ctx->enemies[i].setObstacleObjXpos(1280);
                        ctx->enemies[i].setObstacleObjYpos(0);
                    }
                }
            }
        }
        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                if(ctx->collision.checkObjCollision(ctx->obs[i].getObstacleObjXpos(),ctx->obs[i].getObstacleObjYpos(),ctx->obs[i].getObstacleObjW(),ctx->obs[i].getObstacleObjH(),
                ctx->enemies[j].getEnemyObjXpos(),ctx->enemies[j].getEnemyObjYpos(),ctx->enemies[j].getEnemyObjW(),ctx->enemies[j].getEnemyObjH()) == true)
                {
                    //cout<<"Obj "<<i<<" "<<" is colliding with enemy "<<j<<endl;
                    //enemies[j].updateEnemyPos(deltaTimeFunc.getDeltaTime(),playerOne.getPlayerObjXpos(),playerOne.getPlayerObjYpos());
                    ctx->enemies[j].setObstacleObjXpos(-100);
                    ctx->enemies[j].setObstacleObjYpos(-100);
                    ctx->enemies[j].preventMovement();
                    ctx->level_one_enemyRemaining--;
                    if(ctx->level_one_enemyRemaining == 0)
                    {
                        //cout<<"No enemies left"<<endl;
                        ctx->mainDoor.setDoorXpos(900);
                        ctx->mainDoor.setDoorYpos(600);
                    }
                    //enenmyMove = false;
                }
            }
        }
        if(ctx->collision.checkObjCollision(ctx->mainDoor.getDoorObjXpos(),ctx->mainDoor.getDoorObjYpos(),ctx->mainDoor.getDoorObjW(),ctx->mainDoor.getDoorObjH()
            ,ctx->playerOne.getPlayerObjXpos(),ctx->playerOne.getPlayerObjYpos(),ctx->playerOne.getPlayerObjW(),ctx->playerOne.getPlayerObjH()
            ) == true && ctx->level_one_enemyRemaining == 0)
        {
            ctx->gameplayState_first = false;
            ctx->gameplayState_second = false;
            ctx->gameplayState = false;
            ctx->menuState = true;
            ctx->mainDoor.setDoorXpos(900);
            ctx->mainDoor.setDoorYpos(900);
            ctx->level_one_enemyRemaining = 3;
            for(int i = 0; i < 3; i++)
            {
                ctx->enemies[i].startMovement();
                if(i == 0)
                {
                    ctx->enemies[i].setObstacleObjXpos(0);
                    ctx->enemies[i].setObstacleObjYpos(0);
                }
                if(i == 1)
                {
                    ctx->enemies[i].setObstacleObjXpos(0);
                    ctx->enemies[i].setObstacleObjYpos(768);
                }
                if(i == 2)
                {
                    ctx->enemies[i].setObstacleObjXpos(1280);
                    ctx->enemies[i].setObstacleObjYpos(0);
                }
            }
        }  
    }

    if(ctx->gameoverStateTransition == true)
    {
        //cout<<"Going into gameover "<<endl;

        ctx->menuState= false;
        ctx->gameplayState_first = false;
        ctx->gameoverState = true; 
    }
    if(ctx->gameoverState == true)
    {
      // cout<<"Current stat is Gameover state"<<endl; 
       ctx->gameoverStateTransition = false;
       ctx->gameplayState_first = false;

       //cout<<"Gameplay state is "<<gameplayState<<endl;
       ctx->gameplayState_firstTransition = ctx->collision.checkMouseCollision(ctx->retryFont.Font_BR_getxVal(),ctx->retryFont.Font_BR_getyVal(),ctx->retryFont.Font_BR_getwVal(),ctx->retryFont.Font_BR_gethVal());
       //Input(ctx);
    }
    if(ctx->gameplayState_firstTransition == true)
    {
        //cout<<"Going into menu "<<endl;
        ctx->menuState = true;
        ctx->gameplayState_first = false;
        ctx->gameoverState = false;
    }
   

}


   // }


void Draw(context *ctx){
    SDL_SetRenderDrawColor(ctx->renderer,0,0,0,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ctx->renderer);
   
    if(ctx->menuState == true)
    {
        ctx->newImage.Image_BR_Draw(ctx->renderer);
        ctx->titleFont.Font_Br_Draw(ctx->renderer);
        ctx->menuFont.Font_Br_Draw(ctx->renderer);
        ctx->creditFont.Font_Br_Draw(ctx->renderer);
        ctx->tutFont.Font_Br_Draw(ctx->renderer);
    }
    if(ctx->gameplayState_first == true)
    {
        
        ctx->newImage.Image_BR_Draw(ctx->renderer);
        ctx->BG_Tile.TileMap_BR_Draw(ctx->renderer);
        ctx->levelFont_One.Font_Br_Draw(ctx->renderer);

        ctx->playerOne.renderPlayerObjAnim(ctx->renderer);
        for(int i = 0; i < 1;i++)
            ctx->enemies[i].renderEnemyObj(ctx->renderer); 

        for(int i = 0; i < 1; i++)
            ctx->obs[i].renderObstacleObj(ctx->renderer);

        if(ctx->enemyRemaining == 0)
        {
            ctx->mainDoor.renderDoorObj(ctx->renderer);
        }
            


       //mainDoor.renderDoorObj(renderer);
    }
    if(ctx->gameplayState_second == true)
    {
        ctx->newImage.Image_BR_Draw(ctx->renderer);
        ctx->BG_Tile.TileMap_BR_Draw(ctx->renderer);
        ctx->levelFont_Two.Font_Br_Draw(ctx->renderer);

        ctx->playerOne.renderPlayerObjAnim(ctx->renderer);
        for(int i = 0; i < 2;i++)
            ctx->enemies[i].renderEnemyObj(ctx->renderer); 

        for(int i = 0; i < 3; i++)
            ctx->obs[i].renderObstacleObj(ctx->renderer);

        if(ctx->level_two_enemyRemaining == 0)
        {
            ctx->mainDoor.renderDoorObj(ctx->renderer);
        }      
    }
    if(ctx->gameplayState == true)
    {
        ctx->newImage.Image_BR_Draw(ctx->renderer);
        ctx->BG_Tile.TileMap_BR_Draw(ctx->renderer);
        ctx->levelFont_Three.Font_Br_Draw(ctx->renderer);

        ctx->playerOne.renderPlayerObjAnim(ctx->renderer);
        for(int i = 0; i < 3;i++)
            ctx->enemies[i].renderEnemyObj(ctx->renderer); 

        for(int i = 0; i < 5; i++)
            ctx->obs[i].renderObstacleObj(ctx->renderer);

        if(ctx->level_one_enemyRemaining == 0)
        {
            ctx->mainDoor.renderDoorObj(ctx->renderer);
        }       
    }
    if(ctx->gameoverState == true)
    {
        ctx->newImage.Image_BR_Draw(ctx->renderer);
        ctx->endFont.Font_Br_Draw(ctx->renderer);
        ctx->retryFont.Font_Br_Draw(ctx->renderer);
    }
   // 
    
    SDL_RenderPresent(ctx->renderer);
}


void loop(context *ctx)
{
   // while(ctx->gameIsRunning){ //Main Game Function! 
        ctx->frameStart = SDL_GetTicks(); // Capping it at 60 fps


        Update(ctx);
        //Input(ctx);
        Draw(ctx);
        

        ctx->frameTime = SDL_GetTicks() - ctx->frameStart;

        if(ctx->frameDelay > ctx->frameTime)
        {
            SDL_Delay(ctx->frameDelay - ctx->frameTime);
         

        }

            
   // }

}
void mainloop(void *args)
{
    context *ctx = static_cast<context*>(args);
    loop(ctx);
}

int main(int argc, char* argv[]){
    context ctx;
   /* SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);*/





    // Setup our function pointers
    //gladLoadGLLoader(SDL_GL_GetProcAddress);
    SDL_Init_BR sdlInit("Zi in a Haunted House!", 50, 50, WIDTH, HEIGHT);
    ctx.window = sdlInit.window();
    ctx.renderer = sdlInit.renderer();

    // OpenGL setup the graphics context
    //context = SDL_GL_CreateContext(window);

    Init(&ctx); //Initialize function
    emscripten_set_main_loop_arg(mainloop, &ctx, -1, 1);


   /* while(gameIsRunning){ //Main Game Function! 
        frameStart = SDL_GetTicks(); // Capping it at 60 fps


        Update();
       // Input();
        Draw();
        

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
         

        }
            
    }*/

    //SDL_Delay(3000);

    //SDL_Delay(3000);

    //SDL_Delay(3000);

    //SDL_Delay(3000);

    //SDL_Delay(3000);


    //SDL_Quit();
    return 0;
}
