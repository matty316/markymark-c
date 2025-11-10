#include "markymark.h"

int main() {
  parse("# this is a heading\n"
        "## this is a heading 2\n"
        "### this is a heading 3\n"
        "#### this is a heading 4\n"
        "##### this is a heading 5\n"
        "###### this is a heading 6\n"
        "####### this is not a heading\n");
  return 0;
}
