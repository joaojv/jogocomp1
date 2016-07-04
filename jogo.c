#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <sys/types.h>
#include "enemy.h"

#define MAX_BULLETS 1000

int Screen_width = 1300;
int Screen_height = 768;

STRUCT_ENEMY enemy[20];

typedef struct
{
  float x, y, dx , dy;
} Bullet;

typedef struct 
{
  int width;
  int height;
  SDL_Texture* texture;
  SDL_Rect position;
  SDL_Rect leagthpixel;
  float speed;
  int alive;
  int visible;

} STRUCT_BOSS;

SDL_Texture *bulletTexture;
SDL_Texture *backgroundTexture;
SDL_Window *window;               
SDL_Renderer *renderer; 
SDL_Texture* logo_mark = NULL;
SDL_Texture* gameover = NULL;
SDL_Texture* gamewin = NULL;
SDL_Texture* warning = NULL;
SDL_Texture* credits = NULL;
SDL_Texture* instructions = NULL;
SDL_Texture* menu = NULL;
SDL_Texture* cenario = NULL;
SDL_Surface* screen;
TTF_Font *fonte; 
SDL_Rect srcRect;
Bullet *bullets[MAX_BULLETS] = { NULL };
STRUCT_BOSS boss;
STRUCT_BOSS enemy2[20];
STRUCT_BOSS enemy3[20];

int running_intro = 1;
int running_game = 1;
int running_full = 1;
int return_menu = 0;
int quant_enemys = 0;
int delay_enemy = 40;
int enemy_death = 0;
int running_pause = 1;
int running_gameover = 1;
int running_gamewin = 1;
int contador = 0;
int quant_tiro = 0;
int ok = 0;

Mix_Chunk * menu_musica = NULL;
Mix_Music* jogo = NULL;
Mix_Chunk* tiro = NULL;

int globalTime = 0;

void load_image_iniciation();
void load_image_warning();
void load_image_credits();
void load_image_instructions();
void draw_image_iniciation();
void load_image_menu();
void draw_image_warning();
void draw_image_credits();
void draw_image_instructions();
void restart_game();
void draw_image_menu();
void keydown_menu();
void events_menu();
void events_game();
void load_image_gameover();
void draw_image_gameover();
void load_image_gamewin();
void draw_image_gamewin();
void movimentation_boss(int);
void enemy_total_movimentation(int);


void desenhaTexto(char* texto, SDL_Renderer* dst, int x, int y, SDL_Color cor)
{
    SDL_Surface* src = TTF_RenderText_Blended(fonte, texto, cor);
    SDL_Texture *texturee = SDL_CreateTextureFromSurface(renderer, src);
    SDL_FreeSurface(src);
    SDL_Rect dst_rect = {x, y, 0, 0};

    SDL_RenderCopy(dst,texturee,&dst_rect,NULL);
}


void escrever_recordes()//escrever record no menu
{
    FILE* fp = fopen("score.txt", "r");
    if(fp == NULL)
    {
      printf("não foi possivel abrir o arquivo");
      //exit(1);
    }

    SDL_Color cor = {255,255,255};
    fonte = TTF_OpenFont("Fontes/Jazz_Ball_Regular.ttf",20);
    char nome_1[11],nome_2[11],nome_3[11],nome_4[11],nome_5[11];
    char pont_1[6],pont_2[6],pont_3[6],pont_4[6];
    float score_1,score_2,score_3,score_4,aux;

    fscanf(fp,"%s", nome_1);
    fscanf(fp,"%f", &score_1);
    fscanf(fp,"%s", nome_2);
    fscanf(fp,"%f", &score_2);
    fscanf(fp,"%s", nome_3);
    fscanf(fp,"%f", &score_3);
    //fscanf(fp,"%s", nome_4);
    //fscanf(fp,"%f", &score_4);

    sprintf(pont_1,"%.0f", score_1);
    sprintf(pont_2,"%.0f", score_2);
    sprintf(pont_3,"%.0f", score_3);
    //sprintf(pont_4,"%.0f", score_4);

    desenhaTexto(nome_1, renderer, 275, 247, cor);
    desenhaTexto(pont_1, renderer, 470, 247, cor);
    desenhaTexto(nome_2, renderer, 275, 338, cor);
    desenhaTexto(pont_2, renderer, 470, 338, cor);
    desenhaTexto(nome_3, renderer, 275, 429, cor);
    desenhaTexto(pont_3, renderer, 470, 429, cor);
    //desenhaTexto(nome_4, screen, 180, 410, cor);
    //desenhaTexto(pont_4, screen, 300, 410, cor);
    //desenhaTexto(nome_1, screen, 180, 10, cor);
    //desenhaTexto(nome_1, screen, 180, 10, cor);

    //printf("Nome : %s pontuacao: %s\n", nome_1,score_1);
    //printf("Nome : %s pontuacao: %s\n", nome_2,score_2);
    //printf("Nome : %s pontuacao: %s\n", nome_3,score_3);
    //printf("Nome : %s pontuacao: %s\n", nome_4,score_4);
    TTF_CloseFont(fonte);
    fclose(fp);
}

