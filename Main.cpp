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
#include <list>

using namespace std;

/*Structures
 ================================*/

struct Vecteur2f {
    float x;
    float y;  
};

// Structure pour définir un objet dans le jeu
struct Entite {
    bool vivant;                        // L'entite est-il vivant?
    int vie;                            // Son nombre de vie
    
    Vecteur2f position;                 // Sa position dans l'espace
    Vecteur2f velocite;                 // Sa vélocité sur 2 dimensions
    float largeur;                      // Sa largeur en pixel
    float hauteur;                      // Sa hauteur en pixel
    SDL_Rect spritePos;
    
    SDL_Surface *surface;               // Surface SDL
    SDL_Rect subrect;                   // Correspond au sous-rectangle a afficher de la surface
};

struct Timer {
    unsigned long old;
    unsigned long actual;
};

struct Randomizer
{
	unsigned long seed;
	int		minimum,
			maximum;
};

const int	LARGEUR_ALIEN = 13,        // La largeur en pixel d'un alien
            HAUTEUR_ALIEN = 7,         // La hauteur en pixel d'un alien
            LARGEUR_HERO = 13,         // La largeur en pixel du hero
            HAUTEUR_HERO = 8,          // La hauteur en pixel du hero
            LARGEUR_BLOC = 10,         // Le nombre de colone d'alien dans le bloc
            HAUTEUR_BLOC = 6;         // Le nombre de ligne d'alien dans le bloc

const float	VITESSE_BALLE = 0.25f,     // La vitesse d'une balle
            VITESSE_HERO = 0.25f;      // La vitesse du héro

const int   LARGEUR_ECRAN = 400,
            HAUTEUR_ECRAN = 600;

Entite creerEntite(int vie, float x, float y, float largeur, float hauteur);
void tirerProjectile(Entite &projectile, float x, float y);

void rafraichirPositionEntite(Entite &entite);

bool collisionEntiteBordure(Entite &entite, int largeur, int hauteur);
bool collisionEntiteEntite(Entite &a, Entite &b);

void toucheAppuyer(SDL_Event event, bool &toucheH, bool &toucheG, bool &toucheD);
void toucheRetirer(SDL_Event event, bool &toucheH, bool &toucheG, bool &toucheD);

void animerAlien(Entite &alien);

int random(Randomizer randomizer);

void afficherRendu(SDL_Surface *ecran, Entite &joueur, Entite &balleHero, Entite Alien[][LARGEUR_BLOC], list<Entite> &balleAlien);

void verifierMortEntite(Entite &entite);

void initialiserAliens(Entite alien[][LARGEUR_BLOC], SDL_Surface *spriteSheet);

/* Programme principale
=================================*/

