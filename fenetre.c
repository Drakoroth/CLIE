#include "structure.h"
#include "fenetre.h"
#include "clavier.h"



//récupère la largeur d'une fenêtre
int get_win_width(listFen *l){
  int width;
  SDL_GetWindowSize(l -> fen, &width, NULL);
  return width;
}



//récupère la hauteur d'une fenêtre
int get_win_height(listFen *l){
  int height;
  SDL_GetWindowSize(l -> fen, NULL, &height);
  return height;
}
    


//messages d'erreur pour SDL
void erreur(){
  //fprintf(stderr, "Erreur %s\n", SDL_GetError());
  SDL_Quit();
  exit(1);
}



//détruit une fenêtre et l'enlève de la liste
void close_window(listFen *l){
  SDL_FreeSurface(SDL_GetWindowSurface(l -> fen));
  SDL_DestroyWindow(l -> fen);
  SDL_FreeSurface(l -> img);
  if(l == first_fen){
    first_fen = l -> next;
  } else {
    if(l -> next != NULL) l -> prev -> next = l -> next;
    else l -> prev -> next = NULL;
    if(l -> prev != NULL && l -> next != NULL) l -> next -> prev = l -> prev;  
  }
  l -> prev = l -> next = NULL;
  free(l -> nom_img);
  free(l);
}



//destruction du rendu et de la fenêtre
void destroy_erreur(SDL_Window *f){
  SDL_FreeSurface(SDL_GetWindowSurface(f));
  SDL_DestroyWindow(f);
  erreur();
}



//crée et affiche une surface vide
void blit_vide(listFen *l){
  SDL_Surface *vide = SDL_CreateRGBSurface(0, get_win_width(l), get_win_height(l), 32, 0, 0, 0, 0); //création surface vide
  if(vide == NULL) {
    fprintf(stderr, "Erreur RGB Surface\n");
    SDL_FreeSurface(vide);
  } else {
    SDL_BlitSurface(vide, NULL, SDL_GetWindowSurface(l -> fen), NULL); //nouvelle surface vide sur l'ancienne fenêtre
    SDL_FreeSurface(vide);
  }
}



void update_all_win(){
  listFen *tmp = first_fen;
  while(tmp != NULL){
    SDL_UpdateWindowSurface(tmp -> fen);
  }
}



//ouvrir une fenêtre
int open_fen(){
  listFen *new_fen = cree_fen();
  new_fen -> fen = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, FEN_WIDTH, FEN_HEIGHT, SDL_WINDOW_RESIZABLE);
  if(new_fen -> fen == NULL) erreur();
  new_fen -> id_fen = SDL_GetWindowID(new_fen -> fen);
  new_fen -> id = new_fen -> id_fen -1;
  add_last(new_fen);  //ajout de la nouvelle cellule dans la liste
  char buf[200];
  sprintf(buf, "CIMP %d", new_fen ->id); //conversion int to char*
  SDL_SetWindowTitle((new_fen -> fen), buf);
  memset(buf, 0, sizeof(buf));
  return 0;
}



//ouvrir une image
int open_img(char *pic){
  listFen *new_fen = cree_fen();
  new_fen -> img = IMG_Load(pic); //chargement de l'image avec taille originale
  new_fen -> fen = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, new_fen -> img -> w, new_fen -> img -> h, SDL_WINDOW_RESIZABLE);
  if(new_fen -> fen == NULL) erreur();
  new_fen -> id_fen = SDL_GetWindowID(new_fen -> fen);
  new_fen -> id = new_fen -> id_fen -1;
  add_last(new_fen); //ajout de la nouvelle cellule dans la liste
  char buf[200];
  sprintf(buf, "CIMP %d  -  %s", new_fen->id, pic); //conversion int to char*
  SDL_SetWindowTitle((new_fen -> fen), buf);
  strcpy(new_fen -> nom_img, pic);
  memset(buf, 0, sizeof(buf));  
  if(new_fen -> img == NULL) destroy_erreur(new_fen -> fen);
  SDL_BlitSurface(new_fen -> img, NULL, SDL_GetWindowSurface(new_fen -> fen), NULL);//&(new_fen->rec)); //colle la surface sur la fenêtre
  SDL_UpdateWindowSurface(new_fen -> fen); //mise à jour de la fenêtre
  return 0;
}



