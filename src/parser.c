#include "parser.h"
#include "markup.h"
#include "scanner.h"
#include "token.h"

#include <stdbool.h>
#include <stdio.h>

struct Parser {
  Token current;
  Token previous;
  bool hadError;
} parser;

Markup markup;

void error_at(Token *token, const char *message) {
  fprintf(stderr, "[line %d] Error", token->line);

  if (token->type == TOKEN_EOF)
    fprintf(stderr, " at end");
  else if (token->type == TOKEN_ERROR)
    ;
  else
    fprintf(stderr, " at '%.*s'", token->length, token->start);

  fprintf(stderr, ": %s\n", message);
  parser.hadError = true;
}

void error(const char *message) { error_at(&parser.previous, message); }

void error_at_current(const char *message) {
  error_at(&parser.current, message);
}

void advance_parser() {
  parser.current = parser.previous;

  while (true) {
    parser.current = scan_token();

    if (parser.current.type != TOKEN_ERROR ||
        parser.current.type == TOKEN_LINE_ENDING)
      break;

    error_at_current(parser.current.start);
  }
}

void consume(TokenType type, const char *message) {
  if (parser.current.type == type) {
    advance_parser();
    return;
  }

  error_at_current(message);
}

bool match(TokenType *types, int count) {
  for (int i = 0; i < count; i++) {
    if (types[i] == parser.current.type) {
      advance_parser();
      return true;
    }
  }
  return false;
}

void parse_blank() {}

void parse_line() {
  if (match((TokenType[]){TOKEN_LINE_ENDING}, 1))
    parse_blank();
}

void parse_element() { parse_line(); }