void addBullet(float x, float y, float dx , float dy)
{
  int found = -1;
  for(int i = 0; i < MAX_BULLETS; i++)
  {
    if(bullets[i] == NULL)
    {
      found = i;
      break;
    }
  }
    
  if(found >= 0)
  {
    int i = found;
    bullets[i] = malloc(sizeof(Bullet));
    bullets[i]->x = x;
    bullets[i]->y = y;
    bullets[i]->dx = dx;   
    bullets[i]->dy = dy; 
  }
}

void removeBullet(int i)
{
  if(bullets[i])
  {
    free(bullets[i]);
    bullets[i] = NULL;
  }
}

int Events_Moving_player(SDL_Window *window, Man *man)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  
    if(state[SDL_SCANCODE_LEFT])
    {
      man->x -= 5;
      man->walking = 1;
      
      if(man->shooting == 0)
      {
      
        if(globalTime % 6 == 0)
        {
          srcRect.x += 87;
          srcRect.y = 82;

          if(srcRect.x > 87 * 3)
            srcRect.x = 0;
        }  
      }
      else
      {
        srcRect.x += 87;
        if(srcRect.x > 87 * 3)
            srcRect.x = 0;
      }
    }
    if(state[SDL_SCANCODE_RIGHT])
    {
      man->x += 5;
      man->walking = 1;
     
      if(man->shooting == 0)
      {
        if(globalTime % 6 == 0)
        {
          srcRect.x += 87;
          srcRect.y = 0;

          if(srcRect.x > 87 * 3)
            srcRect.x = 0;
        }   
      }

      else
      {
        srcRect.x += 87;
        if(srcRect.x > 87 * 3)
            srcRect.x = 0;
      }
    }

    if(state[SDL_SCANCODE_UP])
    {
      man->y -= 5;
      man->walking = 1;
      
      if(man->shooting == 0)
      {
        if(globalTime % 6 == 0)
        {
          srcRect.x += 87;
          srcRect.y = 154;

          if(srcRect.x > 87 * 3)
            srcRect.x = 0;
        }  
      }
      else
      {
        srcRect.x += 87;
        if(srcRect.x > 87 * 3)
            srcRect.x = 0;
      }

    }
    if(state[SDL_SCANCODE_DOWN])
    {
      man->y += 5;
      man->walking = 1;
     
      if(man->shooting == 0)
      {
        if(globalTime % 6 == 0)
        {
          srcRect.x += 87;
          srcRect.y = 256;

          if(srcRect.x > 87 * 3)
            srcRect.x = 0;
        }  
      }

      else
      {
        srcRect.x += 87;
        if(srcRect.x > 87 * 3)
            srcRect.x = 0;
      }
    }
    else
    {
      man->walking = 0;
      man->currentSprite = 4;
    }
}

