#include "markymark.h"
#include "parser.h"
#include "scanner.h"
#include <stdbool.h>

void parse(const char *input, const char *outputPath) {
  init_scanner(input);
  init_parser();
  if (outputPath == nullptr)
    print_markup();
  else
    create_html(outputPath);
  free_parser();
}
