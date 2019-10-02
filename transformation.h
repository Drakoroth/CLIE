#ifndef transformation_h
#define transformation_h 
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL2/SDL2_rotozoom.h>
#include <stdio.h>

int rotate(char **argv);
int sym(char **argv, int code);
Uint8 change_lum(Uint8 c, int n);
Uint8 change_contrast(Uint8 c, double n);
int change_color(char **argv, int code);
int change_size(char **argv);
int reset_img(char **argv);
int remplissage(char **argv);
Uint32 moyenne_pixels(SDL_Surface *surface, int i, int j, int n);
int selection(char **argv);

#endif
