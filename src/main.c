#include "markymark.h"

int main() {
  parse("*=-_.+`\n[]\r\n()<>!\n"
        "# ## ### #### ##### ###### \n"
        "- -- --- \n"
        "* ** *** \n"
        "_ __ ___ \n"
        "` `` \n"
        "```\n"
        "func test() {\n"
        "    print(\"hell nah\")\n"
        "}\n"
        "``` \n"
        "word 2 23\n"
        "2. this is a list\n"
        "23 this is not\n"
        "####### not a heading\n");
  return 0;
}
