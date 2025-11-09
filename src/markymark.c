#include "markymark.h"
#include "scanner.h"
#include "token.h"
#include <stdbool.h>
#include <stdio.h>

void parse(const char *input) {
  init_scanner(input);
  while (true) {
    Token token = scan_token();

    printf("%2d '%.*s'\n", token.type, token.length, token.start);

    if (token.type == TOKEN_EOF)
      break;
  }
}
