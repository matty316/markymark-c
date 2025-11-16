#ifndef parser_h
#define parser_h

#include "token.h"

void init_parser();
void free_parser();
void advance_parser();
void parse_element();
void consume(TokenType type, const char *message);
void print_markup();
const char *create_html(const char *outputPath);
const char *html_string();
#endif
