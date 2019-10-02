#ifndef fenetre_h
#define fenetre_h 

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jpeglib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FEN_WIDTH 500
#define FEN_HEIGHT 500


int get_win_width(listFen *l);
int get_win_height(listFen *l);
void erreur();
void close_window(listFen *l);
void destroy_erreur(SDL_Window *f);
void blit_vide(listFen *l);
void update_all_win();
int open_fen();
int open_img(char *pic);
int open_img_in_fen(char **argv);
int close_fen(char **argv);
int syntaxe(char *name);
int save_img(char **argv);
int transfert_img(char **argv);

#endif
