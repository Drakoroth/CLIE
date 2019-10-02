#include "structure.h"
#include "main.h"
#include "clavier.h"
#include "fenetre.h"
#include "transformation.h"



//fait tourner le terminal
void my_term(){
  int flags = IMG_INIT_JPG|IMG_INIT_PNG;
  int initted = IMG_Init(flags);
  if((initted&flags) != flags){
    printf("IMG_Init: Failed to init required jpg and png support!\n");
    printf("IMG_Init: %s\n", IMG_GetError());
    //handle error
  }
  char *line;
  char **argv;
  int status;
  while(status >= 0){
    line = readline("> "); //lit la ligne
    if(line[strlen(line)-1] == '\n') line[strlen(line)-1] = '\0'; //supprime le saut de ligne
    argv = split_line(line); 
    status = cmd(argv);
    printList(); //test
    free(line);
    free(argv);
  }
  free(first_fen -> nom_img);
  free(first_fen);
  IMG_Quit();
}



int main(int argc, char **argv){
  if(SDL_Init(SDL_INIT_VIDEO) != 0) erreur();
  my_term();
  atexit(SDL_Quit);
}


