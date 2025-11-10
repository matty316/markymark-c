#include "markymark.h"
#include "parser.h"
#include "scanner.h"
#include <stdbool.h>

void parse(const char *input) {
  init_scanner(input);
  init_parser();
  print_markup();
  free_parser();
}
