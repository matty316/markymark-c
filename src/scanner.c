#include "scanner.h"
#include "token.h"

#include <stdbool.h>
#include <stddef.h>
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
  token.length = (size_t)(scanner.position - scanner.start);
  token.line = scanner.line;
  return token;
}

static Token error_token(const char *message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (size_t)strlen(message);
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

void skip_whitespace() {
  while (true) {
    char c = peek();
    switch (c) {
      case ' ':
      case '\r':
        advance();
        break;
      default:
        return;
    }
  }
}

Token read(TokenType type, TokenType type2, TokenType type3, char c) {
  int count = 1;
  while (peek() == c) {
    advance();
    count++;
  }
  
  if (count == 1) {
    return make_token(type);
  } else if (count == 2) {
    return make_token(type2);
  } else if (count == 3) {
    return make_token(type3);
  } else {
    return make_token(type);
  }
}

bool is_number(char c) { return '0' <= c && c <= '9'; }

Token read_text() {
  while (!is_at_line_ending() && !is_at_end())
    advance();
  
  return make_token(TOKEN_TEXT);
}

Token read_num() {
  while (is_number(peek()))
    advance();
  
  if (peek() == '.') {
    advance();
    advance();
    
    return make_token(TOKEN_NUM);
  } else {
    return read_text();
  }
}

Token read_heading() {
  int count = 1;
  while (peek() == '#') {
    advance();
    count++;
  }
  
  if (count <= 6 && peek() == ' ') {
    advance();
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
      advance();
      return make_token(TOKEN_PLUS);
    case '(':
      return make_token(TOKEN_LPAREN);
    case ')':
      return make_token(TOKEN_RPAREN);
    case '<':
      return make_token(TOKEN_LT);
    case '>':
      advance();
      return make_token(TOKEN_GT);
    case '!':
      return make_token(TOKEN_BANG);
    case '*':
      if (peek() == '*') {
        return read(TOKEN_STAR, TOKEN_STAR2, TOKEN_STAR3, '*');
      } else {
        advance();
        return make_token(TOKEN_STAR);
      }
    case '-':
      if (peek() == '-') {
        return read(TOKEN_MINUS, TOKEN_MINUS2, TOKEN_MINUS3, '-');
      } else {
        advance();
        return make_token(TOKEN_MINUS);
      }
    case '_':
      if (peek() == '_')
        return read(TOKEN_UNDERSCORE, TOKEN_UNDERSCORE2, TOKEN_UNDERSCORE3, '_');
      else
        return make_token(TOKEN_UNDERSCORE);
    case '`':
      if (peek() == '`')
        return read(TOKEN_TICK, TOKEN_TICK2, TOKEN_TICK3, '`');
      else
        return make_token(TOKEN_TICK);
    case '\t':
      return make_token(TOKEN_TAB);
    case '\n':
      scanner.line++;
      return make_token(TOKEN_LINE_ENDING);
    default:
      if (is_number(c))
        return read_num();
      else
        return read_text();
  }
  
  return error_token("unexpected character");
}
