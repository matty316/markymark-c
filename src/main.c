#include "markymark.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void runFile(const char* inputPath, const char* outputPath) {
  FILE* file = fopen(inputPath, "rb");
  
  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);
  
  char* buffer = (char*)malloc(fileSize + 1);
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  buffer[bytesRead] = '\0';
  
  fclose(file);
  mm_parse(buffer, outputPath);
  mm_free();
  free(buffer);
}

int main(int argc, const char* argv[]) {
  if (argc == 3) {
    runFile(argv[1], argv[2]);
  } else if (argc == 2) {
    runFile(argv[1], nullptr);
  } else {
    fprintf(stderr, "Usage: markymark [input] [output]\n");
    exit(EXIT_FAILURE);
  }
  
  return 0;
}
