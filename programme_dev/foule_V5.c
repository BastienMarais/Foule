#include "../lib/libgraphique.h"
#include "../lib/lib_foule.h"
#include <unistd.h>

#define UNITE 10    //côté d'une case en pixels
#define NB_CASES_X 80  // nb de cases en abscisse (et dans le tableau 'plateau')
#define NB_CASES_Y 60  // nb de cases en ordonnée (et dans le tableau 'plateau')
#define FICHIER_PLAN "../data/plan.txt"
#define FICHIER_JOUEURS "../data/joueurs"

// conditions des différents boutons
#define START p2.x<=639 && p2.x>=159 && p2.y<=320 && p2.y>=274
#define OPTION p2.x<=639 && p2.x>=159 && p2.y<=418 && p2.y>=372
#define QUITTER p2.x<=639 && p2.x>=159 && p2.y<=514 && p2.y>=468
#define MENU_FIN p2.x<=639 && p2.x>=159 && p2.y<=407 && p2.y>=360
#define QUITTER_FIN p2.x<=639 && p2.x>=159 && p2.y<=499 && p2.y>=453

#define NB_JOUEUR 50


/******************************************************************************/
/* Déclaration des structures et variables globales                           */
/******************************************************************************/

typedef struct Joueur
{
	int depart_x;
	int depart_y;
	int fin_x;
	int fin_y;
	int posi_act_x;
	int posi_act_y;
	int couleur;
	
} Joueur;

// tableau des positions des joueurs
struct Joueur foule[NB_JOUEUR+1];

// 0 = menu , 1 = start , 2 = options , 3 = quitter
int choix;

// variable qui gère la vitesse d'exécution de la simulation
int temps=0;

//correspond a : vide = 0 , mur = 1 , entrée = 2 , sortie = 3 , joueur = 4
int plateau[NB_CASES_X][NB_CASES_Y]={{0}};

int f;

/******************************************************************************/
/* Déclaration de vos fonctions                                               */
/******************************************************************************/

void aff_plan ();
void charge_joueur(char *fjoueurs);
void deplacement_dirige();
void deplacement_alea(int j);
void start();
void menu();
void lancement_menu();
void fin(int i);
void effacement_joueur(int j);
void dessiner_joueur(int j);
void aff_entier (int entier,int taille,Point coin,int couleur);
void dep_fixe(int j);
void dep_haut(int j);
void dep_bas(int j);
void dep_droite(int j);
void dep_gauche(int j);
void dep_haut_droite(int j);
void dep_bas_droite(int j);
void dep_haut_gauche(int j);
void dep_bas_gauche(int j);
void dep_arrivee (int j);
void assignation_couleur();
int detection_sortie(int j);
void initialisation_posi_joueur();

/******************************************************************************/
/* main                                                                       */
/******************************************************************************/
int main()
{

	// initialisation des murs du plan et joueurs dans des tablaux:
	charge_plan(FICHIER_PLAN, plateau);
	charge_joueur(FICHIER_JOUEURS);
	


	// ouvre la fenêtre graphique :
	ouvrir_fenetre(NB_CASES_X*UNITE,NB_CASES_Y*UNITE);

    // lancement du menu
    lancement_menu();

	// fin du programme :
	return 0;
}

/******************************************************************************/
/*  Définitions des autres fonctions                                          */
/******************************************************************************/

/******************************************************************************/
/* affiche le plan avec mur/entrée/sortie                                     */
/******************************************************************************/

void aff_plan ()
{
	Point p1;
	int c,l,j;
	for (j=0;j<NB_JOUEUR;j++)
	{
		plateau[foule[j].depart_x][foule[j].depart_y]=2;
	}
	for (j=0;j<NB_JOUEUR;j++)
	{
		plateau[foule[j].fin_x][foule[j].fin_y]=3;
	}
	
	for (l=0;l<NB_CASES_Y;l++)
	{
		for (c=0;c<NB_CASES_X;c++)
		{
			if (plateau[c][l]==1) // mur
			{
				p1.x=c*UNITE; p1.y=l*UNITE;
				dessiner_rectangle(p1,UNITE,UNITE,blanc);
			}
			if (plateau[c][l]==3) // sortie
			{
				p1.x=c*UNITE; p1.y=l*UNITE;
				dessiner_rectangle(p1,UNITE,UNITE,vert);
			}
		}

	}

	actualiser();
}