int Events_shotting(SDL_Window *window , Man *man)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if(state[SDL_SCANCODE_W])
    {
       srcRect.y = 154;
        Mix_PlayChannel(1, tiro , 0);

      if(globalTime % 6 == 0)
      {
        if(man->currentSprite == 4)         
          man->currentSprite = 5;
        else
          man->currentSprite = 4;
          
          addBullet(man->x+35, man->y+20, 0 , -10); 
    
      }  
  
      man->shooting = 1;
    }
    else if(state[SDL_SCANCODE_S])
    {
      
      srcRect.y = 256;
      Mix_PlayChannel(1, tiro , 0);

      if(globalTime % 6 == 0)
      {
        if(man->currentSprite == 4)         
          man->currentSprite = 5;
        else
          man->currentSprite = 4;
          
          addBullet(man->x+35, man->y+20, 0 , 10); 
    
      }  
  
      man->shooting = 1;
    }
    else if(state[SDL_SCANCODE_A])
    {
       srcRect.y = 82;
       Mix_PlayChannel(1, tiro , 0);

      if(globalTime % 6 == 0)
      {
        if(man->currentSprite == 4)         
          man->currentSprite = 5;
        else
          man->currentSprite = 4;
          
          addBullet(man->x+35, man->y+20, -10 , 0); 
    
      }  
  
      man->shooting = 1;
    }
    else if(state[SDL_SCANCODE_D])
    {
        srcRect.y = 0;
        Mix_PlayChannel(1, tiro , 0);

      if(globalTime % 6 == 0)
      {
        if(man->currentSprite == 4)         
          man->currentSprite = 5;
        else
          man->currentSprite = 4;
          
          addBullet(man->x+35, man->y+20, 10 , 0); 
    
      }  
  
      man->shooting = 1;
    }
    else 
    {
      man->currentSprite = 4;  
      man->shooting = 0;
    }

}

void position_of_iniciation()
{
  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.w = 87;
  srcRect.h = 88 ;  
}

void doRender(SDL_Renderer *renderer, Man *man , int frame)
{
  // SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  
  SDL_RenderClear(renderer);
  
  // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
   
  SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    SDL_Rect rect = { man->x, man->y, 87, 88 };  
    SDL_RenderCopy(renderer, man->sheetTexture, &srcRect, &rect);


  for(int i = 0; i < MAX_BULLETS; i++) if(bullets[i])
  {
    SDL_Rect rect = { bullets[i]->x, bullets[i]->y, 8, 8 };  
    SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
  }

  for(int i = 0 ; i < quant_enemys ; i ++ )
  { 
    if(enemy[i].visible == 1)
    draw_image_enemy(renderer, &enemy[i] , frame);
	
	if(enemy2[i].visible == 1)
	SDL_RenderCopy(renderer,enemy2[i].texture,&enemy2[i].leagthpixel,&enemy2[i].position);
	
	if(enemy3[i].visible == 1)
	SDL_RenderCopy(renderer,enemy3[i].texture,&enemy3[i].leagthpixel,&enemy3[i].position);
  }

  if(contador == 3)
    SDL_RenderCopy(renderer,boss.texture,&boss.leagthpixel,&boss.position);
  
  SDL_RenderPresent(renderer);
}

