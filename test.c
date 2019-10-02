#include "test.h"


int main(void){
  int erreur = 0;
  Suite *s = classe();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  erreur = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (erreur == 0 ? 0:1);
}
