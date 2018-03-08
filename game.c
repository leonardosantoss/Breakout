#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 420
#define PLAYER_WIDTH 64
#define PLAYER_HEIGHT 8
#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 12
#define ROW 6
#define COLUMN 18
#define FONTSIZE 35
#define BALL_WIDTH 8
#define BALL_HEIGHT 8

const int true = 1;
const int false = 0;



typedef int bool;

typedef struct _ball
{
    int positionX;
    int positionY;
    int stepX;
    int stepY;
    int width;
    int height;
    SDL_Surface* image;
} ball;

typedef struct _player 
{
    int positionX;
    int positionY;
    int width;
    int height;
    SDL_Surface* image;
} player;

typedef struct _block
{
    int positionX;
    int positionY;
    int width;
    int height;
    SDL_Surface* image;
} block;


bool init(void);
bool loadMedia(void);
SDL_Surface* loadSurface(char *path);
void drawText(char* texto, SDL_Surface* dst, int x, int y);
player createPlayer(int positionX, int positionY, SDL_Surface *image);
block createBlock(int positionX, int positionY, SDL_Surface *image);
ball createBall( int positionX, int positionY, int stepX, int stepY, SDL_Surface *image);

bool moveBall(ball *p, player *q);
bool collision(ball *b, player *p);
bool collisionBlock(ball *b, block *k);
bool manager();

void drawPlayer(player *player, int quit);
void drawBall(ball *ball, int quit);
void drawBlock(block *block, int quit);
void run(player *player);
void runGame();
void runMenu();
void youLost();
void manual();
int scene;

player player1;
ball ball1;
block block1[18][6];
bool start;
int score;
int lives = 3;
char buffer[64];
char buffer2[64];
char buffer3[64];
int multiplicador = 1;
int count = 0;
int level = 1;
int countSpeedY = 0;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL ;
SDL_Surface* playerSurface = NULL;
SDL_Surface* ballSurface = NULL;
SDL_Surface* blockSurface[6];
SDL_Surface* backgroundSurface = NULL;
SDL_Surface* menuSurface = NULL;
SDL_Surface* youLostSurface = NULL;
SDL_Event evento;
TTF_Font* fonte;
bool bloco[18][6];

int main (int argc, char** argv)
{    
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {   
         if (TTF_Init() == -1){
            return -1;
         }
         if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
       
        else
        {        
            int quit = false;
                while( !quit )
                {

                    quit = manager();
                    switch(scene)
                    {
                        case 0:
                            runMenu();
                            break;
                        case 1:
                            runGame();
                            break;  
                    }

                }
        }
    }
    TTF_Quit();


    return EXIT_SUCCESS;
}

void runGame()
{   
	

    int quit = false;
                int i, j;
                    SDL_BlitSurface(backgroundSurface, NULL, screenSurface, NULL);
                    drawPlayer(&player1, quit);
                    drawBall(&ball1, quit);
                    collision(&ball1, &player1);
                    for (i = 0; i < 18; i++)
                    {
                        for (j = 0; j < 6; j++)
                        {
                            if(bloco[i][j]==1){
                                drawBlock(&block1[i][j], quit);
                                collisionBlock(&ball1, &block1[i][j]);
                                   
                            }
                        }
                    }
                    

                    if (count == 108)
                    {
                    	 count = 0;
                    	 level++;

                    	 for (i = 0; i < 18; i++)
		                 {
		                        for (j = 0; j < 6; j++)
		                        {
		                            bloco[i][j] = 1;
		                            if(bloco[i][j] == 1){
		                            	drawBlock(&block1[i][j], quit);
                                    	collisionBlock(&ball1, &block1[i][j]);
                                 }
		                        }
		                  }
		                  start = false;
            			  player1 = createPlayer(SCREEN_WIDTH/2 - PLAYER_WIDTH/2, SCREEN_HEIGHT - 34 - PLAYER_HEIGHT/2, playerSurface);
            			  ball1 = createBall(SCREEN_WIDTH/2 -BALL_WIDTH/2, (SCREEN_HEIGHT - 34 - PLAYER_HEIGHT/2) - PLAYER_HEIGHT, 0, 0, ballSurface);
           				  drawPlayer(&player1, quit);
           				  drawBall(&ball1, quit);
           				  countSpeedY++;
            
                    }
                    moveBall(&ball1, &player1);
                    run(&player1);
                    fonte = TTF_OpenFont("Sans.ttf", FONTSIZE);
                    sprintf(buffer, "%d", score);
                    sprintf(buffer2, "%d", lives);
                    sprintf(buffer3, "%d", level);
                    drawText(buffer, screenSurface, 0, 0);
                    drawText(buffer2, screenSurface, 250, 0);
                    drawText(buffer3, screenSurface, 550, 0);
                    TTF_SetFontStyle(fonte, TTF_STYLE_BOLD);
                    SDL_UpdateWindowSurface(window);
                    TTF_CloseFont( fonte );
                    SDL_Delay(50);  
                       if(lives == 0){

                        scene = 5;
                        youLost();
                    }
                    
}