void updateLogic(Man *man)
{
  
  for(int i = 0; i < MAX_BULLETS; i++) if(bullets[i])
  {
    bullets[i]->x += bullets[i]->dx;
    bullets[i]->y += bullets[i]->dy;

    
    for(int j = 0 ; j < quant_enemys ; j ++)
    {
      if(bullets[i]->x > enemy[j].position.x && bullets[i]->x < enemy[j].position.x + 448/7 && 
         bullets[i]->y > enemy[j].position.y && bullets[i]->y < enemy[j].position.y + 512/8)
      {
	      enemy[j].alive = 0;
      }

 	  if(bullets[i]->x > enemy2[j].position.x && bullets[i]->x < enemy2[j].position.x + 448/7 && 
         bullets[i]->y > enemy2[j].position.y && bullets[i]->y < enemy2[j].position.y + 512/8)
      {
	      enemy2[j].alive = 0;
      }

      if(bullets[i]->x > enemy3[j].position.x && bullets[i]->x < enemy3[j].position.x + 448/7 && 
         bullets[i]->y > enemy3[j].position.y && bullets[i]->y < enemy3[j].position.y + 512/8)
      {
	      enemy3[j].alive = 0;
      }
      // if(bullets[i]->x > enemy[j].position2.x && bullets[i]->x < enemy[j].position2.x+40 && 
      //    bullets[i]->y > enemy[j].position2.y && bullets[i]->y < enemy[j].position2.y +50)
      // {
      //    enemy[j].alive = 0;
      // } 

      // if(bullets[i]->x > enemy[j].position3.x && bullets[i]->x < enemy[j].position3.x+40 && 
      //    bullets[i]->y > enemy[j].position3.y && bullets[i]->y < enemy[j].position3.y +50)
      // {
      //    enemy[j].alive = 0;
      // } 

      // if(bullets[i]->x > enemy[j].position4.x && bullets[i]->x < enemy[j].position4.x+40 && 
      //    bullets[i]->y > enemy[j].position4.y && bullets[i]->y < enemy[j].position4.y +50)
      // {
      //     enemy[j].alive = 0;
      // } 
    }

    if(bullets[i]->x > boss.position.x && bullets[i]->x < boss.position.x + 40 && 
         bullets[i]->y > boss.position.y && bullets[i]->y < boss.position.y + 50)
      {
        quant_tiro ++;
      }
    
    if(bullets[i]->x < -1000 || bullets[i]->x > 1000)
      removeBullet(i);
  }

  for(int j = 0 ; j < quant_enemys ; j ++)
  {

  if(enemy[j].alive == 0 && globalTime % 6 == 0)
  {
      enemy[j].visible = 0;
      enemy_death ++;
  }

   if(enemy2[j].alive == 0 && globalTime % 6 == 0)
  {
      enemy2[j].visible = 0;
      enemy_death ++;
  }

   if(enemy3[j].alive == 0 && globalTime % 6 == 0)
  {
      enemy3[j].visible = 0;
      enemy_death ++;
  }

}
  
  
  globalTime++;
 // colisao do player com a screen

  if(man->x < 0)
    man->x = 0;

  if(man->x + 87 > Screen_width)
    man->x = Screen_width - 87;

  if(man->y < 0)
    man->y = 0;

  if(man->y + 88 >Screen_height)
    man->y = Screen_height- 88;

 //colisao do player com o enemy
  for(int i = 0 ; i < quant_enemys ; i ++ )
  {
   	if(man->x == enemy[i].position.x && man->y == enemy[i].position.y || man->x == enemy2[i].position.x && man->y == enemy2[i].position.y || man->x == enemy3[i].position.x && man->y == enemy3[i].position.y )
    {

      SDL_Delay(30);

      while(running_gameover)
      {
        unsigned frame2 = SDL_GetTicks();
        running_game = 0;
        for(int j = 0 ; j < 400 ; j ++ )
           draw_image_gameover();

        return_menu = 1;

        break;

        if (SDL_GetTicks() - frame2 < 1000 / 60)
        {
          SDL_Delay(1000 / 60 - (SDL_GetTicks() - frame2));
        }
      }
    }
}	
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	if(state[SDL_SCANCODE_P])

		running_pause = 0;

    if(running_pause == 0)
		{
			draw_image_warning();

			const Uint8 *state2 = SDL_GetKeyboardState(NULL);

			if(state2[SDL_SCANCODE_R])
				running_pause = 1;
		}


    if(quant_tiro >= 200)
  {
    SDL_Delay(30);

    while(running_gamewin)
      {
        unsigned frame2 = SDL_GetTicks();
        running_game = 0;
        for(int j = 0 ; j < 400 ; j ++ )
           draw_image_gamewin();

        return_menu = 1;

        break;

        if (SDL_GetTicks() - frame2 < 1000 / 60)
        {
          SDL_Delay(1000 / 60 - (SDL_GetTicks() - frame2));
        }
      }
  }
}

