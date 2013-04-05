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

const int	LARGEUR_ALIEN=50,         // La largeur en pixel d'un alien
			HAUTEUR_ALIEN=50,         // La hauteur en pixel d'un alien
			LARGEUR_HERO=50,          // La largeur en pixel du héro
			HAUTEUR_HERO=50,          // La hauteur en pixel du héro
			LARGEUR_BLOC=10,          // Le nombre de colone d'alien dans le bloc
			HAUTEUR_BLOC=5;           // Le nombre de ligne d'alien dans le bloc

const float	VITESSE_BALLE=1;          // La vitesse d'une balle

int main(int argc, char *argv[])
{
	
	/*************** Variable SDL *********************/
	// Les différentes surfaces du programme
	SDL_Surface *ecran = 0;            // L'écran
	SDL_Surface *spriteHero = 0;       // Le sprite du héro
	SDL_Surface *spriteAlien = 0;      // Le sprite d'un alien
	SDL_Surface *spriteBalleHero = 0;  // Le sprite d'une balle du héro
	SDL_Surface *spriteBalleAlien = 0; // Le sprite d'une balle alien

	
    // Les rectangles
	SDL_Rect posHero = {200,450},      // Vecteur de la position du héro
			 posAlien,                 // Vecteur de la position du bloc d'alien
			 velAlien,                 // La vélocité du bloc d'alien
			 velHero,                  // La vélocité du héro
			 posBalleHero;             // La position de la balle unique du héro

	vector<SDL_Rect> posBalleAlien;    // Un tableau dynamique des balles aliens

	/*************** Variable Standards *********************/
	bool	partieTerminee = false,    // La partie est terminée
			droitTirer = true,         // Le héro à le droit de tirer
			toucheD=0,                 // La touche Droite est appuyée
			toucheG=0,                 // La touche Gauche est appuyée
			toucheH=0;                 // La touche Haut ou même espace est appuyée

	unsigned int	score = 0;         // Le score de la présente partie

	int		vie = 3;                   // Le nombre de vie du joueur

	unsigned long ticks;               // Le nombre de milisecondes écoulées depuis le lancement du programme

	int Alien[HAUTEUR_BLOC][LARGEUR_BLOC]; // Matrice contenant les informations du bloc d'aliens

	/*************** PROGRAMME *********************/
	// Initialisation SDL
	if(SDL_Init(SDL_INIT_VIDEO) == -1) // Si elle échoue…
	{
		cout << "Impossible d initialiser SDL !"; // On averti l'utilisateur et on quitte
		return EXIT_FAILURE;
	}
	
	// On charge l'image du héro à l'aide de la bibliothèque SDL_Images
	spriteHero = IMG_Load("SpaceInvaders.bmp");

    // On crée l'écran à l'aide de la fonction initEcran
	ecran = initEcran("Space Invaders", NULL, 400, 600, 0, 0, 0);
	
	/*************** BOUCLE PRINCIPALE *********************/
	while(partieTerminee == false)         // Tant que la partie n'est pas terminée...
	{
		SDL_Event event;                  // On récupère l'évènement de la fenêtre sans bloquer le programme
		SDL_PollEvent(&event);            
		switch(event.type)
		{
		case SDL_QUIT:                    // Si on appui sur le bouton de fermeture de la fenêtre...
			partieTerminee = true;         // On quitte la boucle
			break;
			case SDL_KEYDOWN:/* Si appui d'une touche, on sauvegarde son état dans le booleen correspondant*/
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
				case SDL_KEYUP:/* Si retire d'une touche, on sauvegarde son état dans le booleen correspondant*/
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
		
		// On réinitialise la velocité du héro à 0
		velHero.x = 0;

        // Si on appui sur la touche de droite...		
		if(toucheD)
			velHero.x = 1;   // La vélocité du héro est 1 sur son axe x
		if(toucheG)
			velHero.x = -1; 
			
		// On déplace le héro en fonction de sa vélocité
		posHero.x += velHero.x;
		

        // On remplit l'écran de noir
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
		
		// On déssine le héro
		SDL_BlitSurface(spriteHero, NULL, ecran, &posHero);
		
		// On change les buffers
		SDL_Flip(ecran);
		
		// On endort le programme 12 ms
		SDL_Delay(12);

	}
	
	// On libère la mémoire de la surface
	SDL_FreeSurface(spriteHero);
	
	// On ferme la SDL
	SDL_Quit();
	
	// On quitte
 	return EXIT_SUCCESS;
}


