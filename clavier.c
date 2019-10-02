#include "structure.h"
#include "clavier.h"
#include "fenetre.h"
#include "transformation.h"



//compteur pour récupérer le nombre d'arguments d'une ligne
int nb_arg(char **argv){
  int i = 0;
  while(argv[i] != NULL) i++;
  return i;
}



//renvoie un tableau avec chaque élément de la ligne
char **split_line(char *line){
  char **tokens;
  char *token;
  int pos = 0;
  if((tokens = malloc(sizeof(char*)*500)) == NULL) exit(1);
  token = strtok(line, " ");
  while(token != NULL){
    tokens[pos++] = token;
    token = strtok(NULL, " ");
  }
  tokens[pos] = NULL; //dernier élément
  return tokens;
}



//return 1 s'il s'agit d'un entier, 0 sinon
int verif_int(char *c){
  int cp = 0;
  for(int i = 0; i < strlen(c); i++){
    if(isdigit(c[i]) != 0) cp++;    
  }
  if(cp == strlen(c)) return 1;
  return 0;
}



//récupère le nombre sans le '-'
char *get_number(char *c){
  char *res = malloc((strlen(c)-1)*sizeof(char));
  for(int i = 0; i < (strlen(c)-1); i++){
    res[i] = c[i+1];
  }
  return res;
}



//inverse une chaine de caractères
char *inv_str(char *str){
  int i = strlen(str)-1;
  int j = 0;
  char ch;
  while (i > j){
    ch = str[i];
    str[i] = str[j];
    str[j] = ch;
    i--;
    j++;
  }
  return str;
}



//reconnaissance des commandes
int cmd(char **argv){
  //char **argv = (char **)data;
  if(nb_arg(argv) == 0) return 0; //ligne vide
  else if(strcmp(argv[0], "exit") == 0) exit(1);
  else if((nb_arg(argv) == 4) && (strcmp(argv[0], "open") == 0) && (strcmp(argv[1], "in") == 0)) open_img_in_fen(argv);
  else if((nb_arg(argv) == 2) && (strcmp(argv[0], "open") == 0) && (strcmp(argv[1], "fen") == 0)) open_fen(); //ouvre une fenêtre
  else if((nb_arg(argv) == 1) && (strcmp(argv[0], "open") == 0)) printf("erreur: manque d'arguments\n");
  else if((nb_arg(argv) > 1) && (strcmp(argv[0], "open") == 0)){
    struct stat s;
    for(int i = 1; i < nb_arg(argv); i++){
      char str[100];
      strcpy(str, inv_str(argv[i]));
      argv[i] = inv_str(argv[i]); //retour à la forme initiale
      if(stat(argv[i], &s) == -1){ //vérification de l'existance d'un fichier
	printf("« %s » n'est pas une image existante\n", argv[i]);
      } else if((strncasecmp(str, "pmb.", 4) != 0) && (strncasecmp(str, "gpj.", 4) != 0) && (strncasecmp(str, "gepj.", 4) != 0) && (strncasecmp(str, "gnp.", 4) != 0)){
	printf("« %s » n'a pas un format image\n", argv[i]);	
      } else {
        open_img(argv[i]); //ouvre une image
      }
    }
  }  
  else if((nb_arg(argv) >= 1) && (strcmp(argv[0], "save") == 0)) save_img(argv); //enregistrer une image
  else if((nb_arg(argv) >= 1) && (strcmp(argv[0], "transfert") == 0)) transfert_img(argv);
  else if((nb_arg(argv) >= 1) && (strcmp(argv[0], "rotate") == 0)) rotate(argv);
  else if(((nb_arg(argv) >= 1) && (strcmp(argv[0], "sym") == 0)  && (strcmp(argv[1], "vert") == 0))) sym(argv, 1);
  else if(((nb_arg(argv) >= 1) && (strcmp(argv[0], "sym") == 0)  && (strcmp(argv[1], "horiz") == 0))) sym(argv, 2);
  else if(((nb_arg(argv) >= 1) && (strcmp(argv[0], "negative") == 0))) change_color(argv, 1);
  else if(((nb_arg(argv) >= 1) && (strcmp(argv[0], "gris") == 0))) change_color(argv, 2);
  else if(((nb_arg(argv) >= 1) && (strcmp(argv[0], "lum") == 0))) change_color(argv, 3);
  else if(((nb_arg(argv) >= 1) && (strcmp(argv[0], "contraste") == 0))) change_color(argv, 4);
  else if((nb_arg(argv) >= 1) && (strcmp(argv[0], "flou") == 0)) change_color(argv, 5);
  else if((nb_arg(argv) >= 1) && (strcmp(argv[0], "close") == 0)) close_fen(argv);
  else if((nb_arg(argv) >= 1) && (strcmp(argv[0], "select") == 0)) selection(argv);
  else if((nb_arg(argv) >= 1) && (strcmp(argv[0], "remplissage") == 0)) remplissage(argv);
  else if((nb_arg(argv) >= 1) && (strcmp(argv[0], "reset") == 0)) reset_img(argv);
  else printf("commande introuvable...\n");
  return 0;
}