void runMenu()
{
    SDL_BlitSurface(menuSurface, NULL, screenSurface, NULL);
    //SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x0F, 0xF0) );
    SDL_UpdateWindowSurface(window);
    SDL_Delay(50);   
}
void manual(){
    //SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00) );
    SDL_BlitSurface(backgroundSurface, NULL, screenSurface, NULL);
    fonte = TTF_OpenFont("Sans.ttf", FONTSIZE);
    drawText("MANUAL DO JOGO", screenSurface, 70 , 2);
    fonte = TTF_OpenFont("Sans.ttf", 24);
    drawText("Comandos:", screenSurface, 40, 70);
    drawText("Sobre o jogo:", screenSurface, 40, 180);
    fonte = TTF_OpenFont("Sans.ttf", 16);
    drawText("Setas laterais para guiar a barra.", screenSurface, 40, 100);
    drawText("Tecla space para iniciar a partida.", screenSurface, 40, 120);
    drawText("Tecla M para voltar ao menu.", screenSurface, 40, 140);
    drawText("Cada barra vale 100 pontos.", screenSurface, 40, 210);
    drawText("A cada 10000 pontos +1 vida.", screenSurface, 40, 230);
    drawText("Jogador inicia com 3 vidas.", screenSurface, 40, 250);

    TTF_SetFontStyle(fonte, TTF_STYLE_BOLD);   
    SDL_UpdateWindowSurface(window);
    SDL_Delay(50);
    
}

void youLost(){
    SDL_BlitSurface(youLostSurface, NULL, screenSurface, NULL);
    fonte = TTF_OpenFont("Sans.ttf", FONTSIZE);
    sprintf(buffer, "%d", score);
    drawText(buffer, screenSurface, 200 , 240);
    TTF_SetFontStyle(fonte, TTF_STYLE_BOLD);    
    SDL_UpdateWindowSurface(window);
    SDL_Delay(50);

}

bool init()
{
    int success = true ;
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        window = SDL_CreateWindow
        (
            "Breakout", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
        );
    
        if(window == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) & imgFlags))
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            else
            {
                screenSurface = SDL_GetWindowSurface(window);
            }
        }
    }
    return success;
}

