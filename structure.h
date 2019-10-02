#ifndef structure_h
#define structure_h 
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL2/SDL2_rotozoom.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <jpeglib.h>
#include <pthread.h>
#include <readline/readline.h>

typedef struct listFen {
  int id;
  int id_fen;
  char *nom_img;
  SDL_Window *fen;
  SDL_Surface *img;
  SDL_Rect rec;
  struct listFen *next;
  struct listFen *prev;
}listFen;


//première cellule de la liste de fenêtres
listFen *first_fen;

listFen *cree_fen();
listFen *get_last_fen();
listFen *get_fen(int id);
listFen *get_win(int id);
void add_last(listFen *l);
void printList();
int list_length();

#endif
