/* Entete du programme
Nom: Space invaders
Auteur: Maxime Fontaine Bombardier
		Nathan Giraldeau
Date: 03/04/2013
Description du programme: Space invader en SDL

Directives au pré-processeur
=================================*/

#include "SDL_Utils.h"
#include <vector>

using namespace std;

/* Prototype
=================================*/



/* Programme principale
=================================*/

const int	LARGEUR_ALIEN=50,
			HAUTEUR_ALIEN=50,
			LARGEUR_HERO=50,
			HAUTEUR_HERO=50,
			LARGEUR_BLOC=10,
			HAUTEUR_BLOC=5;

const float	VITESSE_BALLE=1;

int main(int argc, char *argv[])
{
	
	// Variable SDL
	SDL_Surface *ecran = 0;
	SDL_Surface *spriteHero = 0;
	SDL_Surface *spriteAlien = 0;
	SDL_Surface *spriteBalleHero = 0;
	SDL_Surface *spriteBalleAlien = 0;

	

	SDL_Rect posHero = {200,450},
			 posAlien,
			 velAlien,
			 velHero,
			 posBalleHero;

	vector<SDL_Rect> posBalleAlien;

	// Variable standard
	bool	partieTerminee = false,
			droitTirer = true,
			toucheD=0,
			toucheG=0,
			toucheH=0;

	unsigned int	score = 0;

	int		vie = 3;

	unsigned long ticks;

	int Alien[HAUTEUR_BLOC][LARGEUR_BLOC];

	// Programme

	// Initialisation SDL
	SDL_Init(SDL_INIT_VIDEO);
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		cout << "Impossible d initialiser SDL !";
		return EXIT_FAILURE;
	}
	
	spriteHero = IMG_Load("SpaceInvaders.bmp");

	ecran = initEcran("Space Invaders", NULL, 400, 600, 0, 0, 0);
	while(partieTerminee == false)
	{
		SDL_Event event;
		SDL_PollEvent(&event);
		switch(event.type)
		{
		case SDL_QUIT:
			partieTerminee = true;
			break;
			case SDL_KEYDOWN:/* Si appui d'une touche on ferme la fenêtre*/
					switch (event.key.keysym.sym)
					{
					case SDLK_UP: case SDLK_SPACE:
							toucheH = 1;
							break;
						case SDLK_LEFT:
							toucheG = 1;
							break;
						case SDLK_RIGHT:
							toucheD = 1;
							break;
					}
					break;
				case SDL_KEYUP:/* Si retire d'une touche on ferme la fenêtre*/
					switch (event.key.keysym.sym)
					{
					case SDLK_UP: case SDLK_SPACE:
							toucheH = 0;
							break;
						case SDLK_LEFT:
							toucheG = 0;
							break;
						case SDLK_RIGHT:
							toucheD = 0;
							break;
					}
					break;
		}
		velHero.x = 0;

		
		if(toucheD)
			velHero.x = 1;
		if(toucheG)
			velHero.x = -1;
		posHero.x += velHero.x;
		

		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
		SDL_BlitSurface(spriteHero, NULL, ecran, &posHero);
		SDL_Flip(ecran);
		SDL_Delay(12);

	}
	SDL_FreeSurface(spriteHero);
	SDL_Quit();
	// Fermeture SDL
 	return EXIT_SUCCESS;
}


