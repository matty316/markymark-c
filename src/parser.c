#include "parser.h"
#include "markup.h"
#include "scanner.h"
#include "token.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

struct Parser {
  Token current;
  Token previous;
  bool hadError;
} parser;

Markup markup;

bool match(TokenType *types, int count) {
  for (int i = 0; i < count; i++) {
    if (types[i] == parser.current.type) {
      advance_parser();
      return true;
    }
  }
  return false;
}

void init_parser() {
  init_markup(&markup);
  advance_parser();
  while (!match((TokenType[]){TOKEN_EOF}, 1)) {
    while (match((TokenType[]){TOKEN_WHITESPACE}, 1))
      advance_parser();
    parse_element();
  }
}

void free_parser() { free_markup(&markup); }

void error_at(Token *token, const char *message) {
  fprintf(stderr, "[line %d] Error", token->line);

  if (token->type == TOKEN_EOF)
    fprintf(stderr, " at end");
  else if (token->type == TOKEN_ERROR)
    ;
  else
    fprintf(stderr, " at '%.*s'", (int)token->length, token->start);

  fprintf(stderr, ": %s\n", message);
  parser.hadError = true;
}

void error(const char *message) { error_at(&parser.previous, message); }

void error_at_current(const char *message) {
  error_at(&parser.current, message);
}

void advance_parser() {
  parser.previous = parser.current;

  while (true) {
    parser.current = scan_token();

    if (parser.current.type != TOKEN_ERROR)
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

void expect(TokenType *types, int count) {
  if (match(types, count))
    return;

  error_at_current("expected token not found");
}

void expect_end() {
  expect((TokenType[]){TOKEN_LINE_ENDING, TOKEN_EOF}, 2);
}

void parse_blank() {
  add_blank(&markup);
}

void parse_paragraph() {
  const char *textStart = parser.current.start;
  size_t length = parser.current.length;
  advance_parser();
  expect_end();
  add_line(&markup, LINE_P, textStart, length);
}

void parse_heading() {
  TokenType type = parser.previous.type;
  const char *textStart = parser.current.start;
  size_t length = parser.current.length;
  advance_parser();
  expect_end();

  switch (type) {
  case TOKEN_HASH:
    add_line(&markup, LINE_H1, textStart, length);
    break;
  case TOKEN_HASH2:
    add_line(&markup, LINE_H2, textStart, length);
    break;
  case TOKEN_HASH3:
    add_line(&markup, LINE_H3, textStart, length);
    break;
  case TOKEN_HASH4:
    add_line(&markup, LINE_H4, textStart, length);
    break;
  case TOKEN_HASH5:
    add_line(&markup, LINE_H5, textStart, length);
    break;
  case TOKEN_HASH6:
    add_line(&markup, LINE_H6, textStart, length);
    break;
  default:
    parse_paragraph();
    break;
  }
}

void parse_line() {
  if (match((TokenType[]){TOKEN_LINE_ENDING}, 1))
    parse_blank();
  else if (match((TokenType[]){TOKEN_HASH, TOKEN_HASH2, TOKEN_HASH3,
                               TOKEN_HASH4, TOKEN_HASH5, TOKEN_HASH6},
                 6))
    parse_heading();
  else
    parse_paragraph();
}

void parse_unordered_list() {
  add_list(&markup, false);
  
  while (true) {
    const char* start = parser.current.start;
    size_t length = parser.current.length;
    advance_parser();
    add_list_item(&markup, start, length);
    expect_end();
    if (!match((TokenType[]){TOKEN_PLUS, TOKEN_STAR, TOKEN_MINUS}, 3))
      break;
  }
  
  end_list(&markup, false);
}

void parse_ordered_list() {
  add_list(&markup, true);
  
  while (true) {
    const char* start = parser.current.start;
    size_t length = parser.current.length;
    advance_parser();
    add_list_item(&markup, start, length);
    expect_end();
    if (!match((TokenType[]){TOKEN_NUM}, 1))
      break;
  }
  
  end_list(&markup, true);
}

void parse_block_quote() {
  add_block_quote(&markup);
  
  while (true) {
    parse_line();
    if (!match((TokenType[]){TOKEN_GT}, 1))
      break;
  }
  expect_end();
  
  end_block_quote(&markup);
}

void parse_code_block() {
  add_code_block(&markup);
  expect_end();
  
  while (true) {
    if (parser.current.type == TOKEN_TEXT) {
      const char* start = parser.current.start;
      size_t length = parser.current.length;
      advance_parser();
      add_line(&markup, LINE_TEXT, start, length);
      expect_end();
    } else if (parser.current.type == TOKEN_TAB){
      add_tab(&markup);
      advance_parser();
    }
    if (match((TokenType[]){TOKEN_TICK3}, 1))
      break;
  }
  expect_end();
  
  end_code_block(&markup);
}

void parse_element() {
  if (match((TokenType[]){TOKEN_STAR, TOKEN_PLUS, TOKEN_MINUS}, 3))
    parse_unordered_list();
  if (match((TokenType[]){TOKEN_NUM}, 1))
    parse_ordered_list();
  if (match((TokenType[]){TOKEN_GT}, 1))
    parse_block_quote();
  if (match((TokenType[]){TOKEN_TICK3}, 1))
    parse_code_block();
  else
    parse_line();
}

void print_markup() { html_from_markup(&markup); }
