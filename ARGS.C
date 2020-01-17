  #include <stdlib.h>
  #include <stdio.h>
  #include <float.h>
  #include <math.h>
  #include <limits.h>
  #include <string.h>
  #include <stdbool.h>

  #include "args.h"
 

option_t* opt_void(option_t* l, const char* kw, void (*f)()) {
  option_t* tmp = (option_t*) calloc(1,sizeof(option_t));
  tmp->keyword = (char*) malloc(strlen(kw)+1);
  strcpy(tmp->keyword,kw);
  tmp->spec = OptVoid;
  tmp->fct.opt_void = f;
  tmp->next = l;
  l = tmp;
  return l;
}

option_t* opt_int(option_t* l, const char* kw, void (*f)(int)) {
  option_t* tmp = (option_t*) calloc(1,sizeof(option_t));
  tmp->keyword = (char*) malloc(strlen(kw)+1);
  strcpy(tmp->keyword,kw);
  tmp->spec = OptInt;
  tmp->fct.opt_int = f;
  tmp->next = l;
  l = tmp;
  return l;
}

option_t* opt_float(option_t* l, const char* kw, void (*f)(float)) {
  option_t* tmp = (option_t*) calloc(1,sizeof(option_t));
  tmp->keyword = (char*) malloc(strlen(kw)+1);
  strcpy(tmp->keyword,kw);
  tmp->spec = OptFloat;
  tmp->fct.opt_float = f;
  tmp->next = l;
  l = tmp;
  return l;
}

option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*)) {
  option_t* tmp = (option_t*) calloc(1,sizeof(option_t));
  tmp->keyword = (char*) malloc(strlen(kw)+1);
  strcpy(tmp->keyword,kw);
  tmp->spec = OptString;
  tmp->fct.opt_str = f;
  tmp->next = l;
  l = tmp;
  return l;
}

void opt_delete(option_t* l) {
  option_t* tmp = l;
  while( tmp!= NULL) {
    free(tmp->keyword);
    option_t* d = tmp;
    tmp = tmp->next;
    free(d);
  }
}

void process_options(option_t* l, int argc, char* *argv) {
  int i;
  for(i=1;i<argc-1;i++) {
    option_t* tmp = l;
    while(tmp != NULL) {
      if(strcmp(tmp->keyword,argv[i])==0) {
        if(tmp->spec == OptInt) {
          (tmp->fct.opt_int)(atoi(argv[i+1]));
        }
        else if(tmp->spec == OptString) {
          (tmp->fct.opt_str)(argv[i+1]);
          if(strcmp(argv[i+1],"-i")==0 || strcmp(argv[i+1],"-o")==0 || strcmp(argv[i+1],"-c")==0 || strcmp(argv[i+1],"-l")==0 || strcmp(argv[i+1],"-x")==0) {
            i++;
          }
        }
        else if(tmp->spec == OptFloat) {
          (tmp->fct.opt_float)(atof(argv[i+1]));
        }
        else {
          tmp->fct.opt_void();
        }
      }
      tmp = tmp->next;
    }
  }
}


void f1(const char* str) {
  printf("F1: %s\n", str);
}
 
void f2(int i) {
  printf("F2: %d\n", i);
}
 
void f3() {
  printf("F3: no param\n");
}
 
void f4(float f) {
  printf("F4: %f\n", f);
}
 
 
int main(int argc, char** argv) {
  option_t* opt = NOOPTION;
  opt = opt_string(opt, "-a", f1);
  opt = opt_int(opt, "-b", f2);
  opt = opt_void(opt, "-c", f3);
  opt = opt_float(opt, "-d", f4);
 
  process_options(opt, argc, argv);
 
  opt_delete(opt);
 
  return EXIT_SUCCESS;
}
