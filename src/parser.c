#include "parser.h"
#include "scanner.h"

#include <stdbool.h>
#include <stdio.h>

struct Parser {
  Token current;
  Token previous;
  bool hadError;
} parser;

void error_at(Token *token, const char* message) {
  fprintf(stderr, "[line %d] Error", token->line);
  
  if (token->type == TOKEN_EOF)
    fprintf(stderr, " at end");
  else if (token->type == TOKEN_ERROR);
  else
    fprintf(stderr, " at '%.*s'", token->length, token->start);
  
  fprintf(stderr, ": %s\n", message);
  parser.hadError = true;
}

void error(const char* message) {
  error_at(&parser.previous, message);
}

void error_at_current(const char* message) {
  error_at(&parser.current, message);
}

void advance_parser() {
  parser.current = parser.previous;
  
  while (true) {
    parser.current = scan_token();
    
    if (parser.current.type != TOKEN_ERROR) break;
    
    error_at_current(parser.current.start);
  }
}

void consume(TokenType type, const char* message) {
  if (parser.current.type == type) {
    advance_parser();
    return;
  }
  
  error_at_current(message);
}
