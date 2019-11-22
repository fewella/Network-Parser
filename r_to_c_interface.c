
#include <R.h>
#include <Rinternals.h>
#include <stdio.h>

SEXP dotProd(SEXP a, SEXP b) {
  printf("entering function");
  
  double* a_vec = REAL(a);
  double* b_vec = REAL(b);
  SEXP output = PROTECT(allocVector(REALSXP, LENGTH(a)));
  double* c_vec = REAL(output);
  int i;
  for (i = 0; i < LENGTH(a); ++i) {
    c_vec[i] = a_vec[i] * b_vec[i];
  }
  UNPROTECT(1);
  return output;
}