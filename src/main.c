#include "markymark.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void runFile(const char* path) {
  FILE* file = fopen(path, "rb");
  
  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);
  
  char* buffer = (char*)malloc(fileSize + 1);
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  buffer[bytesRead] = '\0';
  
  fclose(file);
  parse(buffer);
}

int main(int argc, const char* argv[]) {
  if (argc == 2) {
    runFile(argv[1]);
  } else {
    fprintf(stderr, "Usage: markymark [path]\n");
    exit(64);
  }
  
  return 0;
}