//ouvrir une image dans une fenêtre déjà existante
int open_img_in_fen(char **argv){
  if((verif_int(argv[2]) == 1) && (verif_int(argv[3]) == 0)){
    listFen *tmp = get_fen(atoi(argv[2])); //récupération de la fenêtre
    if(tmp == NULL) fprintf(stderr, "Fenêtre n°%s inexistante\n", argv[2]);
    else{
      char buf[200];
      sprintf(buf, "CIMP %d  -  %s", tmp->id, argv[3]); //conversion int to char*
      SDL_SetWindowTitle((tmp -> fen), buf);
      if(tmp -> nom_img != NULL) { //si fenêtre non vide
	tmp -> nom_img = NULL;
	tmp -> nom_img = malloc(sizeof(char *));
      } else { //sinon fenêtre vide
	tmp -> nom_img = malloc(sizeof(char *));
      }
      strcpy(tmp -> nom_img, argv[3]);
      memset(buf, 0, sizeof(buf));
      tmp -> img = IMG_Load(argv[3]); //chargement de l'image
      if(tmp -> img == NULL) {
	fprintf(stderr, "%s: Image inexistante\n", argv[3]);
      } else {
	SDL_SetWindowSize(tmp -> fen, tmp -> img -> w, tmp -> img -> h);  
	SDL_FreeSurface(SDL_GetWindowSurface(tmp -> fen));
        blit_vide(tmp);	
	SDL_BlitSurface(tmp -> img, NULL, SDL_GetWindowSurface(tmp -> fen), NULL); //colle la surface sur la fenêtre
	SDL_FreeSurface(SDL_GetWindowSurface(tmp -> fen));
	SDL_UpdateWindowSurface(tmp -> fen);
      }
    }
  } else {
    fprintf(stderr, "Arguments incorrects\n");
  }
  return 0;
}



//ferme la fenêtre demandée
int close_fen(char **argv){
  if(nb_arg(argv) >= 2){
    for(int i = 1; i < nb_arg(argv); i++){
      if(verif_int(argv[i]) == 1){
	listFen *tmp = get_fen(atoi(argv[i]));
	if(tmp != NULL) close_window(tmp);
	else printf("Fenêtre inexistante\n");
      } else {
	printf("%s: Arguments incorrects: close <n° de fenêtre>\n", argv[i]);
      }
    }
  } else {
    printf("Arguments incorrects: close <n° de fenêtre>\n");
  }
  return 0;
}



//accepter les lettres minuscules, majuscules, les chiffres, les tirets et le point obligatoire
int syntaxe(char *name){
  int carac = 0;
  int point = 0;
  for(int i = 0; i < strlen(name); i++){
    if(name[i] == 45 || (name[i] >= 48 && name[i] <= 57) || (name[i] >= 65 && name[i] <= 90) || (name[i] == 95) || (name[i] >= 97 && name[i] <= 122)) carac++;
    if(name[i] == 46) point++;
  }
  if((carac == strlen(name)-1) && (point == 1)){
    int j = 0;
    while(name[j] != '.') j++;
    if(j > 0) return 0;
  }
  return 1;
}



