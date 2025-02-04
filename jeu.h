#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/////////////////////////////////////////////////////////
// Définition des structures et prototypes pour le jeu
/////////////////////////////////////////////////////////

typedef struct {
  bool flag;
  bool decouvert;
  int interieur;
} carre;

const char *couleurNumero(int);
void afficherCarre(carre);
int indice_indexation_auxiliaire(int, char);
int indice_indexation(char);
carre **creerGrille();
void afficherGrille(carre **);
void detruireGrille(carre **);
void poseMines(carre **);
int nbMinesVoisines(carre **, int, int);
void remplissage(carre **);
void poseDrapeau(carre **, int, int, bool*);
void retireDrapeau(carre **, int, int, bool*);
void recommencer(void);
bool finDuJeuPerdu(void);
bool finDuJeuGagne(void);
void propagatationDuZero(carre **, int, int);
bool toutesCasesDecouvertes(carre **);
bool decouvreCase(carre **, int, int, bool*);
bool dansPerimetre0(int x2, int y2, int x, int y);
void poseMinesCoup1(carre **grille, int x, int y);
void gestionPremierCoup(carre **, int, int);
bool dialogueUtilisateur(carre **, int*);
void jeu(void);