void iniciation_enemy_total(int quant_enemys)
{
  enemy2[quant_enemys].alive = 1;
  enemy2[quant_enemys].visible = 1;
  
  enemy2[quant_enemys].width = 448/7;
  enemy2[quant_enemys].height = 512/8;

  enemy2[quant_enemys].speed = 1.5;

  enemy2[quant_enemys].leagthpixel.x = 0;
  enemy2[quant_enemys].leagthpixel.y = 0;
  enemy2[quant_enemys].leagthpixel.w = enemy2[quant_enemys].width;
  enemy2[quant_enemys].leagthpixel.h = enemy2[quant_enemys].height;

  enemy2[quant_enemys].position.x = -30;
  enemy2[quant_enemys].position.y = Screen_height / 2;
  enemy2[quant_enemys].position.w = enemy2[quant_enemys].width;
  enemy2[quant_enemys].position.h = enemy2[quant_enemys].height;


  enemy3[quant_enemys].alive = 1;
  enemy3[quant_enemys].visible = 1;
  
  enemy3[quant_enemys].width = 448/7;
  enemy3[quant_enemys].height = 512/8;

  enemy3[quant_enemys].speed = 1.5;

  enemy3[quant_enemys].leagthpixel.x = 0;
  enemy3[quant_enemys].leagthpixel.y = 0;
  enemy3[quant_enemys].leagthpixel.w = enemy3[quant_enemys].width;
  enemy3[quant_enemys].leagthpixel.h = enemy3[quant_enemys].height;

  enemy3[quant_enemys].position.x = Screen_width / 2;
  enemy3[quant_enemys].position.y = Screen_height;
  enemy3[quant_enemys].position.w = enemy3[quant_enemys].width;
  enemy3[quant_enemys].position.h = enemy3[quant_enemys].height;
}

int main(int argc, char *argv[])
{ 
  SDL_Init(SDL_INIT_EVERYTHING);              

  man.x = Screen_width / 2;
  man.y = Screen_height / 2;
  man.currentSprite = 4;  
  man.alive = 1;
  man.visible = 1;
  
  window = SDL_CreateWindow("Blood Reaction",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,Screen_width,Screen_height,SDL_WINDOW_RESIZABLE);

  renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);

  SDL_RenderSetLogicalSize(renderer, 1300, 768);
  
  SDL_Surface *sheet = IMG_Load("player.png");
  if(!sheet)
  {
    printf("Cannot find sheet\n");
    return 1;
  }
  
  man.sheetTexture = SDL_CreateTextureFromSurface(renderer, sheet);  
  SDL_FreeSurface(sheet);
  
 
  SDL_Surface *bg = IMG_Load("cenario.png");
  
  if(!sheet)
  {
    printf("Cannot find background\n");
    return 1;
  }

   backgroundTexture = SDL_CreateTextureFromSurface(renderer, bg);
   SDL_FreeSurface(bg);

  SDL_Surface *bullet = IMG_Load("bullet.png");
  
  if(!bullet)
  {
    printf("Cannot find bullet\n");
    return 1;
  }

  bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
  SDL_FreeSurface(bullet);

  boss.alive = 1;
  boss.visible = 1;
  
  boss.width = 192/3;
  boss.height = 256/4;

  boss.speed = 1.5;

  boss.leagthpixel.x = 0;
  boss.leagthpixel.y = 0;
  boss.leagthpixel.w = boss.width;
  boss.leagthpixel.h = boss.height;

  boss.position.x = Screen_width / 2;
  boss.position.y = - 30;
  boss.position.w = boss.width;
  boss.position.h = boss.height;

  boss.texture = IMG_LoadTexture(renderer,"boss.png");

  for(int i = 0 ; i < 10 ; i ++)
  {
	  enemy2[i].texture = IMG_LoadTexture(renderer , "monster.png");
	  enemy3[i].texture = IMG_LoadTexture(renderer , "monster.png");
}

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {

        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
        Mix_GetError());

        return 0;
    }

    menu_musica = Mix_LoadWAV("jogando.wav");
    jogo  = Mix_LoadMUS("play hard.mp3");
    tiro = Mix_LoadWAV("tiro.wav");

  load_image_iniciation();
  load_image_warning();
  load_image_menu();
  load_image_credits();
  load_image_instructions();
  load_image_gameover();
  load_image_gamewin();

  for(int i = 0 ; i < 10 ; i ++ )
  {
    load_image_enemy(renderer, &enemy[i]);
  }

  int done = 0;

  position_of_iniciation();
  
  while(running_intro)
  {
    unsigned frame2 = SDL_GetTicks();

    for(int j = 0 ; j < 400 ; j ++ )
      draw_image_iniciation();

    for(int j = 0 ; j < 400 ; j ++ )
      draw_image_warning();

    break;

    if (SDL_GetTicks() - frame2 < 1000 / 60)
    {
      SDL_Delay(1000 / 60 - (SDL_GetTicks() - frame2));
    }
  }

  Mix_PlayChannel(-1, menu_musica , -1);

  while(running_full)
  {

    draw_image_menu();

    events_menu();

    keydown_menu();

    if(return_menu == 1)
    {
      running_game = 1;
      return_menu = 0;
     restart_game();
    }
  } 

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  
  SDL_Quit();

  return 0;
}