//enregistrer une image
int save_img(char **argv){
  char buf[100];
  char *arg[4]; //tableau d'arguments pour le rechargement de l'image sauvegardée
  arg[0] = "open";
  arg[1] = "in";
  if(nb_arg(argv) < 2) printf("erreur: manque d'argument(s)\n");
  else if(nb_arg(argv) == 2){ //enregistrer l'image sous le même nom
    listFen *tmp = get_fen(atoi(argv[1]));
    if(tmp != NULL){
      if(tmp -> img == NULL) erreur();
      char str[100];
      strcpy(str, inv_str(tmp -> nom_img));
      tmp -> nom_img = inv_str(tmp -> nom_img); //retour à la forme initiale
      if(strncasecmp(str, "pmb.", 4) == 0) SDL_SaveBMP(tmp -> img, tmp -> nom_img);
      else if(strncasecmp(str, "gnp.", 4) == 0) IMG_SavePNG(tmp -> img, tmp -> nom_img);
      else if((strncasecmp(str, "gpj.", 4) == 0) || (strncasecmp(str, "gepj.", 4) == 0)) IMG_SaveJPG(tmp -> img, tmp -> nom_img, 100);
      else {
	printf("Erreur: format non traité, veuillez réessayer avec une autre extension\n");
	return 0;
      }
      SDL_FreeSurface(tmp -> img); //libération de mémoire
      tmp -> img = NULL;
      arg[2] = argv[1]; //id de la fenêtre
      arg[3] = tmp -> nom_img; //nom de l'image sauvegardée
      open_img_in_fen(arg); //rechargement de l'image avec le nouveau nom
    } else {
      fprintf(stderr, "Fenêtre n°%s inexistante\n", argv[1]);
    }
  } else if(nb_arg(argv) == 3){ //enregistrer l'image sous un autre nom
    struct stat s;
    if(syntaxe(argv[2]) == 0){
      listFen *tmp = get_fen(atoi(argv[1]));
      if(tmp != NULL){	
	if(tmp -> img == NULL) erreur();
	if(stat(argv[2], &s) == 0) remove(argv[2]); //écrasement
	char str[100];
	strcpy(str, inv_str(argv[2]));
        argv[2] = inv_str(argv[2]); //retour à la forme initiale
	if(strncasecmp(str, "pmb.", 4) == 0) SDL_SaveBMP(tmp -> img, argv[2]);
	else if(strncasecmp(str, "gnp.", 4) == 0) IMG_SavePNG(tmp -> img, argv[2]);
	else if((strncasecmp(str, "gpj.", 4) == 0) || (strncasecmp(str, "gepj.", 5) == 0)) IMG_SaveJPG(tmp -> img, argv[2], 100);
	else {
	  printf("Erreur: format non traité, veuillez réessayer avec une autre extension\n");
	  return 0;
	}
	tmp -> nom_img = NULL;
	tmp -> nom_img = malloc(sizeof(char *));
	sprintf(buf, "CIMP %d  -  %s", tmp->id, argv[2]); 
	strcpy(tmp -> nom_img, argv[2]);
	SDL_SetWindowTitle((tmp -> fen), buf);
	SDL_FreeSurface(tmp -> img); //libération de mémoire
	tmp -> img = NULL;
	arg[2] = argv[1]; //id de la fenêtre
	arg[3] = argv[2]; //nouveau nom de l'image sauvegardée
	open_img_in_fen(arg); //rechargement de l'image avec le nouveau nom
      } else {
	fprintf(stderr, "Fenêtre n°%s inexistante\n", argv[1]);
      }
    } else printf("Erreur: seuls les lettres minuscules, majuscules, les chiffres, les tirets sont acceptés, le point est obligatoire\n");	
  } else printf("Erreur: trop d'arguments\n");
  return 0;
}