bool loadMedia()
{
    bool success = true;
    blockSurface[0] = loadSurface( "./block1.bmp");
    blockSurface[1] = loadSurface( "./block2.bmp");
    blockSurface[2] = loadSurface( "./block3.bmp");
    blockSurface[3] = loadSurface( "./block4.bmp");
    blockSurface[4] = loadSurface( "./block5.bmp");
    blockSurface[5] = loadSurface( "./block6.bmp");
    playerSurface = loadSurface("./paddle.bmp");
    ballSurface = loadSurface( "./ball.bmp" );
    backgroundSurface = loadSurface("./fundo.jpg");
    menuSurface = loadSurface("./Menu.bmp");
    youLostSurface = loadSurface("./GameOver.bmp");

    if(playerSurface == NULL)
    {
        printf("Failed to load PNG image!\n");
        success = false;
    }
    if(ballSurface == NULL)
    {
        printf( "Failed to load JPG image!\n" );
        success = false;
    }

    if(blockSurface == NULL)
    {
        printf( "Failed to load JPG image!\n" );
        success = false;
    }
    if(backgroundSurface == NULL)
    {
        printf( "Failed to load JPG image!\n" );
        success = false;
    }
    if(youLostSurface == NULL){
        printf("Failed to load JPG image!\n");
        success = false;
    }
    return success;
}

SDL_Surface* loadSurface(char *path)
{
    SDL_Surface* optimizedSurface = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0 );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
        }
        SDL_FreeSurface(loadedSurface);
    }
    return optimizedSurface;
}

player createPlayer(int positionX, int positionY, SDL_Surface *image)
{
    player p;
    p.positionX = positionX;
    p.positionY = positionY;
    p.image = image;
    return p;
}

ball createBall(int positionX, int positionY, int stepX, int stepY, SDL_Surface *image)
{
    ball b;
    b.positionX = positionX;
    b.positionY = positionY;
    b.stepX = stepX;
    b.stepY = stepY;
    b.image = image;
    return b;
}

block createBlock(int positionX, int positionY, SDL_Surface *image)
{
    block b;
    b.positionX = positionX;
    b.positionY = positionY;
    b.image = image;
    return b;
}

bool moveBall(ball *p, player *q)
{
    bool quit = false;

    if (start)
    {
        if (p->stepX == 0)
        {
            p->stepX = 2.5;
            p->stepY = -7.5 - countSpeedY * 2;
        }
        p->positionX += p->stepX;
        p->positionY += p->stepY;
        
        if ( (p->positionX + BALL_WIDTH > SCREEN_WIDTH - 32) ||
             (p->positionX < 32) )
        {
            p->stepX = -p->stepX ;
            p->positionX += p->stepX;
        }
        if (p->positionY < 64)
        {
            p->stepY = -p->stepY;
            p->positionY += p->stepY;
        }
        if (p->positionY > SCREEN_HEIGHT)  //Perdeu vida
        {
            lives--;
            start = false;
            player1 = createPlayer(SCREEN_WIDTH/2 - PLAYER_WIDTH/2, SCREEN_HEIGHT - 34 - PLAYER_HEIGHT/2, playerSurface);
            ball1 = createBall(SCREEN_WIDTH/2 -BALL_WIDTH/2, (SCREEN_HEIGHT - 34 - PLAYER_HEIGHT/2) - PLAYER_HEIGHT, 0, 0, ballSurface);
            drawPlayer(&player1, quit);
            drawBall(&ball1, quit);
            runGame();

        }
        
        int i, j;
        for (i = 0; i < 18; i++)
        {
            for (j = 0; j < 6; j++)
            {
                if (collisionBlock(&ball1, &block1[i][j]) && bloco[i][j]==1)
                {
                    bloco[i][j]=0;
                    p->stepY = -p->stepY;
                    p->positionY += p->stepY;
                    score += 100;
                    count++;
                    if(score == 10000*multiplicador){
                        lives++;
                        multiplicador++;
                    }
                }                       
            }
        }
        switch(collision(&ball1, &player1))
        {   
            case 0:
                break;
            case 1:  //Metade esquerda da barra, quando bola tem velocidade em x > 0;
                p->stepY = -p->stepY;
                p->positionY += p->stepY;
                p->stepX = p->stepX / 1.20;
                break;
            case 2: //Metade esquerda da barra, quando bola tem velocidade em x < 0;
                p->stepY = -p->stepY;
                p->positionY += p->stepY;
                p->stepX = p->stepX * 1.20;
                break;
            case 3: //Metade direita da barra, quando bola tem velocidade em x > 0;
                p->stepY = -p->stepY;
                p->positionY += p->stepY;
                p->stepX = p->stepX * 1.20;
                break;
            case 4: //Metade direita da barra, quando bola tem velocidade em x < 0;
                p->stepY = -p->stepY;
                p->positionY += p->stepY;
                p->stepX = p->stepX / 1.20;
                break;      

        }                       
    }
    else
    {
        p-> positionX = q->positionX + PLAYER_WIDTH/2 - BALL_WIDTH/2;
    }
    
    return quit;
}


