#include "jeu.h"

/////////////////////////////////////////////////////////
// Définition des structures et prototypes pour l'ia
/////////////////////////////////////////////////////////

extern const int INCONNUE;
extern const int GRILLE;
extern const int MINES;
extern const int bombe;
 
void CalculNbMine(carre **, int, int, int *, int *);
bool coherent(carre **, int *);
carre **etat_initial(carre **);
void backtracking(carre **, int, int, int *, int[GRILLE][GRILLE]);
void ia(carre **);