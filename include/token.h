#ifndef token_h
#define token_h

#include <stdint.h>
typedef enum TokenType {
  TOKEN_HASH,
  TOKEN_HASH2,
  TOKEN_HASH3,
  TOKEN_HASH4,
  TOKEN_HASH5,
  TOKEN_HASH6,
  TOKEN_TEXT,
  TOKEN_LBRACKET,
  TOKEN_RBRACKET,
  TOKEN_LINE_ENDING,
  TOKEN_WHITESPACE,
  TOKEN_EOF,
  TOKEN_ERROR
} TokenType;

typedef struct Token {
  const char *start;
  uint32_t length;
  int line;
  TokenType type;
} Token;

#endif
