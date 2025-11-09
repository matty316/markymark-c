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

void skip_whitespace() {
  while (true) {
    char c = peek();
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance();
      break;
    default:
      return;
    }
  }
}

Token scan_token() {
  skip_whitespace();
  scanner.start = scanner.position;

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
  case '=':
    return make_token(TOKEN_EQ);
  case '.':
    return make_token(TOKEN_DOT);
  case '+':
    return make_token(TOKEN_PLUS);
  case '(':
    return make_token(TOKEN_LPAREN);
  case ')':
    return make_token(TOKEN_RPAREN);
  case '<':
    return make_token(TOKEN_LT);
  case '>':
    return make_token(TOKEN_GT);
  case '!':
    return make_token(TOKEN_BANG);
  case '\n':
    scanner.line++;
    return make_token(TOKEN_LINE_ENDING);
  default:
    return read_text();
  }

  return error_token("unexpected character");
}