/******************************************************************************/
/* transforme 'joueurs' en un tableau de type 'Joueur'                        */
/******************************************************************************/

void charge_joueur(char *fjoueurs)
{
    FILE *f = fopen(fjoueurs,"r");
    int dimx, j;
    

    if(!f)
    {
        printf("Fichier '%s': ouverture impossible\n",fjoueurs);
        exit(1);
    }

    fscanf(f,"%d\n",&dimx);
    if(dimx>NB_JOUEUR)
    {
        printf("Fichier '%s': dimensions du tableau lues dans incorrectes\n",fjoueurs);
        exit(1);
    }

    for(j=0;j<dimx;j++)
    {
            fscanf(f,"%d %d %d %d \n",&foule[j].depart_x,&foule[j].depart_y,&foule[j].fin_x,&foule[j].fin_y);
    }
    fclose(f);
}

/******************************************************************************/
/* déplacement aléatoire   de tous les joueurs                                */
/******************************************************************************/

void deplacement_alea (int j)
{
	int entier_alea;

	entier_alea=entier_aleatoire(8);

	// haut gauche
	if (plateau[foule[j].posi_act_x-1][foule[j].posi_act_y-1]==0 && entier_alea==0)
	{		
		dep_haut_gauche(j);
	}
	// haut
	else if (plateau[foule[j].posi_act_x][foule[j].posi_act_y-1]==0 && entier_alea==1)
	{
		dep_haut(j);
	}
	// haut droit
	else if (plateau[foule[j].posi_act_x+1][foule[j].posi_act_y-1]==0 && entier_alea==2)
	{
		dep_haut_droite(j);
	}
	// droite
	else if (plateau[foule[j].posi_act_x+1][foule[j].posi_act_y]==0 && entier_alea==3)
	{
		dep_droite(j);
	}
	// bas droit
	else if (plateau[foule[j].posi_act_x+1][foule[j].posi_act_y+1]==0 && entier_alea==4)
	{
		dep_bas_droite(j);
	}
	// bas
	else if (plateau[foule[j].posi_act_x][foule[j].posi_act_y+1]==0 && entier_alea==5)
	{
		dep_bas(j);
	}
	// bas gauche
	else if (plateau[foule[j].posi_act_x-1][foule[j].posi_act_y+1]==0 && entier_alea==6)
	{
		dep_bas_gauche(j);
	}
	// gauche
	else if (plateau[foule[j].posi_act_x-1][foule[j].posi_act_y]==0 && entier_alea==7)
	{
		dep_gauche(j);
	}
	else 
	{
		dep_fixe(j);
	}


}

/******************************************************************************/
/* Gère les déplacements de tous les joueurs en même temps                    */
/******************************************************************************/

void deplacement_dirige()
{
	int j;
	for (j=0;j<NB_JOUEUR+1;j++)
	
	{
		//si la sortie est dans les alentours 
		if (detection_sortie(j))
		{
			dep_arrivee(j);
		}
		//si sortie en haut droite
		else if (foule[j].posi_act_x<foule[j].fin_x && foule[j].posi_act_y>foule[j].fin_y && plateau[foule[j].posi_act_x+1][foule[j].posi_act_y-1]==0 )
		{
			dep_haut_droite(j);
		}
		//si sortie en haut gauche
		else if (foule[j].posi_act_x>foule[j].fin_x && foule[j].posi_act_y>foule[j].fin_y && plateau[foule[j].posi_act_x-1][foule[j].posi_act_y-1]==0)
		{
			dep_haut_gauche(j);
		}
		//si sortie en bas droite
		else if (foule[j].posi_act_x<foule[j].fin_x && foule[j].posi_act_y<foule[j].fin_y && plateau[foule[j].posi_act_x+1][foule[j].posi_act_y+1]==0)
		{
			dep_bas_droite(j);
		}
		//si sortie en bas gauche
		else if (foule[j].posi_act_x>foule[j].fin_x && foule[j].posi_act_y<foule[j].fin_y && plateau[foule[j].posi_act_x-1][foule[j].posi_act_y+1]==0)
		{
			dep_bas_gauche(j);
		}
		//si sortie se trouve à droite
		else if(foule[j].posi_act_x<foule[j].fin_x && foule[j].posi_act_y==foule[j].fin_y && plateau[foule[j].posi_act_x+1][foule[j].posi_act_y]==0)
		{
			dep_droite(j);
		}
		//si sortie se trouve à gauche
		else if(foule[j].posi_act_x>foule[j].fin_x && foule[j].posi_act_y==foule[j].fin_y && plateau[foule[j].posi_act_x-1][foule[j].posi_act_y]==0)
		{
			dep_gauche(j);
		}
		//si sortie se trouve en haut
		else if(foule[j].posi_act_x==foule[j].fin_x && foule[j].posi_act_y>foule[j].fin_y && plateau[foule[j].posi_act_x][foule[j].posi_act_y-1]==0)
		{
			dep_haut(j);
		}
		//si sortie se trouve en bas
		else if(foule[j].posi_act_x==foule[j].fin_x && foule[j].posi_act_y<foule[j].fin_y && plateau[foule[j].posi_act_x][foule[j].posi_act_y+1]==0)
		{
			dep_bas(j);
		}
		//si il est sur la sortie
		else if (foule[j].posi_act_x==foule[j].fin_x && foule[j].posi_act_y==foule[j].fin_y)
		{
			dep_fixe(j);
		}
		else
		{
			deplacement_alea(j);

		}
		
		actualiser();
	}
		
}

