#include "markup.h"
#include <stdlib.h>

const char *html_from_markup(const struct Markup *markup) {
  char *html = malloc(sizeof(char));
  for (int i = 0; i < markup->numOfElements; i++) {
    Element element = markup->elements[i];
    switch (element.type) {
      case LINE:
        
        break;
        
      default:
        break;
    }
  }
  return html;
}
