/* Entete du programme
Nom: Space invaders
Auteur: Maxime Fontaine Bombardier
		Nathan Giraldeau
Date: 03/04/2013
Description du programme: Space invader en SDL

Directives au pre-processeur
=================================*/

#include "SDL_Utils.h"
#include <vector>

using namespace std;

/*Structures
 ================================*/

typedef SDL_Rect Vecteur2f;

// Structure pour définir un objet dans le jeu
struct Entite {
    bool vivant;            // L'entite est-il vivant?
    int vie;                // Son nombre de vie
    
    Vecteur2f position = {0,0,0,0};     // Sa position dans l'espace
    Vecteur2f velocite = {0,0,0,0};     // Sa vélocité sur 2 dimensions
    SDL_Rect  hitbox;       // Boite de collision
    SDL_Surface *surface = 0;   // Surface SDL
};


/* Programme principale
=================================*/
const int	LARGEUR_ALIEN = 50,         // La largeur en pixel d'un alien
			HAUTEUR_ALIEN = 50,         // La hauteur en pixel d'un alien
			LARGEUR_HERO = 50,          // La largeur en pixel du hero
			HAUTEUR_HERO = 50,          // La hauteur en pixel du hero
			LARGEUR_BLOC = 10,          // Le nombre de colone d'alien dans le bloc
			HAUTEUR_BLOC = 5;           // Le nombre de ligne d'alien dans le bloc

const float	VITESSE_BALLE = 1;          // La vitesse d'une balle

int main(int argc, char *argv[])
{
	
	/*************** Variable SDL *********************/
	// Les differentes surfaces du programme
	SDL_Surface *ecran = 0;            // L'ecran

	Entite joueur;
    Entite balleHero;
    
    // Les rectangles
	Vecteur2f posAlien,                 // Vecteur de la position du bloc d'alien
              velAlien;                // La velocite du bloc d'alien

	vector<Entite> balleAlien;    // Un tableau dynamique des balles aliens

	/*************** Variable Standards *********************/
	bool	partieTerminee = false,    // La partie est terminee
			droitTirer = true,         // Le hero a le droit de tirer
			toucheD=0,                 // La touche Droite est appuyee
			toucheG=0,                 // La touche Gauche est appuyee
			toucheH=0;                 // La touche Haut ou même espace est appuyee

	unsigned int	score = 0;         // Le score de la presente partie

	int		vie = 3;                   // Le nombre de vie du joueur

	unsigned long ticks;               // Le nombre de milisecondes ecoulees depuis le lancement du programme

	Entite Alien[HAUTEUR_BLOC][LARGEUR_BLOC]; // Matrice contenant les informations du bloc d'aliens

	/*************** PROGRAMME *********************/
	// Initialisation SDL
	if(SDL_Init(SDL_INIT_VIDEO) == -1) // Si elle echoue…
	{
		cout << "Impossible d initialiser SDL !"; // On averti l'utilisateur et on quitte
		return EXIT_FAILURE;
	}
	
	// On charge l'image du hero a l'aide de la bibliotheque SDL_Images
	joueur.surface = SDL_LoadBMP("SpaceInvaders.bmp");
    
    if(!joueur.surface) {
        cerr << "Impossible de charger l'image" << endl;
    }
    
    joueur.position.x = 200;
    joueur.position.y = 500;
    
    // On cree l'ecran a l'aide de la fonction initEcran
	ecran = initEcran((char*)"Space Invaders", NULL, 400, 600, 0, 0, 0);
	
	/*************** BOUCLE PRINCIPALE *********************/
	while(partieTerminee == false)         // Tant que la partie n'est pas terminee...
	{
		SDL_Event event;                  // On recupere l'evenement de la fenêtre sans bloquer le programme
		SDL_PollEvent(&event);            
		switch(event.type)
		{
		case SDL_QUIT:                    // Si on appui sur le bouton de fermeture de la fenêtre...
			partieTerminee = true;         // On quitte la boucle
			break;
			case SDL_KEYDOWN:/* Si appui d'une touche, on sauvegarde son etat dans le booleen correspondant*/
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
                        default:
                            break;
					}
					break;
				case SDL_KEYUP:/* Si retire d'une touche, on sauvegarde son etat dans le booleen correspondant*/
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
                        default:
                            break;
					}
					break;
            default:
                break;
		}
		
		// On reinitialise la velocite du hero a 0
		joueur.velocite.x = 0;

        // Si on appui sur la touche de droite...		
		if(toucheD)
			joueur.velocite.x = 1;   // La velocite du hero est 1 sur son axe x
		if(toucheG)
			joueur.velocite.x = -1; 
			
		// On deplace le hero en fonction de sa velocite
		joueur.position.x += joueur.velocite.x;
        
        // On remplit l'ecran de noir
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
		
		// On dessine le hero
		SDL_BlitSurface(joueur.surface, NULL, ecran, &joueur.position);
		
		// On change les buffers
		SDL_Flip(ecran);
		
		// On endort le programme 12 ms
		SDL_Delay(12);

	}
	
	// On libere la memoire de la surface
	SDL_FreeSurface(joueur.surface);
	
	// On ferme la SDL
	SDL_Quit();
	
	// On quitte
 	return EXIT_SUCCESS;
}
