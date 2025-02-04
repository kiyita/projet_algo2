#include "ia.h"

/////////////////////////////////////////////////////////
// Jeu
/////////////////////////////////////////////////////////

// Déclarations variables globales
const int bombe = 99;      // entier correspond à une bombe
const int INCONNUE = -100; // pour l'ia quand pas assez d'information
const int GRILLE = 7;      // taille de la grille, supérieur à 3
const int MINES = 10;       // nombre de mines, il faut que cette valeur soit
                           // inférieure à (GRILLExGRILLE - 9) pour assurer la
                           // possibilité d'une case avec un zéro
const char *indexation =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; // pour
                                                                      // numéroter
                                                                      // les
                                                                      // cases

// ANSI COLOR SCHEME
// couleurs pour l'esthétique du jeu
#define ANSI_COLOR_RED "\x1b[0;31m"
#define ANSI_COLOR_RED_DIM "\x1b[2;31m"
#define ANSI_COLOR_GREEN "\x1b[0;32m"
#define ANSI_COLOR_GREEN_DIM "\x1b[2;32m"
#define ANSI_COLOR_YELLOW "\x1b[0;33m"
#define ANSI_COLOR_BLUE "\x1b[0;34m"
#define ANSI_COLOR_BLUE_DIM "\x1b[2;34m"
#define ANSI_COLOR_MAGENTA "\x1b[0;35m"
#define ANSI_COLOR_CYAN "\x1b[0;36m"
#define ANSI_COLOR_WHITE "\x1b[0;37m"
#define ANSI_COLOR_GRAY "\x1b[0;2m"
#define ANSI_RESET "\x1b[0;0m"
#define ANSI_BACKGROUND_GRAY "\x1b[48;5;245m"

// Esthétique de la grille de jeu et la numérotation des cases

const char *couleurNumero(int numero) {
  // fonction qui renvoie la couleur correspondant au numéro
  switch (numero) {
  case 1:
    return ANSI_COLOR_BLUE;
  case 2:
    return ANSI_COLOR_GREEN_DIM;
  case 3:
    return ANSI_COLOR_RED;
  case 4:
    return ANSI_COLOR_BLUE_DIM;
  case 5:
    return ANSI_COLOR_RED_DIM;
  case 6:
    return ANSI_COLOR_CYAN;
  case 7:
    return ANSI_COLOR_MAGENTA;
  default:
    return ANSI_COLOR_GRAY;
  }
}

void afficherCarre(carre carre) {
  // fonction qui affiche les cases avec les bonnes couleurs
  if (carre.interieur == INCONNUE) {
    printf(ANSI_COLOR_YELLOW);
    printf("?");
  } else {
    if (carre.decouvert) {
      if (carre.interieur == bombe) {
        printf("M");
      } else if (carre.interieur == 0) {
        printf(" ");
      } else {
        printf("%s", couleurNumero(carre.interieur));
        printf("%d", carre.interieur);
      }
    } else {
      if (carre.flag) {
        printf(ANSI_COLOR_RED);
        printf("D");
      } else {
        printf(ANSI_COLOR_GREEN);
        printf("#");
      }
    }
  }
  printf(ANSI_RESET);
}

// Bases pour créer, afficher et manipuler la grille

int indice_indexation_auxiliaire(int i, char c) {
  // fonction qui renvoie l'indice de la lettre dans la chaine indexation
  if (indexation[i] == c) {
    return i;
  } else if (i < GRILLE) {
    return indice_indexation_auxiliaire(i + 1, c);
  } else {
    return -1;
  }
}

int indice_indexation(char c) {
  // fonction qui lance la fonction récursive indice_indexation_auxiliaire
  return indice_indexation_auxiliaire(0, c);
}

carre **creerGrille() {
  // fonction qui initilise la grille
  carre **grille = malloc((size_t)GRILLE * sizeof(carre *));
  for (int i = 0; i < GRILLE; i++) {
    grille[i] = malloc((size_t)GRILLE * sizeof(carre));
    for (int j = 0; j < GRILLE; j++) {
      grille[i][j].flag = false;
      grille[i][j].decouvert = false;
    }
  }
  return grille;
}

void afficherGrille(carre **grille) {
  // procédure qui affiche la grille
  printf("\n");

  printf(ANSI_BACKGROUND_GRAY);
  for (int i = 0; i < GRILLE; i++) {
    printf("%c ", indexation[i]);
  }
  printf(ANSI_RESET);
  printf("\n");
  for (int y = 0; y < GRILLE; y++) {
    for (int x = 0; x < GRILLE; x++) {
      afficherCarre(grille[y][x]);
      printf(" ");
    }
    printf(ANSI_BACKGROUND_GRAY);
    printf("%c", indexation[y]);
    printf(ANSI_RESET);
    printf("\n");
  }

  printf("\n");
}

