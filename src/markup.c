#include "markup.h"
#include "line-processor.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

char *html;

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

void append_string(char **dest, const char *src) {
  if (*dest == NULL) {
    *dest = (char *)malloc(strlen(src) + 1);
    if (*dest == NULL) {
      perror("Failed to allocate memory");
      exit(EXIT_FAILURE);
    }
    strcpy(*dest, src);
  } else {
    *dest = (char *)realloc(*dest, strlen(*dest) + strlen(src) + 1);
    if (*dest == NULL) {
      perror("Failed to allocate memory");
      exit(EXIT_FAILURE);
    }
    strcat(*dest, src);
  }
}

void print_ln(const char *str, FILE *file, ...) {
  va_list args;
  va_start(args, str);
  int len = vsnprintf(NULL, 0, str, args);
  va_end(args);
  
  char buffer[len + 1];
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), str, args);

  va_end(args);
  
  if (file != nullptr)
    fprintf(file, "%s\n", buffer);
  printf("%s\n", buffer);
  append_string(&html, buffer);
}

void print(const char *str, FILE *file, ...) {
  va_list args;
  va_start(args, str);
  int len = vsnprintf(NULL, 0, str, args);
  va_end(args);
  
  char buffer[len + 1];
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), str, args);

  va_end(args);
  
  if (file != nullptr)
    fprintf(file, "%s", str);
  printf("%s", str);
  append_string(&html, buffer);
}

void html_from_line(const Element *element, FILE *file) {
  struct Line line = element->data.line;
  
  char buffer[line.length + 1];
  
  strncpy(buffer, line.start, line.length);
  
  buffer[line.length] = '\0';
  
  process_line(buffer);
  
  switch (line.type) {
    case LINE_H1:
      print_ln("<h1>%s</h1>", file, buffer);
      break;
    case LINE_H2:
      print_ln("<h2>%s</h2>", file, buffer);
      break;
    case LINE_H3:
      print_ln("<h3>%s</h3>", file, buffer);
      break;
    case LINE_H4:
      print_ln("<h4>%s</h4>", file, buffer);
      break;
    case LINE_H5:
      print_ln("<h5>%s</h5>", file, buffer);
      break;
    case LINE_H6:
      print_ln("<h6>%s</h6>", file, buffer);
      break;
    case LINE_P:
      print_ln("<p>%s</p>", file, buffer);
      break;
    case LINE_TEXT:
      print_ln("%s", file, buffer);
      break;
  }
}

void html_from_list_item(Element *element, FILE *file) {
  struct ListItem listItem = element->data.listItem;
  
  char buffer[listItem.length + 1];
  
  strncpy(buffer, listItem.start, listItem.length);
  
  buffer[listItem.length] = '\0';
  
  print_ln("\t<li>%s</li>", file, buffer);
}

void html_from_img(Element *element, FILE *file) {
  struct Img img = element->data.img;
  
  char altBuffer[img.altLength + 1];
  char srcBuffer[img.srcLength + 1];
  
  strncpy(altBuffer, img.altStart, img.altLength);
  strncpy(srcBuffer, img.srcStart, img.srcLength);
  
  altBuffer[img.altLength] = '\0';
  srcBuffer[img.srcLength] = '\0';
  
  print_ln("<img src=\"%s\" alt=\"%s\">", file, srcBuffer, altBuffer);
}

const char *html_from_markup(const struct Markup *markup, FILE *file) {
  for (size_t i = 0; i < markup->numOfElements; i++) {
    Element element = markup->elements[i];
    switch (element.type) {
      case LINE:
        html_from_line(&element, file);
        break;
      case BLANK: break;
      case ULIST_START:
        print_ln("<ul>", file);
        break;
      case ULIST_END:
        print_ln("</ul>", file);
        break;
      case OLIST_START:
        print_ln("<ol>", file);
        break;
      case OLIST_END:
        print_ln("</ol>", file);
        break;
      case LIST_ITEM:
        html_from_list_item(&element, file);
        break;
      case BLOCK_QUOTE_START:
        print_ln("<blockquote>", file);
        break;
      case BLOCK_QUOTE_END:
        print_ln("</blockquote>", file);
        break;
      case CODE_BLOCK_START:
        print_ln("<pre><code>", file);
        break;
      case CODE_BLOCK_END:
        print_ln("</code></pre>", file);
        break;
      case TAB:
        print_ln("\t", file);
        break;
      case HR:
        print_ln("<hr>", file);
        break;
      case IMG:
        html_from_img(&element, file);
        break;
    }
  }
  append_string(&html, "\0");
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

void add_img(Markup *markup, const char* altStart, size_t altLength, const char* srcStart, size_t srcLength) {
  Element element;
  element.type = IMG;
  
  element.data.img.altStart = altStart;
  element.data.img.altLength = altLength;
  element.data.img.srcStart = srcStart;
  element.data.img.srcLength = srcLength;
  
  add_element(markup, element);
}
