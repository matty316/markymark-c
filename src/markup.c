#include "markup.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_markup(Markup *markup) {
  size_t initialSize = 20;
  markup->elements = malloc(sizeof(Element) * initialSize);

  if (markup->elements == NULL)
    exit(1);

  markup->numOfElements = 0;
  markup->capacity = initialSize;
}

void free_markup(Markup *markup) { free(markup->elements); }

void grow_elements(Markup *markup) {
  size_t newCapacity = markup->capacity * 2;
  Element *newElements =
      realloc(markup->elements, sizeof(Element) * newCapacity);

  if (newElements == NULL)
    exit(1);

  markup->elements = newElements;
  markup->capacity = newCapacity;
}

void html_from_line(const Element *element) {
  struct Line line = element->data.line;

  char buffer[line.length + 1];

  strncpy(buffer, line.start, line.length);

  buffer[line.length] = '\0';

  switch (line.type) {
  case LINE_H1:
    printf("<h1>%s</h1>\n", buffer);
    break;
  case LINE_H2:
    printf("<h2>%s</h2>\n", buffer);
    break;
  case LINE_H3:
    printf("<h3>%s</h3>\n", buffer);
    break;
  case LINE_H4:
    printf("<h4>%s</h4>\n", buffer);
    break;
  case LINE_H5:
    printf("<h5>%s</h5>\n", buffer);
    break;
  case LINE_H6:
    printf("<h6>%s</h6>\n", buffer);
    break;
  case LINE_P:
    printf("<p>%s</p>\n", buffer);
    break;
  }
}

const char *html_from_markup(const struct Markup *markup) {
  char *html = malloc(sizeof(char));
  for (size_t i = 0; i < markup->numOfElements; i++) {
    Element element = markup->elements[i];
    switch (element.type) {
    case LINE:
      html_from_line(&element);
      break;
    default:
      break;
    }
  }
  return html;
}

void add_line(Markup *markup, LineType type, const char *content, size_t length) {
  if (markup->numOfElements >= markup->capacity)
    grow_elements(markup);

  Element newElement;
  newElement.type = LINE;
  newElement.data.line.type = type;
  newElement.data.line.start = content;
  newElement.data.line.length = length;

  markup->elements[markup->numOfElements] = newElement;
  markup->numOfElements++;
}