void restart_game()
{
  man.x = Screen_width / 2;
  man.y = Screen_height / 2;
  man.currentSprite = 4;  
  man.alive = 1;
  man.visible = 1;  
  quant_enemys = 0;
  contador = 0;

  boss.alive = 1;
  boss.visible = 1;
  
  boss.width = 448/7;
  boss.height = 512/8;

  boss.speed = 1.5;

  boss.leagthpixel.x = 0;
  boss.leagthpixel.y = 0;
  boss.leagthpixel.w = boss.width;
  boss.leagthpixel.h = boss.height;

  boss.position.x = Screen_width / 2;
  boss.position.y = - 30;
  boss.position.w = boss.width;
  boss.position.h = boss.height;

  boss.texture = IMG_LoadTexture(renderer,"boss.png");

 for(int i = 0 ; i < 10 ; i ++)
  {
	  enemy2[i].texture = IMG_LoadTexture(renderer , "monster.png");
	  enemy3[i].texture = IMG_LoadTexture(renderer , "monster.png");
}

  SDL_Surface *sheet = IMG_Load("player.png");
  if(!sheet)
  {
    printf("Cannot find sheet\n");
    
  }
  
  man.sheetTexture = SDL_CreateTextureFromSurface(renderer, sheet);  
  SDL_FreeSurface(sheet);
  
 
  SDL_Surface *bg = IMG_Load("cenario.png");
  
  if(!sheet)
  {
    printf("Cannot find background\n");
    
  }

   backgroundTexture = SDL_CreateTextureFromSurface(renderer, bg);
   SDL_FreeSurface(bg);

  SDL_Surface *bullet = IMG_Load("bullet.png");
  
  if(!bullet)
  {
    printf("Cannot find bullet\n");
    
  }

  bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
  SDL_FreeSurface(bullet);

}

void load_image_iniciation()
{
  logo_mark = IMG_LoadTexture(renderer,"logo.png");

  if(logo_mark == NULL)
    {
      printf("failed in create load_mark : %s\n",SDL_GetError());
    }

}

void load_image_gameover()
{
  gameover = IMG_LoadTexture(renderer,"gameover.png");

  if(gameover == NULL)
    {
      printf("failed in create gameover : %s\n",SDL_GetError());
    }

}

void load_image_gamewin()
{
  gamewin = IMG_LoadTexture(renderer,"gamewin.png");

  if(gamewin == NULL)
    {
      printf("failed in create gamewin : %s\n",SDL_GetError());
    }

}

void load_image_warning()
{
  warning = IMG_LoadTexture(renderer,"warning.png");

  if(warning == NULL)
    {
      printf("failed in create warning : %s\n",SDL_GetError());
    }

}

void load_image_credits()
{
  credits = IMG_LoadTexture(renderer,"credits.png");

  if(credits == NULL)
    {
      printf("failed in create credits: %s\n",SDL_GetError());
    }

}