void run(player *player1)
{  
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT])
    {
        player1->positionX -= 25;
    }
    if (state[SDL_SCANCODE_RIGHT])
    {
        player1->positionX += 25;
    }
    
    if (player1->positionX > SCREEN_WIDTH - PLAYER_WIDTH - 32)
    {
        player1->positionX = SCREEN_WIDTH - PLAYER_WIDTH - 32;
    }
    else if (player1->positionX < 32)
    {
        player1->positionX = 32;
    }
}

bool manager()
{
    bool quit = false;
    SDL_Event e;
    while(SDL_PollEvent( &e ) != 0)
    {
        switch (e.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if ((e.key.keysym.sym == SDLK_ESCAPE) || (e.key.keysym.sym == SDLK_q))
                {
                    quit = true;
                }
                else if ((e.key.keysym.sym == SDLK_SPACE))
                {
                    start = true;
                }
                else if ((e.key.keysym.sym == SDLK_RETURN) && scene != 1)
                {
                    scene = 1;
                    lives = 3;
                    score =0;
                    start = false;
                    player1 = createPlayer(SCREEN_WIDTH/2 - PLAYER_WIDTH/2, SCREEN_HEIGHT - 34 - PLAYER_HEIGHT/2, playerSurface);
                    ball1 = createBall(SCREEN_WIDTH/2 -BALL_WIDTH/2, (SCREEN_HEIGHT - 34 - PLAYER_HEIGHT/2) - PLAYER_HEIGHT, 0, 0, ballSurface);
                    int i, j;
                    if(scene == 1){
                    for (i = 0; i < 18; i++)
                    {
                        for (j = 0; j < 6; j++)
                        {
                            bloco[i][j]=1;
                            block1[i][j] = createBlock(i*32 + 32, j*12 + 114, blockSurface[j]);
                            
                        }
                    }
                }
            }
                 else if ((e.key.keysym.sym == SDLK_i))
                 {
                 	scene = 3;
                    manual();
                 }
                  else if ((e.key.keysym.sym == SDLK_m)){
                    scene = 0;
                    runMenu();
                 }
                break;
        }
    }
    
    return quit;
}

