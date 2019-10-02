#include "structure.h"
#include "transformation.h"
#include "clavier.h"
#include "fenetre.h"



//fonction de rotations des surfaces
int rotate(char **argv){
  if(nb_arg(argv) == 3){
    if(verif_int(argv[1]) == 1){      
      int fen = atoi(argv[1]);
      int angle = atoi(argv[2]);
      if(angle%90 == 0){
	angle /= 90;
	listFen *tmp = get_fen(fen);
	if(tmp != NULL){
	  if(tmp -> img != NULL){
	    tmp -> img = rotateSurface90Degrees(tmp -> img, angle); //application de la même rotation à l'original
	    SDL_SetWindowSize(tmp -> fen, tmp -> img -> w, tmp -> img -> h);
	    blit_vide(tmp);
	    SDL_BlitSurface(tmp -> img, NULL, SDL_GetWindowSurface(tmp -> fen), NULL);
	    SDL_FreeSurface(SDL_GetWindowSurface(tmp -> fen));
	    SDL_UpdateWindowSurface(tmp -> fen);
	  } else printf("Aucune image dans la fenêtre n°%d\n", fen);
	} else printf("Fenêtre n°%d inexistante\n", fen);
      } else printf("L'angle doit être un multiple de 90 (90, 180, 360...)\n");
    } else printf("Les arguments de rotation doivent être des nombres (n° de fenêtre et angle)\n");
  } else printf("Arguments incorrects: rotate <n° fenêtre> <angle>\n");
  return 0;
}



//symétrie
int sym(char **argv, int code){
  if(nb_arg(argv) == 3){
    if(verif_int(argv[2]) == 1){
      listFen *tmp = get_fen(atoi(argv[2]));
      if(tmp != NULL){
	if(tmp -> img != NULL){
	  if(code == 1){
	  tmp -> img = zoomSurface(tmp -> img, -1, 1, 1); //application de la même rotation à l'original
	  } else if(code == 2){
	    tmp -> img = zoomSurface(tmp -> img, 1, -1, 1);
	  }
	  SDL_SetWindowSize(tmp -> fen, tmp -> img -> w, tmp -> img -> h);
	  blit_vide(tmp);
	  SDL_BlitSurface(tmp -> img, NULL, SDL_GetWindowSurface(tmp -> fen), NULL);
	  SDL_FreeSurface(SDL_GetWindowSurface(tmp -> fen));
	  SDL_UpdateWindowSurface(tmp -> fen);
	} else printf("Aucune image dans la fenêtre n°%s\n", argv[2]);
      } else printf("Fenêtre n°%s inexistante\n", argv[2]);
    } else {
      if(code == 1) printf("Arguments incorrects: sym vert <n° fenêtre>\n");
      else if(code == 2) printf("Arguments incorrects: sym horiz <n° fenêtre>\n");
    }
  } else {
    if(code == 1) printf("Arguments incorrects: sym vert <n° fenêtre>\n");
    else if(code == 2) printf("Arguments incorrects: sym horiz <n° fenêtre>\n");
  }
  return 0;
}



//change le niveau de luminosité d'un pixel
Uint8 change_lum(Uint8 c, int n){
  if(c + n > 255)
    return 255;
  else if(c + n < 0)
    return 0;
  else
    return c + n;
}



//change le niveau de contraste d'un pixel
Uint8 change_contrast(Uint8 c, double n){
  if(c <= 255 / 2)
    return (Uint8)( (255/2) * SDL_pow((double) 2 * c / 255, n));
  else
    return 255 - change_contrast(255 - c, n);
}

//moyenne (fonction auxiliaire pour la fonction flou)
Uint32 moyenne_pixels(SDL_Surface *surface, int i, int j, int n){
  int init_h = SDL_max(i - n, 0);
  int init_w = SDL_max(j - n, 0);
  int fin_h = SDL_min(i + n, surface->h - 1);
  int fin_w = SDL_min(j + n, surface->w - 1);
  int nb_pixel = ((fin_h-init_h) * (fin_w-init_w));
  Uint32 *p = surface->pixels;
  Uint32 rt = 0;
  Uint32 gt = 0;
  Uint32 bt = 0;
  SDL_Color color;

  for (i = init_h; i < fin_h; i++){
    for(j = init_w; j < fin_w; j++){
      SDL_GetRGB(p[i * surface->w + j], surface->format, &color.r, &color.g, &color.b);
      rt += color.r;
      gt += color.g;
      bt += color.b;
    }
  }

  return SDL_MapRGB(surface->format, rt/(nb_pixel), gt/(nb_pixel), bt/(nb_pixel));
}

