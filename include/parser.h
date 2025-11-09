#ifndef parser_h
#define parser_h

#include "token.h"

void advance_parser();
void parse_element();
void consume(TokenType type, const char* message);

#endif