void detruireGrille(carre **grille) {
  // procédure qui libère la mémoire allouée pour la grille
  for (int y = 0; y < GRILLE; y++) {
    free(grille[y]);
  }
  free(grille);
}

void poseMines(carre **grille) {
  // procédure qui pose les mines aléatoirement
  int mines = 0;
  while (mines < MINES) {
    int x = rand() % GRILLE;
    int y = rand() % GRILLE;
    if (grille[y][x].interieur != bombe) {
      grille[y][x].interieur = bombe;
      mines++;
    }
  }
}

int nbMinesVoisines(carre **grille, int x, int y) {
  // fonction qui renvoie le nombre de mines voisines d'une case
  int nbMines = 0;
  for (int i = x - 1; i <= x + 1; i++) {
    for (int j = y - 1; j <= y + 1; j++) {
      if (i >= 0 && i < GRILLE && j >= 0 && j < GRILLE) {
        if (grille[j][i].interieur == bombe) {
          nbMines++;
        }
      }
    }
  }
  return nbMines;
}

void remplissage(carre **grille) {
  // procédure qui rempli la grille avec les mines voisines
  for (int i = 0; i < GRILLE; i++) {
    for (int j = 0; j < GRILLE; j++) {
      if (grille[j][i].interieur != bombe) {
        grille[j][i].interieur = nbMinesVoisines(grille, i, j);
      }
    }
  }
}

// Actions de l'utilisateur

void poseDrapeau(carre **grille, int x, int y, bool *loop_case) {
  // procédure qui pose un drapeau sur une case
  if (grille[y][x].decouvert) {
    printf("La case est découverte, vous ne pouvez pas placer de drapeau\n");
    *loop_case = true;
  } else if (grille[y][x].flag) {
    printf("La case est déjà marquée\n");
    *loop_case = true;
  } else {
    *loop_case = false;
    grille[y][x].flag = true;
  }
}

void retireDrapeau(carre **grille, int x, int y, bool *loop_case) {
  // procédure qui retire un drapeau sur une case
  if (grille[y][x].decouvert) {
    printf("La case est découverte, vous ne pouvez pas retirer de drapeau\n");
    *loop_case = true;
  } else if (grille[y][x].flag) {
    *loop_case = false;
    grille[y][x].flag = false;
  } else {
    printf("La case n'a pas de drapeau, vous ne pouvez pas le retirer\n");
    *loop_case = true;
  }
}

// Fin de partie

void recommencer(void) {
  // procédure qui demande et relance le jeu
  printf("Voulez-vous rejouer ? (O/N)\n");
  char reponse;
  scanf(" %c", &reponse);
  fflush(stdin);
  if (reponse == 'O' || reponse == 'o') {
    jeu();
  }
}

bool finDuJeuPerdu(void) {
  // fonction lancée quand on a perdu
  printf("Fin du jeu ! Vous avez perdu !\n");
  return false;
}

bool finDuJeuGagne(void) {
  // fonction lancée quand on a gagné
  printf("Fin du jeu ! Vous avez gagné !\n");
  return false;
}

// Fonctionnement du jeu

void propagationDuZero(carre **grille, int x, int y) {
  // procédure qui découvre toutes les cases environnante à un 0 de manière
  // récursive
  for (int i = x - 1; i <= x + 1; i++) {
    for (int j = y - 1; j <= y + 1; j++) {
      if (i >= 0 && i < GRILLE && j >= 0 && j < GRILLE) {
        if (grille[j][i].decouvert == false) {
          grille[j][i].decouvert = true;
          if (grille[j][i].interieur == 0) {
            propagationDuZero(grille, i, j);
          }
        }
      }
    }
  }
}

bool toutesCasesDecouvertes(carre **grille) {
  // fonction qui renvoie true si toutes les cases de la grille sont découvertes
  for (int i = 0; i < GRILLE; i++) {
    for (int j = 0; j < GRILLE; j++) {
      if (grille[j][i].decouvert == false && grille[j][i].interieur != bombe) {
        return false;
      }
    }
  }
  return true;
}

