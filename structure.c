#include "structure.h"



//créé une nouvelle cellule contenant une fenêtre
listFen *cree_fen(){
  listFen *new_f = malloc(sizeof(listFen));
  if(new_f == NULL) return NULL;
  new_f -> nom_img = malloc(sizeof(char*));
  new_f -> fen = NULL;
  new_f -> img = NULL;
  new_f -> next = NULL;
  new_f -> prev = NULL;
  return new_f;
}



//retourne la dernière fenêtre ouverte
listFen *get_last_fen(){
  listFen *res = first_fen;
  while(res -> next != NULL){
    res = res -> next;
  }
  return res;
}



//retourne une fenêtre depuis son id de liste (cellule)
listFen *get_fen(int id){
  listFen *tmp = first_fen;
  while(tmp != NULL){
    if((tmp -> id) == id){
      return tmp;
    } else {
      tmp = tmp -> next;
    }
  }
  return NULL;
}



//retourne une fenêtre depuis son id de fenêtre
listFen *get_win(int id){
  listFen *tmp = first_fen;
  while(tmp != NULL){
    if(SDL_GetWindowID(tmp -> fen) == id){
      return tmp;
    } else {
      tmp = tmp -> next;
    }
  }
  return NULL;
}



//ajoute une nouvelle cellule à la fin de la liste de fenêtres
void add_last(listFen *l){
  if(l != NULL){
    if(first_fen == NULL){
      first_fen = l;
      printf("Fenêtre n°%d ajoutée\n", l -> id);
    } else {
      listFen *tmp = first_fen;
      while(tmp -> next != NULL){
	tmp = tmp -> next;
      }
      tmp -> next = l;
      l -> prev = tmp;
      printf("Fenêtre n°%d ajoutée\n", l -> id);
    }
  } else {
    printf("Ajout impossible\n");
  }
}



//afficher pour tester
void printList(){
  if(first_fen != NULL){
    listFen *tmp = first_fen;
    while(tmp != NULL){
      printf(" %d ->", tmp -> id);    
      tmp = tmp -> next;
    }
    printf(" FIN\n");
  } else {
    printf("Pas de liste\n");
  }
}



//longueur liste
int list_length(){
  if(first_fen == NULL){
    return 0;
  }
  int len = 1;
  listFen *tmp = first_fen;
  while(tmp -> next != NULL){
    tmp = tmp -> next;
    len++;
  }
  return len;
}



