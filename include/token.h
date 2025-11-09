#ifndef token_h
#define token_h

#include <stdint.h>
typedef enum TokenType { HASH, L_BRACKET, R_BRACKET, EOF, ERROR } TokenType;

typedef struct Token {
  const char *start;
  uint32_t length;
  int line;
  TokenType type;
} Token;

#endif
