#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "enemy.h"
Man man;

void iniciation_enemy(int Screen_width , int Screen_height, STRUCT_ENEMY *enemy)
{

	for(int i = 0 ; i < 5 ; i ++)
	{
		enemy->survave[i] = 0;
	}

	enemy->alive = 1;
	enemy->visible = 1;
	
	enemy->width = 448/7;
	enemy->height = 512/8;

	enemy->speed = 1.5;

	enemy->leagthpixel.x = 0;
	enemy->leagthpixel.y = 0;
	enemy->leagthpixel.w = enemy->width;
	enemy->leagthpixel.h = enemy->height;

	enemy->dirx = abs(Screen_width / 2);
	enemy->diry = abs(Screen_height / 2);

	enemy->position.x = Screen_width / 2;
	enemy->position.y = - 30;
	enemy->position.w = enemy->width;
	enemy->position.h = enemy->height;

}	

void load_image_enemy(SDL_Renderer* renderer, STRUCT_ENEMY *enemy)
{
	enemy->texture = IMG_LoadTexture(renderer,"monster.png");
}

void draw_image_enemy(SDL_Renderer* renderer, STRUCT_ENEMY *enemy , int frame)
{
	SDL_RenderCopy(renderer,enemy->texture,&enemy->leagthpixel,&enemy->position);
}

void movimentation_enemys(STRUCT_ENEMY *enemy , int frame)
{
	if(enemy->position.x < man.x)
	{
		enemy->position.x += enemy->speed;

		if(frame % 5 == 0)
	    	{
		       enemy->leagthpixel.x += enemy->width;
				enemy->leagthpixel.y = enemy->height  ;

				if(enemy->leagthpixel.x > enemy->width * 6)
					enemy->leagthpixel.x = 0;
			}
	}

	if(enemy->position.y < man.y)
	{
		enemy->position.y += enemy->speed;

		if(frame % 5 == 0)
	    	{
		       enemy->leagthpixel.x += enemy->width;
				enemy->leagthpixel.y = 0 ;

				if(enemy->leagthpixel.x > enemy->width * 6)
					enemy->leagthpixel.x = 0;
			}
	}

	if(enemy->position.x > man.x)
		enemy->position.x -= enemy->speed;

	if(enemy->position.y >man.y)
		enemy->position.y -= enemy->speed;

}	