//fonction de transformation des surfaces
int change_color(char **argv, int code){
  if(nb_arg(argv) == 2 || (nb_arg(argv) == 3 && (code == 3 || code == 4))){
    if(verif_int(argv[1]) == 1){      
      int fen = atoi(argv[1]);
      listFen *tmp = get_fen(fen);
      if(tmp != NULL){
	if(tmp -> img != NULL){
	  tmp -> img = SDL_ConvertSurfaceFormat(tmp -> img, SDL_PIXELFORMAT_RGBA8888, 0);
	  SDL_Surface *s = SDL_GetWindowSurface(tmp -> fen);
	  SDL_Surface *retour = NULL;
	  size_t i, j, w = (size_t) s ->w, h = (size_t) s ->h;
	  Uint32 *dst = NULL, *src = NULL;
	  SDL_Color color;
	  if(SDL_LockSurface(s) < 0) {
	    fprintf(stderr, "Erreur SDL_LockSurface : %s", SDL_GetError());
	    return 1;
	  }
	  dst = malloc((size_t)s ->pitch * h);
	  if(dst == NULL){
	    perror("Erreur malloc surface\n ");
	    return 1;
	  }
	  src = (Uint32 *) s->pixels;
	  if(code == 1){ //négatif
	    for(i = 0; i < h; i++){
	      for(j = 0; j < w; j++){
		SDL_GetRGB(src[i * w + j], s->format, &color.r, &color.g, &color.b);
		dst[i * w + j] = SDL_MapRGB(s->format, 255 - color.r, 255 - color.g, 255 - color.b);
	      }
	    }
	  } else if(code == 2){ //niveau de gris
	    for(i = 0; i < h; i++){
	      for(j = 0; j < w; j++){
		SDL_GetRGB(src[i * w + j], s->format, &color.r, &color.g, &color.b);
		Uint8 gris = (color.r + color.g + color.b) / 3;
		dst[i * w + j] = SDL_MapRGB(s->format, gris, gris, gris);
	      }
	    }
	  } else if(code == 3 || code == 4){ //luminosité/contraste
	    int neg = 0;
	    char *nb;
	    if(argv[2][0] == '-'){
	      neg = 1;
	      nb = get_number(argv[2]);
	    }
	    int n;
	    if(nb != NULL) n = atoi(nb);
	    if(verif_int(argv[2]) == 1) n = atoi(argv[2]);
	    for(i = 0; i < h; i++){
	      for(j = 0; j < w; j++){
		SDL_GetRGB(src[i * w + j], s->format, &color.r, &color.g, &color.b);
		if(code == 3){ //luminosité
		  if(neg == 1){
		    color.r = change_lum(color.r, -n);
		    color.g = change_lum(color.g, -n);
		    color.b = change_lum(color.b, -n);
		  } else {
		    color.r = change_lum(color.r, n);
		    color.g = change_lum(color.g, n);
		    color.b = change_lum(color.b, n);
		  }
		} else {
		  if(neg == 1){ //contraste
		    color.r = change_contrast(color.r, (double)-n);
		    color.g = change_contrast(color.g, (double)-n);
		    color.b = change_contrast(color.b, (double)-n);
		  } else {
		    color.r = change_contrast(color.r, (double)n);
		    color.g = change_contrast(color.g, (double)n);
		    color.b = change_contrast(color.b, (double)n);
		  }
		}
		dst[i * w + j] = SDL_MapRGB(s->format, color.r, color.g, color.b);
	      }
	    }
	    if(nb != NULL) free(nb);
	  } else if(code == 5){ //floutage
	    for(int i = 0; i < h; i++){
	      for(int j = 0; j < w; j++){
		dst[i * w + j] = moyenne_pixels(s, i, j, 2);
	      }
	    }
	  } else {
	    if(code == 3) printf("Arguments incorrects: lum <n° fenêtre> <valeur>\n");
	    if(code == 4) printf("Arguments incorrects: contraste <n° fenêtre> <valeur>\n");
	    if(code == 5) printf("Arguments incorrects: flou <n° fenêtre>\n");
	    return 0;
	  }
	  retour = SDL_CreateRGBSurfaceWithFormatFrom(dst, s->w, s->h, 32, s->pitch, s->format->format);
	  if(retour == NULL) {
	    fprintf(stderr, "Erreur SDL_CreateRGBSurface : %s", SDL_GetError());
	    return 1;
	  }
	  tmp -> img = retour;
	  SDL_SetWindowSize(tmp -> fen, tmp -> img -> w, tmp -> img -> h);
	  blit_vide(tmp);
	  SDL_FreeSurface(SDL_GetWindowSurface(tmp -> fen));
	  SDL_BlitSurface(tmp -> img, NULL, SDL_GetWindowSurface(tmp -> fen), NULL);
	  SDL_UpdateWindowSurface(tmp -> fen);
	  SDL_UnlockSurface(s);
	} else printf("Aucune image dans la fenêtre n°%d\n", fen);
      } else printf("Fenêtre n°%d inexistante\n", fen);
    } else printf("Arguments incorrects: negative/gris <n° fenêtre> \n");
  } else printf("Arguments incorrects: negative/gris <n° fenêtre> \n");
  return 0;
}


  
//remet l'image d'origine dans la fenêtre
int reset_img(char **argv){
  if(nb_arg(argv) == 2){
    if(verif_int(argv[1]) == 1){
      int fen = atoi(argv[1]);
      listFen *tmp = get_fen(fen);
      if(tmp != NULL){
	if(tmp -> img != NULL && tmp -> nom_img != NULL){
	  blit_vide(tmp);
	  tmp -> img = IMG_Load(tmp -> nom_img); //recharge l'image à partir de son nom
	  SDL_SetWindowSize(tmp -> fen, tmp -> img -> w, tmp -> img -> h);
	  SDL_BlitSurface(tmp -> img, NULL, SDL_GetWindowSurface(tmp -> fen), NULL);
	  SDL_UpdateWindowSurface(tmp -> fen);
	} else printf("Fenêtre n°%d vide\n", fen);
      } else printf("Fenêtre n°%d inexistante\n", fen);
    } else printf("Arguments incorrects: reset <n° fenêtre>\n");
  } else printf("Arguments incorrects: reset <n° fenêtre>\n");
  return 0;
}



