#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <omp.h>

bool *findPrimes(char *n, int num_threads){
  int N = atoi(n);
  bool *cross = (bool *)malloc((N + 1) * sizeof(bool));
  int bound = (N + 1) / 2;
  memset(cross, false, sizeof(bool) * (N + 1));
  
  #pragma omp parallel for schedule(static, 8) num_threads(num_threads)
  for (int i=2; i < bound; i++){
    if (cross[i])
      continue;
    #pragma omp simd
    for (int j=2 * i; j<= N; j += i)
      cross[j] = true;
  }
  return cross;
}

void writeFile(char *n, bool *cross){
  int idx=1;
  char *name = strcat(n, ".txt");
  FILE *file = fopen(name, "w");
  
  int N = atoi(n);
  for (int i = 2; i <= N; i++) {
    if (cross[i])
      continue;
    //fprintf(file, "%d %d\n", idx++, i);
    idx++;
  }
  fclose(file);
  free(cross);
  printf("Num: %d\n", idx);
}
  
  



int main(int argc, char **argv) {
  if (argc != 3){
    fprintf(stderr, "Usage: ./genprime N t\n");
    exit(0);
  }
  double tstart = 0.0, ttaken=0.0;
  tstart = omp_get_wtime(); 
 
  bool *out = findPrimes(argv[1], atoi(argv[2]));
  ttaken = omp_get_wtime() - tstart;
  printf("Time take for the main part: %f\n", ttaken);
  writeFile(argv[1], out);
  return 0;
}