#include "markymark.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include <stdbool.h>

void parse(const char *input) {
  init_scanner(input);
  advance_parser();
  parse_element();
  consume(TOKEN_EOF, "expected end of file");
}