void load_image_instructions()
{
  instructions = IMG_LoadTexture(renderer,"instructions.png");

  if(credits == NULL)
    {
      printf("failed in create instructions: %s\n",SDL_GetError());
    }

}

void load_image_menu()
{
  menu = IMG_LoadTexture(renderer,"menu.png");

  if(menu == NULL)
    {
      printf("failed in create menu : %s\n",SDL_GetError());
    }

}

void draw_image_iniciation()
{

  SDL_RenderClear(renderer);

  SDL_RenderCopy(renderer,logo_mark,NULL,NULL);
  SDL_RenderPresent(renderer);
}

void draw_image_gamewin()
{

  SDL_RenderClear(renderer);

  SDL_RenderCopy(renderer,gamewin,NULL,NULL);
  SDL_RenderPresent(renderer);
}

void draw_image_gameover()
{

  SDL_RenderClear(renderer);

  SDL_RenderCopy(renderer,gameover,NULL,NULL);
  SDL_RenderPresent(renderer);
}

void draw_image_warning()
{

  SDL_RenderClear(renderer);

  SDL_RenderCopy(renderer,warning,NULL,NULL);
  SDL_RenderPresent(renderer);
}

void draw_image_credits()
{

  SDL_RenderClear(renderer);

  SDL_RenderCopy(renderer,credits,NULL,NULL);
  SDL_RenderPresent(renderer);
}

void draw_image_instructions()
{

  SDL_RenderClear(renderer);

  SDL_RenderCopy(renderer,instructions,NULL,NULL);
  SDL_RenderPresent(renderer);
}

void draw_image_menu()
{

  SDL_RenderClear(renderer);

  SDL_RenderCopy(renderer,menu,NULL,NULL);

  SDL_RenderPresent(renderer);
}

void boss_movimentation(int frame)
{
  if(boss.position.x < man.x)
  {
    boss.position.x += boss.speed;

    if(frame % 5 == 0)
        {
           boss.leagthpixel.x += boss.width;
        boss.leagthpixel.y = boss.height  ;

        if(boss.leagthpixel.x > boss.width * 6)
          boss.leagthpixel.x = 0;
      }
  }

  if(boss.position.y < man.y)
  {
    boss.position.y += boss.speed;

    if(frame % 5 == 0)
        {
           boss.leagthpixel.x += boss.width;
        boss.leagthpixel.y = 0 ;

        if(boss.leagthpixel.x > boss.width * 6)
          boss.leagthpixel.x = 0;
      }
  }

  if(boss.position.x > man.x)
    boss.position.x -= boss.speed;

  if(boss.position.y >man.y)
    boss.position.y -= boss.speed;
}

void enemy_total_movimentation(int frame)
{
for(int i =0 ; i < quant_enemys ; i ++)
{
  if(enemy2[i].position.x < man.x)
  {
    enemy2[i].position.x += enemy2[i].speed;

    if(frame % 5 == 0)
        {
           enemy2[i].leagthpixel.x += enemy2[i].width;
        enemy2[i].leagthpixel.y = enemy2[i].height  ;

        if(enemy2[i].leagthpixel.x > enemy2[i].width * 6)
          enemy2[i].leagthpixel.x = 0;
      }
  }

  if(enemy2[i].position.y < man.y)
  {
    enemy2[i].position.y += enemy2[i].speed;

    if(frame % 5 == 0)
        {
           enemy2[i].leagthpixel.x += enemy2[i].width;
        enemy2[i].leagthpixel.y = 0 ;

        if(enemy2[i].leagthpixel.x > enemy2[i].width * 6)
          enemy2[i].leagthpixel.x = 0;
      }
  }

  if(enemy2[i].position.x > man.x)
    enemy2[i].position.x -= enemy3[i].speed;
  if(enemy2[i].position.y >man.y)
    enemy2[i].position.y -= enemy2[i].speed;

if(enemy3[i].position.x < man.x)
  {
    enemy3[i].position.x += enemy3[i].speed;

    if(frame % 5 == 0)
        {
           enemy3[i].leagthpixel.x += enemy3[i].width;
        enemy3[i].leagthpixel.y = enemy3[i].height  ;

        if(enemy3[i].leagthpixel.x > enemy3[i].width * 6)
          enemy3[i].leagthpixel.x = 0;
      }
  }

  if(enemy3[i].position.y < man.y)
  {
    enemy3[i].position.y += enemy3[i].speed;

    if(frame % 5 == 0)
        {
           enemy3[i].leagthpixel.x += enemy3[i].width;
        enemy3[i].leagthpixel.y = 0 ;

        if(enemy3[i].leagthpixel.x > enemy3[i].width * 6)
          enemy3[i].leagthpixel.x = 0;
      }
  }

  if(enemy3[i].position.x > man.x)
    enemy3[i].position.x -= enemy3[i].speed;
  if(enemy3[i].position.y >man.y)
    enemy3[i].position.y -= enemy3[i].speed;
}
}

