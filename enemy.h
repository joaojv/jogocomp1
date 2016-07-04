#ifndef ENEMY_H
#define ENEMY_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


typedef struct 
{
	int width;
	int height;
	SDL_Texture* texture;
	SDL_Texture* texture2;
	SDL_Texture* texture3;
	SDL_Texture* texture4;
	SDL_Rect position;
	SDL_Rect position2;
	SDL_Rect position3;
	SDL_Rect position4;
	SDL_Rect leagthpixel;
	int survave[5];
	float dirx;
	float diry;
	float speed;
	float movimentation_diagonally[4];
	int alive;
	int visible;

} STRUCT_ENEMY;

typedef struct
{
  float x, y, dy;
  short life;
  char *name;
  int currentSprite, walking , shooting, visible;
  SDL_Rect leagthpixel;
  int alive;
  
  SDL_Texture *sheetTexture;
} Man;

extern Man man;

void load_image_enemy(SDL_Renderer*, STRUCT_ENEMY *);
void draw_image_enemy(SDL_Renderer*, STRUCT_ENEMY * , int );
void iniciation_enemy(int , int, STRUCT_ENEMY *);
void movimentation_enemys(STRUCT_ENEMY * , int );

#endif
