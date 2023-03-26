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

char *readFile(char *file){
  int fd = open(file, O_RDONLY);
  if (fd == -1)
    printf("Error openning the file");
  struct stat sb;
  if (fstat(fd, &sb) == -1)
    printf("Error getting file information");
  int totalFileSize = sb.st_size;

  //read file
  char *content = mmap(NULL, totalFileSize, PROT_READ, MAP_PRIVATE, fd, 0);
  if (content == MAP_FAILED)
    printf("mmap failed");
  return content;
}

void findMax(int *freq, int n){
  int mostOccur=0;
  int num=0;
  for (int i=0; i<4; i++){
    if (num < freq[i]){
      num = freq[i];
      mostOccur = i;
    }
  }
  printf("%c occurred the most %d times of a total of %d characters.\n", 
        (char) mostOccur + 'a', num, n);
}

void sequential(char **argv){
  char *content = readFile(argv[3]);
  int freq[4];
  memset(freq, 0, sizeof(freq));
  int n=atoi(argv[2]);
  for (int i=0; i<n; i++)
    freq[content[i] - 'a']++;
  findMax(freq, n);
  
}

void LocalHist(int *freq, char *content, int threadCount, int fileSize){
  int freqLocal[4] = {0};
  int myRank = omp_get_thread_num();
  int interval = fileSize / threadCount;
  int start = myRank * interval;
  int end = (myRank + 1) * interval;
  if (myRank == threadCount - 1)
    end = fileSize;

  for (int i=start; i<end; i++)
    freqLocal[content[i] - 'a']++;
  
  #pragma omp critical
  for (int i=0; i<4; i++)
    freq[i] += freqLocal[i];
}
void parallel(char **argv){
  int numThreads=atoi(argv[1]);
  int fileSize = atoi(argv[2]);

  char *content = readFile(argv[3]);
  

  int freq[4] = {0};
  #pragma omp parallel for num_threads(numThreads) \
    reduction(+:freq)
    for (int i = 0; i < fileSize; i++) {
      freq[content[i] - 'a']++;
    }
  /*
  #pragma omp parallel num_threads(numThreads)
    LocalHist(freq, content, numThreads, fileSize);
  */
  
  findMax(freq, fileSize);
}
int main(int argc, char **argv) {
  if (argc != 4){
    fprintf(stderr, "Usage: ./maxnum N num filename");
    exit(0);
  }
    
  if (!strcmp(argv[1], "0"))
    sequential(argv);
  else 
    parallel(argv);
    
  return 0;
}
