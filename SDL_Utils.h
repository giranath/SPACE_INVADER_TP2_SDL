#ifndef DEF_SDL_UTILS
#define DEF_SDL_UTILS

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
// Implémentation =======================================================================================================

//void initSDL()
//{
//	if(SDL_Init(SDL_INIT_VIDEO) == -1)	//evenement
//	{
//		cout << "Erreur lors de l'initialisation de la SDL", SDL_GetError();
//		exit(EXIT_FAILURE);
//	}
//}

//Chargement de l'écran principal
SDL_Surface* initEcran(char titre[], char icone[], int largeur, int hauteur, int rouge, int vert, int bleu)
{
	SDL_Surface *ecran;
	SDL_WM_SetCaption(titre,NULL); //titre de la fenetre
	//Chargement de l'icône AVANT SDL_SetVideoMode, copier le bmp ds le même dossier que .ccp
	
	if(icone != NULL)
		SDL_WM_SetIcon(SDL_LoadBMP(icone), NULL);

	ecran = SDL_SetVideoMode(largeur,hauteur,32,SDL_HWSURFACE | SDL_RESIZABLE| SDL_DOUBLEBUF);  //la surface de écran
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, rouge, vert, bleu));
	return ecran;
}


void initCharset(SDL_Rect posImage[8][5], int ligne, int colonne, int w, int h)
{ 
 for(int i=0;i<ligne;i++) 
  for(int j=0; j<colonne; j++)
  {
  posImage[i][j].x = j*w;
  posImage[i][j].y = i*h;
  posImage[i][j].w = w;
  posImage[i][j].h = h;
  }
}

SDL_Rect initPosition(int x, int y)
{
	SDL_Rect pos;

	pos.x = x;
	pos.y = y;

	return pos;
}

//Fonction remplissant l'écran 
void dessinerImage(SDL_Surface* image, SDL_Rect posImage, SDL_Surface* ecran, SDL_Rect posBonhomme)
{
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));		//l'endroit et quel couleur
	SDL_BlitSurface(image, &posImage, ecran, &posBonhomme);	//Remplir la surface
	SDL_Flip(ecran);						//mettre à jour l'écran avec les nouvelles données
}

#endif