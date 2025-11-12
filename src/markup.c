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
    case LINE_TEXT:
      printf("%s\n", buffer);
      break;
  }
}

void html_from_list_item(Element *element) {
  struct ListItem listItem = element->data.listItem;
  
  char buffer[listItem.length + 1];
  
  strncpy(buffer, listItem.start, listItem.length);
  
  buffer[listItem.length] = '\0';
  
  printf("\t<li>%s</li>\n", buffer);
}

const char *html_from_markup(const struct Markup *markup) {
  char *html = malloc(sizeof(char));
  for (size_t i = 0; i < markup->numOfElements; i++) {
    Element element = markup->elements[i];
    switch (element.type) {
      case LINE:
        html_from_line(&element);
        break;
      case BLANK: break;
      case ULIST_START:
        printf("<ul>\n");
        break;
      case ULIST_END:
        printf("</ul>\n");
        break;
      case OLIST_START:
        printf("<ol>\n");
        break;
      case OLIST_END:
        printf("</ol>\n");
        break;
      case LIST_ITEM:
        html_from_list_item(&element);
        break;
      case BLOCK_QUOTE_START:
        printf("<blockquote>\n");
        break;
      case BLOCK_QUOTE_END:
        printf("</blockquote>\n");
        break;
      case CODE_BLOCK_START:
        printf("<pre><code>\n");
        break;
      case CODE_BLOCK_END:
        printf("</pre></code>\n");
        break;
      case TAB:
        printf("\t");
        break;
      case HR:
        printf("<hr>\n");
        break;
    }
  }
  return html;
}

void add_element(Markup *markup, Element element) {
  if (markup->numOfElements >= markup->capacity)
    grow_elements(markup);
  markup->elements[markup->numOfElements] = element;
  markup->numOfElements++;
}

void add_element_type(Markup *markup, ElementType type) {
  Element newElement;
  newElement.type = type;
  
  add_element(markup, newElement);
}

void add_line(Markup *markup, LineType type, const char *start, size_t length) {
  Element newElement;
  newElement.type = LINE;
  newElement.data.line.type = type;
  newElement.data.line.start = start;
  newElement.data.line.length = length;
  
  add_element(markup, newElement);
}

void add_blank(Markup *markup) {
  add_element_type(markup, BLANK);
}

void add_list(Markup *markup, bool ordered) {
  add_element_type(markup, ordered ? OLIST_START : ULIST_START);
}

void add_list_item(Markup *markup, const char* start, size_t length) {
  Element newElement;
  newElement.type = LIST_ITEM;
  newElement.data.listItem.start = start;
  newElement.data.listItem.length = length;
  
  add_element(markup, newElement);
}

void end_list(Markup *markup, bool ordered) {
  add_element_type(markup, ordered ? OLIST_END : ULIST_END);
}

void add_block_quote(Markup *markup) {
  add_element_type(markup, BLOCK_QUOTE_START);
}

void end_block_quote(Markup *markup) {
  add_element_type(markup, BLOCK_QUOTE_END);
}

void add_code_block(Markup *markup) {
  add_element_type(markup, CODE_BLOCK_START);
}

void end_code_block(Markup *markup) {
  add_element_type(markup, CODE_BLOCK_END);
}

void add_tab(Markup *markup) {
  add_element_type(markup, TAB);
}

void add_hr(Markup *markup) {
  add_element_type(markup, HR);
}