/******************************************************************************/
/* fonctions gérant le déplacement selon la destination attendue              */
/******************************************************************************/

void dep_arrivee (int j)
{
		effacement_joueur(j);
		//si sortie en haut droite
		if (foule[j].posi_act_x+1==foule[j].fin_x && foule[j].posi_act_y-1==foule[j].fin_y)
		{
			effacement_joueur(j);
			foule[j].posi_act_x=foule[j].posi_act_x+1;
			foule[j].posi_act_y=foule[j].posi_act_y-1;
		}
		//si sortie en haut gauche
		else if (foule[j].posi_act_x-1==foule[j].fin_x && foule[j].posi_act_y-1==foule[j].fin_y)
		{
			effacement_joueur(j);
			foule[j].posi_act_x=foule[j].posi_act_x-1;
			foule[j].posi_act_y=foule[j].posi_act_y-1;
		}
		//si sortie en bas droite
		else if (foule[j].posi_act_x+1==foule[j].fin_x && foule[j].posi_act_y+1==foule[j].fin_y)
		{
			effacement_joueur(j);
			foule[j].posi_act_x=foule[j].posi_act_x+1;
			foule[j].posi_act_y=foule[j].posi_act_y+1;
		}
		//si sortie en bas gauche
		else if (foule[j].posi_act_x-1==foule[j].fin_x && foule[j].posi_act_y+1==foule[j].fin_y)
		{
			effacement_joueur(j);
			foule[j].posi_act_x=foule[j].posi_act_x-1;
			foule[j].posi_act_y=foule[j].posi_act_y+1;
		}
		//si sortie se trouve à droite
		else if(foule[j].posi_act_x+1==foule[j].fin_x && foule[j].posi_act_y==foule[j].fin_y)
		{
			effacement_joueur(j);
			foule[j].posi_act_x=foule[j].posi_act_x+1;
			foule[j].posi_act_y=foule[j].posi_act_y;
		}
		//si sortie se trouve à gauche
		else if(foule[j].posi_act_x-1==foule[j].fin_x && foule[j].posi_act_y==foule[j].fin_y)
		{
			effacement_joueur(j);
			foule[j].posi_act_x=foule[j].posi_act_x-1;
			foule[j].posi_act_y=foule[j].posi_act_y;
		}
		//si sortie se trouve en haut
		else if(foule[j].posi_act_x==foule[j].fin_x && foule[j].posi_act_y-1==foule[j].fin_y)
		{
			effacement_joueur(j);
			foule[j].posi_act_x=foule[j].posi_act_x;
			foule[j].posi_act_y=foule[j].posi_act_y-1;
		}
		//si sortie se trouve en bas
		else if(foule[j].posi_act_x==foule[j].fin_x && foule[j].posi_act_y+1==foule[j].fin_y)
		{
			effacement_joueur(j);
			foule[j].posi_act_x=foule[j].posi_act_x;
			foule[j].posi_act_y=foule[j].posi_act_y+1;
		}
		f++;
}

