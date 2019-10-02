#ifndef clavier_h
#define clavier_h 
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

int nb_arg(char **argv);
char **split_line(char *line);
int verif_int(char *c);
int cmd(char **argv);
char *get_number(char *c);
char *inv_str(char *str);

#endif
