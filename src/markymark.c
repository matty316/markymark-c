#include "markymark.h"
#include "scanner.h"
#include "token.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>

void parse(const char *input) {
  init_scanner(input);
  advance_parser();
  consume(TOKEN_EOF, "expected end of file");
}