void dep_fixe(int j)
{
	foule[j].posi_act_x=foule[j].posi_act_x;
	foule[j].posi_act_y=foule[j].posi_act_y;
	dessiner_joueur(j);
}

void dep_haut_droite (int j)
{
	effacement_joueur(j);
	foule[j].posi_act_x=foule[j].posi_act_x+1;
	foule[j].posi_act_y=foule[j].posi_act_y-1;
	dessiner_joueur(j);
}

void dep_haut_gauche (int j)
{
	effacement_joueur(j);
	foule[j].posi_act_x=foule[j].posi_act_x-1;
	foule[j].posi_act_y=foule[j].posi_act_y-1;
	dessiner_joueur(j);
}

void dep_bas_droite (int j)
{
	effacement_joueur(j);
	foule[j].posi_act_x=foule[j].posi_act_x+1;
	foule[j].posi_act_y=foule[j].posi_act_y+1;
	dessiner_joueur(j);
}

void dep_bas_gauche (int j)
{
	effacement_joueur(j);
	foule[j].posi_act_x=foule[j].posi_act_x-1;
	foule[j].posi_act_y=foule[j].posi_act_y+1;
	dessiner_joueur(j);
}

void dep_droite (int j)
{
	effacement_joueur(j);
	foule[j].posi_act_x=foule[j].posi_act_x+1;
	foule[j].posi_act_y=foule[j].posi_act_y;
	dessiner_joueur(j);
}

void dep_gauche (int j)
{
	effacement_joueur(j);
	foule[j].posi_act_x=foule[j].posi_act_x-1;
	foule[j].posi_act_y=foule[j].posi_act_y;
	dessiner_joueur(j);
}

void dep_haut (int j)
{
	effacement_joueur(j);
	foule[j].posi_act_x=foule[j].posi_act_x;
	foule[j].posi_act_y=foule[j].posi_act_y-1;
	dessiner_joueur(j);
}

void dep_bas (int j)
{
	effacement_joueur(j);
	foule[j].posi_act_x=foule[j].posi_act_x;
	foule[j].posi_act_y=foule[j].posi_act_y+1;
	dessiner_joueur(j);
}

/******************************************************************************/
/* vérifie si une sortie est a proximité                                      */
/******************************************************************************/

int detection_sortie(int j)
{
		//si sortie en haut droite
		if (foule[j].posi_act_x+1==foule[j].fin_x && foule[j].posi_act_y-1==foule[j].fin_y)
		{
			return 1;
		}
		//si sortie en haut gauche
		else if (foule[j].posi_act_x-1==foule[j].fin_x && foule[j].posi_act_y-1==foule[j].fin_y)
		{
			return 1;
		}
		//si sortie en bas droite
		else if (foule[j].posi_act_x+1==foule[j].fin_x && foule[j].posi_act_y+1==foule[j].fin_y)
		{
			return 1;
		}
		//si sortie en bas gauche
		else if (foule[j].posi_act_x-1==foule[j].fin_x && foule[j].posi_act_y+1==foule[j].fin_y)
		{
			return 1;
		}
		//si sortie se trouve à droite
		else if(foule[j].posi_act_x+1==foule[j].fin_x && foule[j].posi_act_y==foule[j].fin_y)
		{
			return 1;
		}
		//si sortie se trouve à gauche
		else if(foule[j].posi_act_x-1==foule[j].fin_x && foule[j].posi_act_y==foule[j].fin_y)
		{
			return 1;
		}
		//si sortie se trouve en haut
		else if(foule[j].posi_act_x==foule[j].fin_x && foule[j].posi_act_y-1==foule[j].fin_y)
		{
			return 1;
		}
		//si sortie se trouve en bas
		else if(foule[j].posi_act_x==foule[j].fin_x && foule[j].posi_act_y+1==foule[j].fin_y)
		{
			return 1;
		}
		//si il est sur la sortie
		else
		{
			return 0;
		}
		
	
	
}

/******************************************************************************/
/* permet d'effacer l'ancienne position du joueur                             */
/******************************************************************************/