bool decouvreCase(carre **grille, int x, int y, bool *loop_case) {
  // fonction qui découvre une case et prend une décision en conséquence
  if (grille[y][x].decouvert) {
    printf("La case est déjà découverte, recommencer s'il vous plaît\n");
    *loop_case = true;
  } else if (grille[y][x].flag) {
    printf("La case est marquée, il faut retirer le drapeau pour pouvoir la "
           "découvrir\n");
    *loop_case = true;
  } else {
    *loop_case = false;
    grille[y][x].decouvert = true;
    if (grille[y][x].interieur == bombe) {
      afficherGrille(grille);
      return finDuJeuPerdu();
    } else if (grille[y][x].interieur == 0) {
      propagationDuZero(grille, x, y);
    } else if (toutesCasesDecouvertes(grille)) {
      return finDuJeuGagne();
    }
  }
  return true;
}

// Pour la gestion du premier coup creusage

bool dansPerimetre0(int x2, int y2, int x, int y) {
  // fonction qui renvoie true si la case est dans le perimetre du 0
  return (x2 >= x - 1 && x2 <= x + 1 && y2 >= y - 1 && y2 <= y + 1);
}

void poseMinesCoup1(carre **grille, int x, int y) {
  // procédure qui pose les mines aléatoirement mais assure que la case en x, y soit un 0
  int mines = 0;
  while (mines < MINES) {
    int x_mine = rand() % GRILLE;
    int y_mine = rand() % GRILLE;
    if (grille[y_mine][x_mine].interieur != bombe &&
        !dansPerimetre0(x_mine, y_mine, x, y)) {
      grille[y_mine][x_mine].interieur = bombe;
      mines++;
    }
  }
}

void gestionPremierCoup(carre **grille, int x, int y) {
  // procédure qui gère le premier coup et assure que ce soit un zéro
  if (grille[y][x].interieur != 0) {
    for (int i = 0; i < GRILLE; i++) {
        for (int j = 0; j < GRILLE; j++) {
            grille[i][j].interieur = -1;
        }
    }
    grille[y][x].interieur = 0;
    poseMinesCoup1(grille, x, y);
    remplissage(grille);
  }
}

// Dialogue avec l'utilisateur et boucle de jeu

bool dialogueUtilisateur(carre **grille, int *coup) {
  // fonction pour le choix de l'action par le joueur
  char action;
  bool loop_action = false;
  do {
    printf("Choississez une action parmi: Révéler une case (c), Placer un drapeau (p), Retirer un drapeau (d) ou Demander ia (a): ");
    scanf("\n%c", &action);
    switch (action) {
    case 'c':
    case 'p':
    case 'd':
      loop_action = false;
      break;
    case 'a':
      if (*coup == 1) {
        printf("\nJouez un premier coup avant d'utiliser l'IA\n");
      }
      else {
        ia(grille);
      }
      break;
    default:
      printf("Action invalide, recommencer s'il vous plaît\n");
      loop_action = true;
    }
  } while (loop_action);
  bool out = true;
  if (action != 'a') {
    bool loop_case = false;
    do {
      char car;
      int x, y;
      printf("Choississez une colonne: ");
      scanf("\n%c", &car);
      x = indice_indexation(car);
      printf("Choississez une ligne: ");
      scanf("\n%c", &car);
      y = indice_indexation(car);
      if ((x >= GRILLE) || (x <= -1) || (y >= GRILLE) || (y <= -1)) {
        printf("La case est invalide, recommencer s'il vous plaît\n");
        loop_case = true;
      } else
        switch (action) {
        case 'c':
          if (*coup == 1) {
            gestionPremierCoup(grille, x, y);
            *coup = 0;
          }
          out = decouvreCase(grille, x, y, &loop_case);
          break;
        case 'p':
          poseDrapeau(grille, x, y, &loop_case);
          break;
        case 'd':
          retireDrapeau(grille, x, y, &loop_case);
          break;
        default:
          printf("Action impossible\n"); //Étant donnée qu'on a déjà vérifier
                                         // l'action, on ne devrait jamais
                                         // arriver ici
        }
    } while (loop_case);
  }
  return out;
}

void jeu(void) {
  // procédure qui lance le jeu et mesure le score
  carre **grille = creerGrille();
  poseMines(grille);
  remplissage(grille);
  srand((unsigned int)time(NULL)); // pour changer a graine du random
  int coup = 1;
  clock_t t;
  t = clock();
  bool run = true;
  while (run) {
    afficherGrille(grille);
    run = dialogueUtilisateur(grille, &coup);
  }
  t = clock() - t;
  double time_taken = ((double)t) / CLOCKS_PER_SEC;
  printf("Votre score/durée de jeu est %f \n", time_taken);
  detruireGrille(grille);
  recommencer();
}