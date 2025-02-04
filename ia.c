#include "ia.h"

/////////////////////////////////////////////////////////
// IA par arbre de décision
/////////////////////////////////////////////////////////



void CalculNbMine(carre **grille, int x, int y, int* nbMineMax, int* nbMineMin) {
  // procédure qui compte le nombre de mines qu'il peut y avoir autour d'une case
  *nbMineMax = 0;
  *nbMineMin = 0;
  for (int i = x - 1; i <= x + 1; i++) {
    for (int j = y - 1; j <= y + 1; j++) {
      if (i >= 0 && i < GRILLE && j >= 0 && j < GRILLE) {
        if (!grille[j][i].decouvert) {
          (*nbMineMax)++;
        }
        if (grille[j][i].interieur == bombe) {
          (*nbMineMin)++;
        }
      }
    }
  }
}


bool coherent(carre** grille, int* nbMine){
  // fonction qui vérifie si le nombre de mines est cohérent avec le nombre de mines autour
  // fonction qui détermine si une grille peut aboutir à une solution complète ou non
  int MineMax, MineMin;
  *nbMine = 0;
  for (int y = 0; y < GRILLE; y ++) {
    for (int x = 0; x < GRILLE; x ++) {
      if (grille[y][x].interieur == bombe) {
        (*nbMine)++;
        if ((*nbMine) > MINES) {
          return false;
        }
      }
      else if (grille[y][x].interieur != INCONNUE) {
        CalculNbMine(grille, x, y, &MineMax, &MineMin);
        if ((MineMax < grille[y][x].interieur) || (grille[y][x].interieur < MineMin)) {
          return false;
        }
      }
    }
  }
  return true;
}


carre** etat_initial(carre **grille){
  // fonction qui renvoie la racine de l'arbre de décision, une grille où il n'y a que les données visibles pour le joueur
  carre** out = creerGrille();
  for (int y = 0; y < GRILLE; y ++) {
    for (int x = 0; x < GRILLE; x ++) {
      if (grille[y][x].decouvert) {
        out[y][x].decouvert = true;
        out[y][x].interieur = grille[y][x].interieur;
        out[y][x].flag = false;
      }
      else {
        out[y][x].decouvert = false;
        out[y][x].interieur = INCONNUE;
        out[y][x].flag = grille[y][x].flag;
      }
    }
  }
  return out;
}


void backtracking(carre **grille, int x, int y, int* nb_feuille, int grille_out[GRILLE][GRILLE]){
  // fonction qui construit l'arbre de décision
  if (x < GRILLE) {
    if (grille[y][x].interieur != INCONNUE) { //Si la case est déjà découverte
      backtracking(grille, x + 1, y, nb_feuille, grille_out);
    }
    else {
      int nbMine;
      if (coherent(grille, &nbMine)) {
        grille[y][x].decouvert = true;
        backtracking(grille, x + 1, y, nb_feuille, grille_out);
        grille[y][x].decouvert = false;
        grille[y][x].interieur = bombe;
        backtracking(grille, x + 1, y, nb_feuille, grille_out);
        grille[y][x].interieur = INCONNUE;
      }
    }
  }
  else {
    if (y < GRILLE - 1) { //Si on est pas arriver à la fin de la grille
      return backtracking(grille, 0, y + 1, nb_feuille, grille_out);
    }
    else {
      int nbMine;
      bool b1 = coherent(grille, &nbMine);
      if (b1 && (nbMine == MINES)) {
        (*nb_feuille) ++;
        for (int yp = 0; yp < GRILLE; yp ++) {
          for (int xp = 0; xp < GRILLE; xp ++) {
            if (grille[yp][xp].interieur == bombe) {
              grille_out[yp][xp] --;
            }
            else {
              grille_out[yp][xp] ++;
            }
          }
        }
      }
    }
  }
}


void ia(carre** grille){
  // procédure qui affiche le meilleur coup à jouer
  int nb_feuille = 0;
  int grille_out[GRILLE][GRILLE];
  for (int y = 0; y < GRILLE; y ++) {
    for (int x = 0; x < GRILLE; x ++) {
      grille_out[y][x] = 0;
    }
  }

  carre **grilleDeSortie = etat_initial(grille);
  backtracking(grilleDeSortie, 0, 0, &nb_feuille, grille_out);
  detruireGrille(grilleDeSortie);

  int ymax = -1;
  int xmax = -1;
  int valmax = -1;
  for (int y = 0; y < GRILLE; y ++) {
    for (int x = 0; x < GRILLE; x ++) {
      if (!(((grille_out[y][x] < 0) && grille[y][x].flag)
        || ((grille_out[y][x] > 0) && grille[y][x].decouvert))) {
        if (abs(grille_out[y][x]) > valmax) {
          valmax = abs(grille_out[y][x]);
          ymax = y;
          xmax = x;
        }
      }
    }
  }

  printf("\nJe te conseille le coup ");
  if (grille_out[ymax][xmax] < 0) {
    printf("poser drapeau (p)");
  }
  else {
    if (grille[ymax][xmax].flag) {
      printf("retirer drapeau (d)");
    }
    else {
      printf("révéler case (c)");
    }
  }
  printf(" %d %d (sur à %d%%)\n", xmax, ymax, (valmax*100) / nb_feuille);
}