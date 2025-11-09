#include "scanner.h"
#include "token.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

struct Scanner {
  const char *input;
  const char *start;
  const char *position;
  uint32_t line;
} scanner;

void init_scanner(const char *input) {
  scanner.input = input;
  scanner.start = input;
  scanner.position = input;
  scanner.line = 1;
}

static Token make_token(TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (uint32_t)(scanner.position - scanner.start);
  token.line = scanner.line;
  return token;
}

static Token error_token(const char *message) {
  Token token;
  token.type = ERROR;
  token.start = message;
  token.length = (uint32_t)strlen(message);
  token.line = scanner.line;
  return token;
}

bool is_at_end() { return *scanner.position == '\0'; }
static char advance() {
  scanner.position++;
  return scanner.position[-1];
}

Token scan_token() {
  scanner.position = scanner.start;

  if (is_at_end()) {
    return make_token(EOF);
  }

  char c = advance();

  switch (c) {
  case '[':
    return make_token(L_BRACKET);
  case ']':
    return make_token(R_BRACKET);
  }

  return error_token("unexpected character");
}