void keydown_menu()
{
  const Uint8* currentKeyStates3 = SDL_GetKeyboardState( NULL );

  if( currentKeyStates3[ SDL_SCANCODE_1 ] )
  {
    
    while(running_game)
    {

      unsigned frame = SDL_GetTicks();

      Events_Moving_player(window, &man);
      
      Events_shotting(window,&man);

      updateLogic(&man );

      doRender(renderer, &man , frame);

      if(quant_enemys != 4)
      {
          if(delay_enemy == 0)
          {
            iniciation_enemy(Screen_width , Screen_height, &enemy[quant_enemys]);
            iniciation_enemy_total(quant_enemys);
            quant_enemys ++;
            delay_enemy = 80;
          } 
      }

      if(contador != 3)
      {

        for(int i = 0 ; i < 800 ; i ++);

       if(quant_enemys == 4)
       {
            quant_enemys = 0;
            contador ++;

          for(int i = 0 ; i < 4 ; i ++ )
          {
            enemy[i].visible = 0;
            enemy[i].position.x = Screen_width / 2;
            enemy[i].position.y = - 30;
            enemy[i].position.w = enemy->width;
            enemy[i].position.h = enemy->height;
            iniciation_enemy_total(i);
          }

          for(int j = 0 ; j < 400 ; j ++);
        }
      }

      if(contador == 3)
      {
        boss_movimentation(frame);
      }

      enemy_total_movimentation(frame);

      for(int i = 0 ; i < quant_enemys ; i ++ )
      {
          movimentation_enemys(&enemy[i] , frame);
      }

      delay_enemy --;

      events_game();
      
      if (SDL_GetTicks() - frame < 1000 / 60)
      {
        SDL_Delay(1000 / 60 - (SDL_GetTicks() - frame));
      }
    }

    SDL_DestroyTexture(man.sheetTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(bulletTexture);
  
    for(int i = 0; i < MAX_BULLETS; i++)
      removeBullet(i);
  }

  if( currentKeyStates3[ SDL_SCANCODE_2 ] )
  {
    escrever_recordes();
  }

  if( currentKeyStates3[ SDL_SCANCODE_3 ] )
  {
    while(running_game)
    {
      draw_image_instructions();
      events_game();
    }
}

  if( currentKeyStates3[ SDL_SCANCODE_4 ] )
  { 
    while(running_game)
    {
      draw_image_credits();
      events_game();
    }
  }

  if( currentKeyStates3[ SDL_SCANCODE_5 ] )
  { 
    running_full = 0;
  }
}

void events_menu(void)
{
  SDL_Event event2;

  while(SDL_PollEvent(&event2) != 0)
  {
    if(event2.type == SDL_QUIT)
        running_full = 0;
  } 
}

void events_game(void)
{
  SDL_Event event;

  while(SDL_PollEvent(&event) != 0)
  {
    if(event.type == SDL_QUIT)
    {
      running_full = 0;
      running_game = 0;
    }
  } 

  const Uint8* currentKeyStates4= SDL_GetKeyboardState( NULL );

  if( currentKeyStates4[ SDL_SCANCODE_ESCAPE] )
  {
    running_game = 0;
    return_menu = 1;
  }
} 