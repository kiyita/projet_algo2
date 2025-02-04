build :
	gcc -W -Wall -Wextra -Wwrite-strings -Wconversion -fsanitize=address -std=c99 -Werror -g -o main main.c ia.c jeu.c