//fonction de transfert des surfaces
int transfert_img(char **argv){
  char buf[200];
  if(nb_arg(argv) == 1) {
    printf("Erreur: manque d'argument(s)\n");
  } else if(nb_arg(argv) == 2){ //transférer dans une nouvelle fenêtre
    if(verif_int(argv[1]) == 1){
      listFen *t = get_fen(atoi(argv[1])); //récupération de la fenêtre par son id
      if(t == NULL) {
	fprintf(stderr, "Fenêtre n°%s inexistante\n", argv[1]);
      } else {	
	listFen *new_fen = cree_fen(); //nouvelle cellule 
	new_fen -> fen = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, t -> img -> w, t -> img -> h, SDL_WINDOW_RESIZABLE); //fenêtre de la nouvelle fenêtre où la surface va être transférée
	if(new_fen -> fen == NULL) erreur();	
	new_fen -> id_fen = SDL_GetWindowID(new_fen -> fen);
	new_fen -> id = new_fen -> id_fen -1;
	if(t -> nom_img != NULL){
	  sprintf(buf, "CIMP %d  -  %s", new_fen->id, t -> nom_img); //titre de la nouvelle fenêtre
	  strcpy(new_fen -> nom_img, t -> nom_img);
	  SDL_SetWindowTitle((new_fen -> fen), buf);
	} else {
	  sprintf(buf, "CIMP %d", new_fen->id);
	  SDL_SetWindowTitle((new_fen -> fen), buf);	    
	}
	t -> nom_img = NULL;
	new_fen -> img = t -> img;
	t -> img = NULL;
	add_last(new_fen); //ajout de la nouvelle cellule dans la liste
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "CIMP %d", t->id); //titre de l'ancienne fenêtre
	SDL_SetWindowTitle((t -> fen), buf);	  
	SDL_BlitSurface(SDL_GetWindowSurface(t -> fen), NULL, SDL_GetWindowSurface(new_fen -> fen), NULL); //copie de l'ancienne surface sur la nouvelle fenêtre
	blit_vide(t);
	SDL_UpdateWindowSurface(t -> fen); //mise à jour des fenêtres
	SDL_UpdateWindowSurface(new_fen -> fen);
      }
    } else {
      fprintf(stderr, "Erreur: l'argument doit être un entier\n");
    }
  } else if(nb_arg(argv) == 3){ //transférer dans une fenêtre déjà ouverte (vide ou occupée)
    if(verif_int(argv[1]) == 1 && verif_int(argv[2]) == 1){
      listFen *tmp = get_fen(atoi(argv[1])); //récupération de la fenêtre par son id
      listFen *tmp2 = get_fen(atoi(argv[2])); //récupération de la fenêtre par son id
      if(tmp == NULL && tmp2 == NULL) {
	fprintf(stderr, "Fenêtre n°%s et n°%s inexistantes\n", argv[1], argv[2]);
      } else if(tmp == NULL) {
	fprintf(stderr, "Fenêtre n°%s inexistante\n", argv[1]);
      } else if(tmp2 == NULL) {
	fprintf(stderr, "Fenêtre n°%s inexistante\n", argv[2]);
      } else {
	if(tmp -> nom_img != NULL){
	  sprintf(buf, "CIMP %d  -  %s", tmp2->id, tmp -> nom_img); //conversion int to char*
	  SDL_SetWindowTitle((tmp2 -> fen), buf);
	  tmp2 -> nom_img = NULL;
	  tmp2 -> nom_img = malloc(sizeof(char *));
	  strcpy(tmp2 -> nom_img, tmp -> nom_img); //copie du nom de l'image
	  tmp -> nom_img = NULL;
	} else {
	  sprintf(buf, "CIMP %d", tmp2->id); //conversion int to char*
	  SDL_SetWindowTitle((tmp2 -> fen), buf);
	  tmp2 -> nom_img = NULL;
	}	
	SDL_FreeSurface(tmp2 -> img);
	tmp2 -> img = tmp -> img;
	tmp -> img = NULL;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "CIMP %d", tmp->id); //conversion int to char*
	SDL_SetWindowTitle((tmp -> fen), buf);
	SDL_SetWindowSize(tmp2 -> fen, tmp2 -> img -> w, tmp2 -> img -> h);
	SDL_BlitSurface(SDL_GetWindowSurface(tmp -> fen), NULL, SDL_GetWindowSurface(tmp2 -> fen), NULL); //copie de l'ancienne surface sur la nouvelle fenêtre
	blit_vide(tmp);	
	SDL_UpdateWindowSurface(tmp -> fen); //mise à jour des fenêtres
	SDL_UpdateWindowSurface(tmp2 -> fen);	  
      }        
    } else {
      printf("Erreur: les arguments doivent être des entiers\n");
    }
  } else {
    printf("Erreur: trop d'arguments\n");
  }
  return 0;
}


