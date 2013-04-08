/* Entete du programme
Nom: Space invaders
Auteur: Maxime Fontaine Bombardier
		Nathan Giraldeau
Date: 03/04/2013
Description du programme: Space invader en SDL

Directives au pre-processeur
=================================*/

// Restructuration terminée!


#include "SDL_Utils.h"
#include <vector>

using namespace std;

/*Structures
 ================================*/

// Ici c'est uniquement une sorte de camouflage. Donc il s'agit d'un SDL_Rect mais on peut aussi l'appelé Vecteur2f
typedef SDL_Rect Vecteur2f;

// Structure pour définir un objet dans le jeu
struct Entite {
    bool vivant = true;                 // L'entite est-il vivant?
    int vie = 1;                        // Son nombre de vie
    
    Vecteur2f position = {0,0,0,0};     // Sa position dans l'espace
    Vecteur2f velocite = {0,0,0,0};     // Sa vélocité sur 2 dimensions
    float largeur;                      // Sa largeur en pixel
    float hauteur;                      // Sa hauteur en pixel
    
    SDL_Surface *surface = 0;           // Surface SDL
    SDL_Rect subrect;                   // Correspond au sous-rectangle a afficher de la surface
};

Entite creerEntite(int vie, Vecteur2f position, float largeur, float hauteur);

/* Programme principale
=================================*/
const int	LARGEUR_ALIEN = 50,         // La largeur en pixel d'un alien
			HAUTEUR_ALIEN = 50,         // La hauteur en pixel d'un alien
			LARGEUR_HERO = 50,          // La largeur en pixel du hero
			HAUTEUR_HERO = 50,          // La hauteur en pixel du hero
			LARGEUR_BLOC = 10,          // Le nombre de colone d'alien dans le bloc
			HAUTEUR_BLOC = 5;           // Le nombre de ligne d'alien dans le bloc

const float	VITESSE_BALLE = 5;          // La vitesse d'une balle

const int LARGEUR_ECRAN = 400,
            HAUTEUR_ECRAN = 600;

int main(int argc, char *argv[])
{
	/*************** Variable SDL *********************/
	// Les differentes surfaces du programme
	SDL_Surface *ecran = 0;             // L'ecran
    SDL_Surface *spriteSheet = 0;

	Entite joueur;                      // Il s'agit du hero (le vaisseau vert)
    Entite balleHero;                   // Il s'agit du projectile du héro (1 seul à la fois)
    
	Vecteur2f posAlien,                 // Vecteur de la position du bloc d'alien
              velAlien;                 // La velocite du bloc d'alien

	vector<Entite> balleAlien;          // Un tableau dynamique des balles aliens

	/*************** Variable Standards *********************/
	bool	partieTerminee = false,    // La partie est terminee
			droitTirer = true,         // Le hero a le droit de tirer
			toucheD = false,                 // La touche Droite est appuyee
			toucheG = false,                 // La touche Gauche est appuyee
			toucheH = false;                 // La touche Haut ou même espace est appuyee

	unsigned int	score = 0;         // Le score de la presente partie

	unsigned long ticks;               // Le nombre de milisecondes ecoulees depuis le lancement du programme

	Entite Alien[HAUTEUR_BLOC][LARGEUR_BLOC]; // Matrice contenant les informations du bloc d'aliens

	/*************** PROGRAMME *********************/
	// Initialisation SDL
	if(SDL_Init(SDL_INIT_VIDEO) == -1) // Si elle echoue…
	{
		cout << "Impossible d initialiser SDL !"; // On averti l'utilisateur et on quitte
		return EXIT_FAILURE;
	}
    
    spriteSheet = IMG_Load("spaceInvadersSpriteSheet.png");
    
    if(!spriteSheet) {
        cerr << "Impossible de charger l'image 'spaceInvadersSpriteSheet.png'" << endl;
        return EXIT_FAILURE;
    }
    
    joueur = creerEntite(3, {200,500}, 13, 8);
    joueur.surface = spriteSheet;
    joueur.subrect.x = 0;
    joueur.subrect.y = 32;
    
    
    balleHero = creerEntite(1, {1000, 1000}, 1, 4);
    balleHero.surface = spriteSheet;
    balleHero.subrect.x = 25;
    balleHero.subrect.y = 0;
    balleHero.velocite.y = -VITESSE_BALLE;
    
    // On cree l'ecran a l'aide de la fonction initEcran
	ecran = initEcran((char*)"Space Invaders", NULL, LARGEUR_ECRAN, HAUTEUR_ECRAN, 0, 0, 0);
	
	/*************** BOUCLE PRINCIPALE *********************/
	while(partieTerminee == false)         // Tant que la partie n'est pas terminee...
	{
        ticks = SDL_GetTicks();            // On récupère les ticks de l'application
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
							toucheH = true;
							break;
						case SDLK_LEFT:
							toucheG = true;
							break;
						case SDLK_RIGHT:
							toucheD = true;
							break;
                        default:
                            break;
					}
					break;
				case SDL_KEYUP:/* Si retire d'une touche, on sauvegarde son etat dans le booleen correspondant*/
					switch (event.key.keysym.sym)
					{
					case SDLK_UP: case SDLK_SPACE:
							toucheH = false;
							break;
						case SDLK_LEFT:
							toucheG = false;
							break;
						case SDLK_RIGHT:
							toucheD = false;
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

        if(balleHero.vivant) {
            balleHero.position.y += balleHero.velocite.y;
        }
        
        if(balleHero.position.y == 0) {
            balleHero.vivant = false;
            droitTirer = true;
        }
        
        if(joueur.position.x + joueur.largeur >= LARGEUR_ECRAN) {
            joueur.position.x = LARGEUR_ECRAN - joueur.largeur;   
        }
        
        // Si on appui sur la touche de droite...		
		if(toucheD)
			joueur.velocite.x = 1;   // La velocite du hero est 1 sur son axe x
		if(toucheG)
			joueur.velocite.x = -1; 
			
        
        // Si on appui sur haut et qu'on peut tirer...
        if(toucheH && droitTirer) {
            balleHero.vivant = true;
            balleHero.position.x = joueur.position.x;
            balleHero.position.y = joueur.position.y;
            
            droitTirer = false;
        }
        
		// On deplace le hero en fonction de sa velocite
		joueur.position.x += joueur.velocite.x;
        
        // On remplit l'ecran de noir
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
		
		// On dessine le hero
		SDL_BlitSurface(joueur.surface, &joueur.subrect, ecran, &joueur.position);
		
        // Si la balle du héro est vivante
        if(balleHero.vivant) {
            // On dessine la balle du hero
            SDL_BlitSurface(balleHero.surface, &balleHero.subrect, ecran, &balleHero.position); 
        }
        
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

// Fonction automatisant la creation d'entite.  Parametres: Le nombre de vie, la position initialie, la largeur et la hauteur
//                                              Retour: Un entite dont les attributs correspondent aux parametres
Entite creerEntite(int vie, Vecteur2f position, float largeur, float hauteur) {
    Entite entite;
    
    entite.vivant = true;                   // Par défaut, l'entite est vivant
    entite.vie = vie;                       // On lui definit son nombre de vie
    entite.position = position;             // On lui assigne une position initiale
    entite.largeur = largeur;               // On lui donne la largeur souhaite
    entite.hauteur = hauteur;               // On lui donne la hauteur souhaite
    
    entite.velocite.x = 0;                  // À sa création, l'entité est immobile
    entite.velocite.y = 0;
    
    entite.subrect.w = largeur;
    entite.subrect.h = hauteur;
    
    return entite;                          // On retourne le nouvel entité
}