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
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (uint32_t)strlen(message);
  token.line = scanner.line;
  return token;
}

bool is_at_end() { return *scanner.position == '\0'; }

char peek() { return *scanner.position; }

bool is_at_line_ending() { return peek() == '\n' || peek() == '\r'; }

char advance() {
  scanner.position++;
  return scanner.position[-1];
}

Token read_text() {
  while (!is_at_line_ending() && !is_at_end())
    advance();

  return make_token(TOKEN_TEXT);
}

Token read_heading() {
  int count = 1;
  while (peek() == '#') {
    advance();
    count++;
  }

  if (count <= 6 && peek() == ' ') {
    switch (count) {
    case 1:
      return make_token(TOKEN_HASH);
    case 2:
      return make_token(TOKEN_HASH2);
    case 3:
      return make_token(TOKEN_HASH3);
    case 4:
      return make_token(TOKEN_HASH4);
    case 5:
      return make_token(TOKEN_HASH5);
    case 6:
      return make_token(TOKEN_HASH6);
    default:
      return error_token("illegal char");
    }
  } else {
    return read_text();
  }
}

Token scan_token() {
  scanner.position = scanner.start;

  if (is_at_end())
    return make_token(TOKEN_EOF);

  char c = advance();

  switch (c) {
  case '#':
    return read_heading();
  case '[':
    return make_token(TOKEN_LBRACKET);
  case ']':
    return make_token(TOKEN_RBRACKET);
  case ' ':
    return make_token(TOKEN_WHITESPACE);
  case '\r':
  case '\n':
    scanner.line++;
    return make_token(TOKEN_LINE_ENDING);
  default:
    return read_text();
  }

  return error_token("unexpected character");
}