//remplir une photo par couleur donnée
int remplissage(char **argv){
  if(nb_arg(argv) == 3 || nb_arg(argv) == 4){
    if(verif_int(argv[1]) == 1){      
      int fen = atoi(argv[1]);
      listFen *tmp = get_fen(fen);
      if(tmp != NULL){
	if(tmp -> img != NULL){ //image existante
	  SDL_Rect selec = { 0, 0, tmp->img->w, tmp->img->h };
	  if(nb_arg(argv) == 3){
	    Uint32 color;
	    if(strcmp(argv[2], "bleu") == 0) color = SDL_MapRGB(SDL_GetWindowSurface(tmp->fen)->format, 0, 0, 255);
	    else if(strcmp(argv[2], "rouge") == 0) color = SDL_MapRGB(SDL_GetWindowSurface(tmp->fen)->format, 255, 0, 0);
	    else if(strcmp(argv[2], "vert") == 0) color = SDL_MapRGB(SDL_GetWindowSurface(tmp->fen)->format, 0, 255, 0);
	    else if(strcmp(argv[2], "jaune") == 0) color = SDL_MapRGB(SDL_GetWindowSurface(tmp->fen)->format, 255, 255, 0);
	    else if(strcmp(argv[2], "orange") == 0) color = SDL_MapRGB(SDL_GetWindowSurface(tmp->fen)->format, 255, 165, 0);
	    else if(strcmp(argv[2], "rose") == 0) color = SDL_MapRGB(SDL_GetWindowSurface(tmp->fen)->format, 255, 192, 203);
	    else if(strcmp(argv[2], "violet") == 0) color = SDL_MapRGB(SDL_GetWindowSurface(tmp->fen)->format, 133, 21, 199);
	    else {
	      printf("Erreur, couleurs existantes: bleu, rouge, vert, jaune, orange, rose, violet\n");
	      return 0;
	    }
	    SDL_FillRect(SDL_GetWindowSurface(tmp->fen), &selec, color);
	    SDL_BlitSurface(SDL_GetWindowSurface(tmp->fen), &selec, SDL_GetWindowSurface(tmp->fen), &selec);
	  } else {
	    if(strcmp(argv[2], "-t") == 0){
	      Uint32 r, g, b;
	      SDL_SetSurfaceBlendMode(SDL_GetWindowSurface(tmp->fen), SDL_BLENDMODE_BLEND);
	      SDL_Surface* Srec2 = SDL_CreateRGBSurfaceWithFormat(0, tmp->img->w, tmp->img->h, 32, SDL_PIXELFORMAT_RGBA32);
	      SDL_SetSurfaceBlendMode(Srec2, SDL_BLENDMODE_BLEND);
	       if(strcmp(argv[3], "blanc") == 0){
		 r = 255;
		 g = 255;
		 b = 255;
	       } else if(strcmp(argv[3], "bleu") == 0){
		 r = 0;
		 g = 0;
		 b = 255;
	       } else if(strcmp(argv[3], "rouge") == 0){
		 r = 255;
		 g = 0;
		 b = 0;
	       } else if(strcmp(argv[3], "vert") == 0){
		 r = 0;
		 g = 255;
		 b = 0;
	       } else {
		 printf("Erreur, transparences existantes: blanc, bleu, rouge, vert\n");
		 return 0;
	       }
	       Uint32 color_trans_blanc = SDL_MapRGBA(Srec2->format, r, g, b, 40);
	       SDL_FillRect(Srec2, NULL, color_trans_blanc);
	       SDL_BlitSurface(SDL_GetWindowSurface(tmp->fen), &selec, SDL_GetWindowSurface(tmp->fen), &selec);
	       SDL_BlitSurface(Srec2, &selec, SDL_GetWindowSurface(tmp->fen), &selec);
	    } else {
	       printf("Erreur, format de la commande: remplissage [id] (-t) [couleur]\n");
	       return 0;
	    } 
	  }
	  tmp->img = SDL_GetWindowSurface(tmp->fen);
	  SDL_UpdateWindowSurface(tmp->fen);
	} else printf("Aucune image dans la fenêtre n°%d\n", fen);
      } else printf("Fenêtre n°%d inexistante\n", fen);
    } else printf("Erreur, format de la commande: remplissage [id] (-t) [couleur]\n");
  } else printf("Erreur, nombre d'arguments incorrect\n");
  return 0;
}



