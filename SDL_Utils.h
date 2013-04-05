#ifndef DEF_SDL_UTILS
#define DEF_SDL_UTILS

// Uniquement pour ne pas avoir a ce soucier s'il s'agit d'un mac ou d'un windows lors de la compilation
#ifdef __APPLE__    // S'il s'agit d'un mac, on inclut "SDL/SDL.h" et "SDL_image/SDL_image.h"
    #include <SDL/SDL.h>    
    #include <SDL_image/SDL_image.h>
#else               // Sinon, on inclut "SDL.h" et "SDL_image.h"
    #include <SDL.h>
    #include <SDL_image.h>
#endif

#include <iostream>
// Implementation =======================================================================================================


//Chargement de l'ecran principal
SDL_Surface* initEcran(char titre[], char icone[], int largeur, int hauteur, int rouge, int vert, int bleu)
{
	SDL_Surface *ecran;
	SDL_WM_SetCaption(titre,NULL); //titre de la fenetre
	//Chargement de l'icone AVANT SDL_SetVideoMode, copier le bmp dans le meme dossier que .cpp
	
	if(icone != NULL)
		SDL_WM_SetIcon(SDL_LoadBMP(icone), NULL);

	ecran = SDL_SetVideoMode(largeur,hauteur,32,SDL_HWSURFACE | SDL_DOUBLEBUF);  //la surface de l'ecran
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, rouge, vert, bleu));
	return ecran;
}

SDL_Rect initPosition(int x, int y)
{
	SDL_Rect pos;

	pos.x = x;
	pos.y = y;

	return pos;
}

#endif