bool collision(ball *b, player *p)
{
    int case0 = 0, case1 = 1, case2 = 2, case3 = 3, case4 = 4;

    if((b->positionY + BALL_HEIGHT) > p->positionY && b->positionY < (p->positionY + PLAYER_HEIGHT)
        && b->positionX < (p->positionX + PLAYER_WIDTH/2) && b->positionX > p->positionX && 
        b->stepX > 0 ){
        return case1;   // Bola bate na METADE ESQUERDA da barra, com Vx POSITIVO
    }
    
    else if(b->positionX < p->positionX && (b->positionX + BALL_WIDTH) > p->positionX &&
    (b->positionY + BALL_HEIGHT) > p->positionY && b->positionY < (p->positionY + PLAYER_HEIGHT) && b->stepX > 0) {
        return case1;   //bola bate na QUINA pela ESQUERDA com Vx POSITIVO
    }
    
    else if((b->positionY + BALL_HEIGHT) > p->positionY && b->positionY < (p->positionY + PLAYER_HEIGHT)
        && b->positionX < (p->positionX + PLAYER_WIDTH/2) && b->positionX > p->positionX && 
        b->stepX < 0){
        return case2;   //Bola bate na METADE ESQUERDA da barra, com Vx NEGATIVO
    }   

    else if(b->positionX < p->positionX && (b->positionX + BALL_WIDTH) > p->positionX &&
    (b->positionY + BALL_HEIGHT) > p->positionY && b->positionY < (p->positionY + PLAYER_HEIGHT) && b->stepX < 0) {
        return case2;   //bola bate na QUINA pela ESQUERDA com Vx NEGATIVO
    }

    else if((b->positionY + BALL_HEIGHT) > p->positionY && b->positionY < (p->positionY + PLAYER_HEIGHT)
        && b->positionX > (p->positionX + PLAYER_WIDTH/2) && b->positionX < (p->positionX + PLAYER_WIDTH) && 
        b->stepX > 0){
        return case3;  //Bola bate na METADE DIREITA da barra, com Vx POSITIVO
    }
    else if((b->positionY + BALL_HEIGHT) > p->positionY && b->positionY < (p->positionY + PLAYER_HEIGHT)
        && b->positionX > (p->positionX + PLAYER_WIDTH/2) && b->positionX < (p->positionX + PLAYER_WIDTH) && 
        b->stepX < 0){
        return case4;  //Bola bate na METADE DIREITA da barra, com Vx NEGATIVO
    }

    else{
        return case0;  //Sem colisões.
    }   

}

bool collisionBlock(ball *b, block *k)
{
    if(b->positionY < (k->positionY + BLOCK_HEIGHT) && (b->positionY + BALL_HEIGHT) > k->positionY &&
      (b->positionX + BALL_WIDTH) > k->positionX && b->positionX < (k->positionX + BLOCK_WIDTH))
    {

        return true;
    }
    else
    {
        return false;
    }    
}

void drawPlayer (player *player1, int quit)
{    
    SDL_Rect srcRect, dstRect;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = PLAYER_WIDTH;
        srcRect.h = PLAYER_HEIGHT;
        dstRect.x = player1->positionX;
        dstRect.y = player1->positionY;    

    if( SDL_BlitSurface(player1->image, &srcRect, screenSurface, &dstRect ) < 0 )
    {
        printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
        quit = true;
    }

    quit = false;
}

void drawBall(ball *ball1, int quit)
{
    SDL_Rect srcRect, dstRect;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = BALL_HEIGHT;
        srcRect.h = BALL_WIDTH;
        dstRect.x = ball1->positionX;
        dstRect.y = ball1->positionY;
    if( SDL_BlitSurface(ball1->image, &srcRect, screenSurface, &dstRect ) < 0 )
    {
        printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
        quit = true;
    }

    quit = false;
}

void drawBlock(block *block1, int quit)
{
            SDL_Rect srcRect, dstRect;
            srcRect.x = 0;
            srcRect.y = 0;
            srcRect.w = BLOCK_WIDTH;
            srcRect.h = BLOCK_HEIGHT;
            dstRect.x = block1->positionX;
            dstRect.y = block1->positionY;
            if( SDL_BlitSurface(block1->image, &srcRect, screenSurface, &dstRect ) < 0 )
            {
                printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                quit = true;
            }
}

void drawText(char* texto, SDL_Surface* dst, int x, int y)
{
  SDL_Color cor = {255, 255, 255}; //branco
  SDL_Surface* src = TTF_RenderText_Blended(fonte, texto, cor);
  SDL_Rect dst_rect = {x, y, 0, 0};
  //SDL_Rect dst_rect2 = {x, y, 0, 0};
  
  SDL_BlitSurface(src, NULL, dst, &dst_rect);
  //SDL_BlitSurface(src, NULL, dst, &dst_rect2);
  
  SDL_FreeSurface(src);
}
