#include "mma/mma.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]) {
  int i;
  for (i=1; i<argc; i++) {
    MMACommRegister(argv[i]);
  }
  MMACommInitialize();
  MMACommPrint();
  MMACommFinalize();
}