void effacement_joueur (int j)
{
		
	Point p1;
	if (plateau[foule[j].posi_act_x][foule[j].posi_act_y]==4)
	{
		plateau[foule[j].posi_act_x][foule[j].posi_act_y]=0;
		p1.x=foule[j].posi_act_x*UNITE;
		p1.y=foule[j].posi_act_y*UNITE;
		dessiner_rectangle(p1,UNITE,UNITE,noir);
	}
	
}

/******************************************************************************/
/* permet de dessiner la nouvelle position du joueur                          */
/******************************************************************************/

void dessiner_joueur(int j)
{

	Point p1;
	if (plateau[foule[j].posi_act_x][foule[j].posi_act_y]==0)	
	{
	plateau[foule[j].posi_act_x][foule[j].posi_act_y]=4;
	p1.x=foule[j].posi_act_x*UNITE; p1.y=foule[j].posi_act_y*UNITE;
	dessiner_rectangle(p1,UNITE,UNITE,foule[j].couleur);
	}

}

/******************************************************************************/
/* gère l'interface menu                                                      */
/******************************************************************************/

void menu()
{
        choix=-1;
        Point p1={0,0};
        Point p2;

        afficher_image("../images/menu.bmp",p1);
		actualiser();
        while (choix==-1)
        {
            p2=attendre_clic();
            if (START)
                choix=1;
            if (QUITTER)
                choix=3;

         }
         dessiner_rectangle(p1,800,600,noir);
}

/******************************************************************************/
/* lance la simulation                                                        */
/******************************************************************************/

void start()
{
	f=0;
    aff_plan();
    assignation_couleur();
    initialisation_posi_joueur();
	int i=0;
	
	attendre_clic();
	do 
	{
		attente(temps);
		deplacement_dirige();
		i++;
		
		
	} while (f!=NB_JOUEUR-1);
	
	aff_plan();
	fin(i);
}

/******************************************************************************/
/* lance le menu et gère les redirections vers les autres interfaces          */
/******************************************************************************/

void lancement_menu ()
{
    
		menu();
        if(choix==1)
        {
			assignation_couleur();
            start();
        }
        else if(choix==3)
			fermer_fenetre();
        

    

}

/******************************************************************************/
/* fin de simulation avec le nombre d'étapes et q redirection vers le menu    */
/******************************************************************************/

void fin(int i)
{
	Point p1={0,0};
	choix=-1;
	Point p2;
	dessiner_rectangle(p1,800,600,noir);
	afficher_image("../images/fin.bmp",p1);
	p1.x=345;p1.y=190;
	aff_entier(i,50,p1,blanc);
	actualiser();
	
	
	while(choix==-1)
	{
		p2=attendre_clic();
		if (MENU_FIN)
		{
			choix=0;
			lancement_menu();
		}
		if (QUITTER_FIN)
		{
			choix=3;
                	fermer_fenetre();
		}
	}
	
}

/******************************************************************************/
/* permet d'afficher a l'écran un entier                                      */
/******************************************************************************/

void aff_entier (int entier,int taille,Point coin,int couleur)
{
	char texte[5];
	sprintf(texte,"%d",entier); 
	afficher_texte(texte,taille,coin,couleur);
}



/******************************************************************************/
/* assigne une couleur selon l'entrée du joueur                               */
/******************************************************************************/

void assignation_couleur()
{
    int j;
    int r=entier_aleatoire(256);
    int g=entier_aleatoire(256);
    int b=entier_aleatoire(256);
   
     
    foule[0].couleur=fabrique_couleur(r,g,b);
     
    for (j=1;j<NB_JOUEUR+1;j++)
    {
         
        if (foule[j-1].depart_x!=foule[j].depart_x || foule[j-1].depart_y!=foule[j].depart_y)
        {
            r=entier_aleatoire(256);
            g=entier_aleatoire(256);
            b=entier_aleatoire(256);
            foule[j].couleur=fabrique_couleur(r,g,b);
          
             
        }
        else
        {
            foule[j].couleur=fabrique_couleur(r,g,b);
        }
    }  
}




void initialisation_posi_joueur()
{
	int j;
	for (j=0;j<NB_JOUEUR;j++)
	{
	foule[j].posi_act_x=foule[j].depart_x;
	foule[j].posi_act_y=foule[j].depart_y;
	}
}
