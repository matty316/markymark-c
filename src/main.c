#include "markymark.h"

int main() {
  parse("*=-_.+`\n[]\r\n()<>!"
"# ## ### #### ##### ######"
"- -- ---"
"* ** ***"
"_ __ ___"
"` ``"
"```"
"func test() {"
"    print(\"hell nah\")"
"}"
"```"
"word 2 23"
"2. this is a list"
"23 this is not"
"####### not a heading");
  return 0;
}