//sélection
int selection(char **argv){
  if(nb_arg(argv) == 2){
    if(verif_int(argv[1]) == 1){
      int fen = atoi(argv[1]);
      listFen *tmp = get_fen(fen);
      if(tmp != NULL){
	SDL_SetWindowInputFocus(tmp -> fen);
	int a = 1;
	int clic = SDL_FALSE; 
	int x1, x2, x3, y1, y2, y3;
	SDL_Rect selec;
	Uint32 color, *pixels_img, *pixels_cop;
	SDL_Surface *copie = NULL;

	SDL_Event event;
	while(SDL_PollEvent (&event)); //vider la boucle d'event
	while(a == 1){
	  while(SDL_PollEvent(&event)){
	    switch(event.type){
	      case SDL_MOUSEBUTTONDOWN: //appui du clic
		if(event.button.button == SDL_BUTTON_LEFT){ //point de sélection
		  x1 = event.button.x;
		  y1 = event.button.y;
		  selec.x = x1;
		  selec.y = y1;
		  clic = SDL_TRUE;
		}
		if(event.button.button == SDL_BUTTON_RIGHT){
		  x3 = event.button.x;
		  y3 = event.button.y;
		}
		break;

	    case SDL_MOUSEMOTION: //rectangle en cours (coordonnées changeantes)
	      if(clic){
		x2 = event.motion.x;
		y2 = event.motion.y;
		if(x1 < x2 && y1 < y2){ //sélection bas gauche à droite
		  selec.w = x2-x1;
		  selec.h = y2-y1;
		} else if(x1 > x2 && y1 < y2){ //sélection bas droite à gauche
		  selec.x = x1 - (x1-x2);
		  selec.w = x1-x2;
		  selec.h = y2-y1;
		} else if(x1 > x2 && y1 > y2){ //sélection haut droite à gauche
		  selec.x = x1 - (x1-x2);
		  selec.y = y1 - (y1-y2);
		  selec.w = x1-x2;
		  selec.h = y1-y2;
		} else { //sélection haut gauche à droite
		  selec.y = y1 - (y1-y2);
		  selec.w = x2-x1;
		  selec.h = y1-y2;
		}
	      
		color = SDL_MapRGBA(SDL_GetWindowSurface(get_win(event.motion.windowID)->fen)->format, 255, 255, 255, 10);
		//SDL_FillRect(SDL_GetWindowSurface(get_win(event.motion.windowID)->fen), &selec, color);
		SDL_BlitSurface(SDL_GetWindowSurface(get_win(event.motion.windowID)->fen), &selec, SDL_GetWindowSurface(get_win(event.motion.windowID)->fen), &selec);
		SDL_UpdateWindowSurface(get_win(event.motion.windowID)->fen);
	      }
	      break;

	    case SDL_MOUSEBUTTONUP: //relachement du clic
	      if(event.button.button == SDL_BUTTON_LEFT){
		printf("x=%d, y=%d, w=%d, h=%d\n", selec.x, selec.y, selec.w, selec.h);
		clic = SDL_FALSE;
	      }
	      break;

	    case SDL_KEYDOWN: //relachement d'une touche du clavier
	      switch(event.key.keysym.sym){

	      case SDLK_r: //touche R: reset
		blit_vide(get_win(event.motion.windowID));
		SDL_BlitSurface(tmp -> img, NULL, SDL_GetWindowSurface(get_win(event.motion.windowID)->fen), NULL);
		a = 0;
		break;
		
	      case SDLK_d: //touche D: déselection
		a = 0;
		break;	 

	    case SDLK_c: //touche C: colle
	      if(copie == NULL){
		if(x3 != 0 && y3 != 0){
		  copie = SDL_CreateRGBSurface(0, selec.w, selec.h, 32, 0, 0, 0, 0);
		  SDL_LockSurface(SDL_GetWindowSurface(get_win(event.button.windowID)->fen));
		  SDL_LockSurface(copie);
		  pixels_img = SDL_GetWindowSurface(get_win(event.button.windowID)->fen)->pixels;
		  pixels_cop = copie->pixels;
		  int ind = 0;
		  for(int i = selec.y; i < selec.y + selec.h; i++){
		    for(int j = selec.x; j < selec.x + selec.w; j++){
		      pixels_cop[ind] = pixels_img[i*(SDL_GetWindowSurface(get_win(event.button.windowID)->fen)->w)+j];
		      ind++;
		    }
		  }
		  SDL_UnlockSurface(SDL_GetWindowSurface(get_win(event.button.windowID)->fen));
		  SDL_UnlockSurface(copie);
		}	  
		selec.x = x3;
		selec.y = y3;
		SDL_BlitSurface(copie, NULL, SDL_GetWindowSurface(get_win(event.button.windowID)->fen), &selec);
		tmp->img = SDL_GetWindowSurface(tmp->fen);
		SDL_UpdateWindowSurface(get_win(event.button.windowID)->fen);
		copie = NULL;
	      }
	      break;

	      case SDLK_x: //touche X: coupe la partie sélectionnée (fond noir)
		if(copie == NULL){
		  if(x3 != 0 && y3 != 0){
		    copie = SDL_CreateRGBSurface(0, selec.w, selec.h, 32, 0, 0, 0, 0);
		    SDL_LockSurface(SDL_GetWindowSurface(get_win(event.button.windowID)->fen));
		    SDL_LockSurface(copie);
		    pixels_img = SDL_GetWindowSurface(get_win(event.button.windowID)->fen)->pixels;
		    pixels_cop = copie->pixels;
		    int ind = 0;
		    for(int i = selec.y; i < selec.y + selec.h; i++){
		      for(int j = selec.x; j < selec.x + selec.w; j++){
			pixels_cop[ind] = pixels_img[i*(SDL_GetWindowSurface(get_win(event.button.windowID)->fen)->w)+j];
			ind++;
		      }
		    }
		    SDL_UnlockSurface(SDL_GetWindowSurface(get_win(event.button.windowID)->fen));
		    SDL_UnlockSurface(copie);
		  }
		  color = SDL_MapRGB(SDL_GetWindowSurface(get_win(event.motion.windowID)->fen)->format, 0, 0, 0);
		SDL_FillRect(SDL_GetWindowSurface(get_win(event.motion.windowID)->fen), &selec, color);
		SDL_BlitSurface(SDL_GetWindowSurface(get_win(event.motion.windowID)->fen), &selec, SDL_GetWindowSurface(get_win(event.motion.windowID)->fen), &selec);
		  selec.x = x3;
		  selec.y = y3;
		  SDL_BlitSurface(copie, NULL, SDL_GetWindowSurface(get_win(event.button.windowID)->fen), &selec);
		}
		
		SDL_UpdateWindowSurface(get_win(event.motion.windowID)->fen);
		break;
	      }


	    default:
	      break;
	    }
	  }
	}
      } else printf("Fenêtre %d inexistante\n", fen);
    } else printf("Arguments incorrects: select <n° fenêtre>\n");
  } else printf("Arguments incorrects: select <n° fenêtre>\n");
  return 0;
}

