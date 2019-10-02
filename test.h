#ifndef TEST_H
#define TEST_H
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include "structure.h"
#include "main.h"
#include "clavier.h"
#include "fenetre.h"
#include "transformation.h"

char* str3[] = {"open", "in", "1", "lapin.jpg"};
char* str4[] = {"rotate", "1", "90"};
char* str5[] = {"gris", "1"};


START_TEST(test1){
  int res = open_fen();
  ck_assert_int_eq(res, 0);
}
END_TEST


START_TEST(test2){
  int res = open_img("lapin.jpg");
  ck_assert_int_eq(res, 0);
}
END_TEST


START_TEST(test3){
  open_fen();
  int res = open_img_in_fen(str3);
  ck_assert_int_eq(res, 0);
}
END_TEST


START_TEST(test4){
  open_img("lapin.jpg");
  int res = rotate(str4);
  ck_assert_int_eq(res, 0);
}
END_TEST


START_TEST(test5){
  open_img("lapin.jpg");
  int res = change_color(str5, 1);
  ck_assert_int_eq(res, 0);
}
END_TEST


Suite *classe(void){
  TCase *tc;
  Suite *s;

  tc = tcase_create(" test unitaire ");
  tcase_add_test(tc, test1);
  tcase_add_test(tc, test2);
  tcase_add_test(tc, test3);
  tcase_add_test(tc, test4);
  tcase_add_test(tc, test5);

  s = suite_create("PROJET CIMP");
  suite_add_tcase(s, tc);
  
  return s;
}

#endif
