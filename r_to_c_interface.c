
#include <R.h>
#include <Rinternals.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "capture.h"

pthread_t session_thread;

SEXP startSession() {
  pthread_create(&session_thread, NULL, startup, (void*) 1);
  return R_NilValue;
}

SEXP endSession() {
  SEXP out = PROTECT(allocVector(INTSXP, 1));
  int* val = INTEGER(out);
  *val = pthread_cancel(session_thread);
  pthread_join(session_thread, NULL);
  UNPROTECT(1);
  return out;
}

void* wait_seconds(void* value) {
  long seconds = (long) value;
  printf("Hello\n");
  sleep(seconds);
  printf("World!\n");
  return NULL;
}

SEXP testThread() {
  pthread_create(&session_thread, NULL, wait_seconds, (void*) 3);
  pthread_detach(session_thread);
  return R_NilValue;
}

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

typedef struct {
  point points[NUM_KEYS];
} second_unit;

static second_unit* head = NULL;
static int start_idx = 0;
static int itr_idx = 0;

SEXP getSecondData() {
	SEXP columns[NUM_KEYS];
  int i;
  for (i = 0; i < NUM_KEYS; ++i) {
    columns[i] = PROTECT(allocVector(REALSXP, 60));
  }
  double* dbls[NUM_KEYS];
  for (i = 0; i < NUM_KEYS; ++i) {
    dbls[i] = REAL(columns[i]);
  }
	
	if (head == NULL) {
	  head = malloc(sizeof(second_unit) * 60);
	}
	
	second_unit* unit = &head[itr_idx];
	memcpy(unit->points, datapoints, sizeof(point) * NUM_KEYS);
	itr_idx++;
	if (start_idx == itr_idx) ++start_idx;
	
	i = start_idx;
	for (i = 0; i != itr_idx; i = (i + 1) % 60) {
	  point* points = (&head[i])->points;
	  int j;
    for (j = 0; j < NUM_KEYS; ++j) {
      dbls[j][i] = points[j].freq;
    }
	}
	
	SEXP ans = PROTECT(allocVector(VECSXP, 5)),
	     nms = PROTECT(allocVector(STRSXP, 5)),
	     rnms = PROTECT(allocVector(INTSXP, 2));
	
	SET_STRING_ELT(nms, 0, mkChar("myColumn1"));
	SET_STRING_ELT(nms, 1, mkChar("myColumn2"));
	SET_STRING_ELT(nms, 2, mkChar("myColumn3"));
	SET_STRING_ELT(nms, 3, mkChar("myColumn4"));
	SET_STRING_ELT(nms, 4, mkChar("myColumn5"));
	
	for (i = 0; i < 5; ++i) {
	  SET_VECTOR_ELT(ans, i, columns[i]);
	}
	
	INTEGER(rnms)[0] = NA_INTEGER;
	INTEGER(rnms)[1] = -60;
	
	setAttrib(ans, R_ClassSymbol, ScalarString(mkChar("data.frame")));
	setAttrib(ans, R_RowNamesSymbol, rnms);
	setAttrib(ans, R_NamesSymbol, nms);
	
	UNPROTECT(NUM_KEYS + 3);
	return(ans);
}
