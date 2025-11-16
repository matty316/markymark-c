#include "markymark.h"
#include "parser.h"
#include "scanner.h"
#include <stdbool.h>

const char *parse(const char *input, const char *outputPath) {
  init_scanner(input);
  init_parser();
  
  const char* html;
  if (outputPath == nullptr)
    html = html_string();
  else
    html = create_html(outputPath);
  free_parser();
  return html;
}
