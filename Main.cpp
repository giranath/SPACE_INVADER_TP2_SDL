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
    bool vivant;                        // L'entite est-il vivant?
    int vie;                            // Son nombre de vie
    
    Vecteur2f position;                 // Sa position dans l'espace
    Vecteur2f velocite;                 // Sa vélocité sur 2 dimensions
    float largeur;                      // Sa largeur en pixel
    float hauteur;                      // Sa hauteur en pixel
    
    SDL_Surface *surface;               // Surface SDL
    SDL_Rect subrect;                   // Correspond au sous-rectangle a afficher de la surface
};

Entite creerEntite(int vie, float x, float y, float largeur, float hauteur);
void tirerProjectile(Entite &projectile, float x, float y);

void rafraichirPositionEntite(Entite &entite);

bool collisionEntiteBordure(Entite &entite, int largeur, int hauteur);
bool collisionEntiteEntite(Entite &a, Entite &b);

/* Programme principale
=================================*/
const int	LARGEUR_ALIEN = 13,         // La largeur en pixel d'un alien
			HAUTEUR_ALIEN = 7,         // La hauteur en pixel d'un alien
			LARGEUR_HERO = 13,          // La largeur en pixel du hero
			HAUTEUR_HERO = 8,          // La hauteur en pixel du hero
			LARGEUR_BLOC = 10,          // Le nombre de colone d'alien dans le bloc
			HAUTEUR_BLOC = 5;           // Le nombre de ligne d'alien dans le bloc

const float	VITESSE_BALLE = 0.85f,
            VITESSE_HERO = 0.25f;          // La vitesse d'une balle

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

	unsigned int  score = 0;            // Le score de la presente partie

	unsigned long ticks;                // Le nombre de milisecondes ecoulees depuis le lancement du programme

	Entite Alien[HAUTEUR_BLOC][LARGEUR_BLOC]; // Matrice contenant les informations du bloc d'aliens

	/*************** PROGRAMME *********************/
	// Initialisation SDL
	if(SDL_Init(SDL_INIT_VIDEO) == -1) // Si elle echoue…
	{
		cout << "Impossible d initialiser SDL !"; // On averti l'utilisateur et on quitte
		return EXIT_FAILURE;
	}
    
    // On charge le sprite sheet en mémoire
    spriteSheet = SDL_LoadBMP("spaceInvadersSpriteSheet.bmp");
    SDL_SetColorKey(spriteSheet, SDL_SRCCOLORKEY, SDL_MapRGB(spriteSheet->format, 255, 0, 255));
    
    // Si une erreur est survenue...
    if(!spriteSheet) {
        // On prévient l'utilisateur
        cerr << "Impossible de charger l'image 'spaceInvadersSpriteSheet.png'" << endl;
        // On quitte
        return EXIT_FAILURE;
    }
    
    joueur = creerEntite(3, 200, 500, LARGEUR_HERO, HAUTEUR_HERO);
    joueur.surface = spriteSheet;
    joueur.subrect.x = 0;
    joueur.subrect.y = 32;
    
    balleHero = creerEntite(1, 1000, 1000, 1, 4);
    balleHero.surface = spriteSheet;
    balleHero.subrect.x = 25;
    balleHero.subrect.y = 0;
    balleHero.velocite.y = -VITESSE_BALLE;
    
    // On cree l'ecran a l'aide de la fonction initEcran
	ecran = initEcran((char*)"Space Invaders", NULL, LARGEUR_ECRAN, HAUTEUR_ECRAN, 0, 0, 0);
	
    unsigned long oldTicks;
    float delta;
    
	/*************** BOUCLE PRINCIPALE *********************/
	while(partieTerminee == false)         // Tant que la partie n'est pas terminee...
	{
        oldTicks = ticks;
        ticks = SDL_GetTicks();            // On récupère les ticks de l'application
        
        delta = ticks - oldTicks;
        
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

        balleHero.velocite.y = -VITESSE_BALLE * delta;
        
        if(balleHero.vivant) {
            rafraichirPositionEntite(balleHero);
        }
        
        if(collisionEntiteBordure(balleHero, LARGEUR_ECRAN, HAUTEUR_ECRAN)) {
            balleHero.vivant = false;
            droitTirer = true;
        }
        
        if(collisionEntiteBordure(joueur, LARGEUR_ECRAN, HAUTEUR_ECRAN)) {
            if(joueur.position.x == 0)
                joueur.position.x = LARGEUR_ECRAN - joueur.largeur - 1;
            else 
                joueur.position.x = 1;
        }
        
        // Si on appui sur la touche de droite...		
		if(toucheD)
			joueur.velocite.x = VITESSE_HERO * delta;   // La velocite du hero est 1 sur son axe x
		if(toucheG)
			joueur.velocite.x = -VITESSE_HERO * delta; 
			
        
        // Si on appui sur haut et qu'on peut tirer...
        if(toucheH && droitTirer) {
            tirerProjectile(balleHero, joueur.position.x + joueur.largeur/2, joueur.position.y);
            droitTirer = false;
        }
        
		// On deplace le hero en fonction de sa velocite
        rafraichirPositionEntite(joueur);
        
        // On remplit l'ecran de noir
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
		
		// On dessine le hero
		SDL_BlitSurface(joueur.surface, &joueur.subrect, ecran, &joueur.position);
		
        // Si la balle du héro est vivante, on la dessine
        if(balleHero.vivant) {
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

/* Fonction automatisant la creation d'entite.  Parametres: Le nombre de vie, la position initialie, la largeur et la hauteur
                                                Retour: Un entite dont les attributs correspondent aux parametres */
Entite creerEntite(int vie, float x, float y, float largeur, float hauteur) {
    Entite entite;
    
    entite.vivant = true;                   // Par défaut, l'entite est vivant
    entite.vie = vie;                       // On lui definit son nombre de vie
    entite.position.x = x;             // On lui assigne une position initiale
    entite.position.y = y; 
    entite.largeur = largeur;               // On lui donne la largeur souhaite
    entite.hauteur = hauteur;               // On lui donne la hauteur souhaite
    
    entite.velocite.x = 0;                  // À sa création, l'entité est immobile
    entite.velocite.y = 0;
    
    entite.subrect.w = largeur;
    entite.subrect.h = hauteur;
    
    return entite;                          // On retourne le nouvel entité
}

void rafraichirPositionEntite(Entite &entite) {
    entite.position.x += entite.velocite.x;
    entite.position.y += entite.velocite.y;
}

void tirerProjectile(Entite &projectile, float x, float y) {
    projectile.vivant = true;
    projectile.position.x = x;
    projectile.position.y = y;
}

bool collisionEntiteBordure(Entite &entite, int largeur, int hauteur) {
    return(entite.position.x + entite.largeur >= largeur ||
           entite.position.x <= 0 ||
           entite.position.y <= 0 ||
           entite.position.y + entite.hauteur >= hauteur);
}

bool collisionEntiteEntite(Entite &a, Entite &b) {
    if(a.position.x + a.largeur < b.position.x ||   // Si le rectangle a est plus a gauche que le rectangle b
       a.position.y + a.largeur < b.position.y ||   // Ou si le rectangle a est plus haut que le rectangle b
       a.position.x > b.position.x + b.largeur ||   // Ou si le rectangle a est plus à droite que le rectangle b
       a.position.y > b.position.y + b.hauteur) {   // Ou si le rectangle a est plus bas que le rectangle b
        return false;                               // Il n'y a pas de collision
    }
    
    return true;                                    // Sinon il y en a une
}