int main(int argc, char *argv[])
{
	/*************** Variable SDL *********************/
	// Les differentes surfaces du programme
	SDL_Surface *ecran = 0;             // L'ecran
    SDL_Surface *spriteSheet = 0;

	Entite joueur;                      // Il s'agit du hero (le vaisseau vert)
    Entite balleHero;                   // Il s'agit du projectile du héro (1 seul à la fois)

	list<Entite> balleAlien;                // Un tableau dynamique des balles aliens
    list<Entite>::iterator it_balleAlien;   // L'itérateur des balles aliens
    
	/*************** Variable Standards *********************/
	bool	partieTerminee = false,    // La partie est terminee
			droitTirer = true,         // Le hero a le droit de tirer
			toucheD = false,                 // La touche Droite est appuyee
			toucheG = false,                 // La touche Gauche est appuyee
			toucheH = false;                 // La touche Haut ou même espace est appuyee

	unsigned int  score = 0;            // Le score de la presente partie

	unsigned long ticks;                // Le nombre de milisecondes ecoulees depuis le lancement du programme

	Entite Alien[HAUTEUR_BLOC][LARGEUR_BLOC]; // Matrice contenant les informations du bloc d'aliens

	//================================================================================================================================
	Randomizer randomizer;
    
	randomizer.seed = (time(NULL));
    randomizer.minimum = 0;
    randomizer.maximum = 100;
    
	srand(randomizer.seed);

    
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
    if(!spriteSheet) 
    {
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
    
    // Initialisation du tableau d'alien
    initialiserAliens(Alien, spriteSheet);
    
    // On cree l'ecran a l'aide de la fonction initEcran
	ecran = initEcran((char*)"Space Invaders", NULL, LARGEUR_ECRAN, HAUTEUR_ECRAN, 0, 0, 0);
	
    unsigned long oldTicks;
    float delta;
    
    Timer alienDepTimer;
    alienDepTimer.old = SDL_GetTicks();
    alienDepTimer.actual = SDL_GetTicks();
    
	/*************** BOUCLE PRINCIPALE *********************/
	while(partieTerminee == false)         // Tant que la partie n'est pas terminee...
	{
        oldTicks = ticks;
        ticks = SDL_GetTicks();            // On récupère les ticks de l'application
    
        alienDepTimer.actual = ticks;
        
        delta = ticks - oldTicks;
        
		SDL_Event event;                  // On recupere l'evenement de la fenêtre sans bloquer le programme
		SDL_PollEvent(&event);            
		switch(event.type)
		{
            case SDL_QUIT:                    // Si on appui sur le bouton de fermeture de la fenêtre...
                partieTerminee = true;         // On quitte la boucle
                break;
			case SDL_KEYDOWN:/* Si appui d'une touche, on sauvegarde son etat dans le booleen correspondant*/
                toucheAppuyer(event, toucheH, toucheG,  toucheD);
                break;
            case SDL_KEYUP:/* Si retire d'une touche, on sauvegarde son etat dans le booleen correspondant*/
                toucheRetirer(event, toucheH, toucheG, toucheD);
                break;
            default:
                break;
		}
		
		// On reinitialise la velocite du hero a 0
		joueur.velocite.x = 0;

        balleHero.velocite.y = -VITESSE_BALLE * delta;
        
        // Si la balle du héro est vivante, on rafraichit sa position
        if(balleHero.vivant) 
        {
            rafraichirPositionEntite(balleHero);
        }
        
        for(int i = 0; i < HAUTEUR_BLOC; i++) 
        {
            for(int j = 0; j < LARGEUR_BLOC; j++) 
            {
                randomizer.maximum = 100;
                
                // On génère un nombre entre 1 et 100
                int anim = random(randomizer);
                
                if(anim < 5)                        // 4% de chance que l'alien change sa frame d'animation
                {
                    animerAlien(Alien[i][j]);
                }
                
                randomizer.maximum = 10000;
                int shoot = random(randomizer);     // On génere un nombre entre 0 et 10000
                
                if(shoot == 20)                     // Si ce nombre égal 20 (donc 0.01% de chance) l'alien tir un projectile
                {                   
                    Entite balle;
                    balle = creerEntite(1, 1000, 1000, 1, 4);
                    balle.surface = spriteSheet;
                    balle.subrect.x = 25;
                    balle.subrect.y = 0;
                    balle.velocite.y = VITESSE_BALLE;
                    
                    tirerProjectile(balle, Alien[i][j].position.x, Alien[i][j].position.y);
                    
                    balleAlien.push_back(balle);
                    
                }
                
                // Si la balle du héro atteint un alien
                if(collisionEntiteEntite(balleHero, Alien[i][j]) && Alien[i][j].vivant && balleHero.vivant) 
                {
                    // La balle est détruite
                    balleHero.vivant = false;
                    droitTirer =  true;
                    
                    // L'alien perd une vie
                    Alien[i][j].vie -= 1;
                    
                    // Il prend du recul vers le haut
                    Alien[i][j].velocite.y -= HAUTEUR_ALIEN/2;
                    
                    // Si l'alien n'a plus de vie, il est mort
                    verifierMortEntite(Alien[i][j]);
                }
                
                it_balleAlien = balleAlien.begin();
                
                while(it_balleAlien != balleAlien.end()) 
                {
                    
                    it_balleAlien->velocite.y =  0.0025f * delta;
                    
                    if(collisionEntiteBordure(*it_balleAlien, LARGEUR_ECRAN, HAUTEUR_ECRAN)) 
                    {
                        it_balleAlien->vivant = false;
                    }
                    
                    if(collisionEntiteEntite(*it_balleAlien, joueur)) 
                    {
                        it_balleAlien->vivant = false;
                        
                        joueur.vie--;
                        
                        verifierMortEntite(joueur);
                    }
                    
                    if(it_balleAlien->vivant == false) 
                    {
                        it_balleAlien = balleAlien.erase(it_balleAlien);
                    } 
                    else 
                    {
                        rafraichirPositionEntite(*it_balleAlien);
                        
                        it_balleAlien++;
                    }
                }
		
                // S'il y a collision avec la bordure et un alien
                // Il change sa direction, et descend pour se mettre entre deux rangés
                if(collisionEntiteBordure(Alien[i][j], LARGEUR_ECRAN, HAUTEUR_ECRAN)) 
                {
                    if(Alien[i][j].position.x + LARGEUR_ALIEN >= LARGEUR_ECRAN) 
                    {
                        Alien[i][j].position.x = LARGEUR_ECRAN - (1 + LARGEUR_ALIEN);
                    } 
                    else 
                    {
                        Alien[i][j].position.x = 1;
                    }
                    
                    Alien[i][j].position.y += HAUTEUR_ALIEN * 2;
                    Alien[i][j].velocite.x = -1 * Alien[i][j].velocite.x;
                }
            }
        }
        
        // Le déplacment des aliens dépend du temps
        if(alienDepTimer.actual - alienDepTimer.old >= 80) 
        {
            for(int i = 0; i < HAUTEUR_BLOC; i++) 
            {
                for(int j = 0; j < LARGEUR_BLOC; j++) 
                {
                    // On rafraichit la position de l'alien
                    rafraichirPositionEntite(Alien[i][j]);
                    // Par défaut les aliens ne descende pas
                    Alien[i][j].velocite.y = 0;
                }
            }
            // On réinitialise le timer de déplacement
            alienDepTimer.old = alienDepTimer.actual;
        }
        
        // Si la balle touche une bordure, on la détruit
        if(collisionEntiteBordure(balleHero, LARGEUR_ECRAN, HAUTEUR_ECRAN)) 
        {
            balleHero.vivant = false;
            droitTirer = true;
        }
        
        // Si le joeur touche une bordure, on le téleporte à l'autre
        if(collisionEntiteBordure(joueur, LARGEUR_ECRAN, HAUTEUR_ECRAN)) 
        {
            if(joueur.position.x <= 0)
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
        if(toucheH && droitTirer) 
        {
            tirerProjectile(balleHero, joueur.position.x + joueur.largeur/2, joueur.position.y);
            droitTirer = false;
        }
        
		// On deplace le hero en fonction de sa velocite
        rafraichirPositionEntite(joueur);
        
        if(joueur.vivant == false)
            partieTerminee = true;
        
        // On affiche la frame actuelle
        afficherRendu(ecran, joueur, balleHero, Alien, balleAlien);
		
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

void afficherRendu(SDL_Surface *ecran, Entite &joueur, Entite &balleHero, Entite Alien[][LARGEUR_BLOC], list<Entite> &balleAlien) 
{
    // On remplit l'ecran de noir
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    
    // On dessine le hero
    SDL_BlitSurface(joueur.surface, &joueur.subrect, ecran, &joueur.spritePos);
    
    // Initialisation du tableau d'alien
    for(int i = 0; i < HAUTEUR_BLOC; i++) 
    {
        for(int j = 0; j < LARGEUR_BLOC; j++) 
        {
            if(Alien[i][j].vivant == true)
                SDL_BlitSurface(Alien[i][j].surface, &Alien[i][j].subrect, ecran, &Alien[i][j].spritePos);
        }
    }
    
    // Si la balle du héro est vivante, on la dessine
    if(balleHero.vivant) 
    {
        SDL_BlitSurface(balleHero.surface, &balleHero.subrect, ecran, &balleHero.spritePos); 
    }
    
    // Pour toutes les balles aliens, si elle est vivante on l'affiche
    for(list<Entite>::iterator it = balleAlien.begin(); it != balleAlien.end(); it++) 
    {
        if(it->vivant)
            SDL_BlitSurface(it->surface, &it->subrect, ecran, &it->spritePos); 
    }
    
    // On change les buffers
    SDL_Flip(ecran);
}

/* Fonction automatisant la creation d'entite.  Parametres: Le nombre de vie, la position initialie, la largeur et la hauteur
                                                Retour: Un entite dont les attributs correspondent aux parametres */
Entite creerEntite(int vie, float x, float y, float largeur, float hauteur) 
{
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

// On déplace un entité en fonction de sa velocité
void rafraichirPositionEntite(Entite &entite) 
{
    entite.position.x += entite.velocite.x;
    entite.position.y += entite.velocite.y;
    
    entite.spritePos.x = entite.position.x;
    entite.spritePos.y = entite.position.y;
}

// Fonction pour tirer un projectile
void tirerProjectile(Entite &projectile, float x, float y) 
{
    projectile.vivant = true;
    projectile.position.x = x;
    projectile.position.y = y;
}

// Retourne vrai si un entité est en contact avec l'une des 4 bordures de l'écran
bool collisionEntiteBordure(Entite &entite, int largeur, int hauteur) 
{
    return(entite.position.x + entite.largeur >= largeur ||
           entite.position.x <= 0 ||
           entite.position.y <= 0 ||
           entite.position.y + entite.hauteur >= hauteur);
}

// Retourne vrai si deux entités sont en contact
bool collisionEntiteEntite(Entite &a, Entite &b) 
{
    if(a.position.x + a.largeur < b.position.x ||   // Si le rectangle a est plus a gauche que le rectangle b
       a.position.y + a.largeur < b.position.y ||   // Ou si le rectangle a est plus haut que le rectangle b
       a.position.x > b.position.x + b.largeur ||   // Ou si le rectangle a est plus à droite que le rectangle b
       a.position.y > b.position.y + b.hauteur)     // Ou si le rectangle a est plus bas que le rectangle b
    {   
        return false;                               // Il n'y a pas de collision
    }
    
    return true;                                    // Sinon il y en a une
}

// Fonction d'appui sur une touche
void toucheAppuyer(SDL_Event event, bool &toucheH, bool &toucheG, bool &toucheD)
{
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
    
}

// Fonction du retirement de touche
void toucheRetirer(SDL_Event event, bool &toucheH, bool &toucheG, bool &toucheD)
{
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
    
}

// Fonction qui génère un nombre aléatoire
int random(Randomizer randomizer)
{
	int rd;
	rd = ((rand() % randomizer.maximum) + randomizer.minimum);
	return rd;
}


// Fonction qui sert à alterner la frame d'animation d'un alien
void animerAlien(Entite &alien) 
{
    if(alien.subrect.x == LARGEUR_ALIEN) 
    {
        alien.subrect.x = 0;
    } else 
    {
        alien.subrect.x = LARGEUR_ALIEN;
    }
}

void verifierMortEntite(Entite &entite) 
{
    if(entite.vie <= 0)
        entite.vivant = false;
}

void initialiserAliens(Entite Alien[][LARGEUR_BLOC], SDL_Surface *spriteSheet) {
    for(int i = 0; i < HAUTEUR_BLOC; i++)
    {
        for(int j = 0; j < LARGEUR_BLOC; j++) 
        {
            Alien[i][j] = creerEntite(0, j * (LARGEUR_ALIEN + LARGEUR_ALIEN) + 10, i * (HAUTEUR_ALIEN + HAUTEUR_ALIEN * 3) + 10, LARGEUR_ALIEN, HAUTEUR_ALIEN);
            Alien[i][j].surface = spriteSheet;
            Alien[i][j].subrect.x = 0;
            
            // Les deux rangés du haut deviennent les petits aliens avec 1 vie
            if(i >= 0 && i < 2) 
            {
                Alien[i][j].subrect.y = 24;
                Alien[i][j].vie = 1;
            }
            else if(i >= 2 && i < 4)    // Les aliens du centres sont les plus connus avec 2 vies
            {
                Alien[i][j].subrect.y = 9;
                Alien[i][j].vie = 2;
            }
            else                        // Et la première rangé, la chair canon avec 3 vies
            {
                Alien[i][j].subrect.y = 16;
                Alien[i][j].vie = 3;
            }
            
            Alien[i][j].velocite.x = 1;
            
            rafraichirPositionEntite(Alien[i][j]);
        }
    }
}