#ifndef markup_h
#define markup_h

#include <stddef.h>
typedef enum ElementType {
  LINE,
  LIST,
} ElementType;

typedef enum LineType {
  LINE_H1,
  LINE_H2,
  LINE_H3,
  LINE_H4,
  LINE_H5,
  LINE_H6,
  LINE_P,
} LineType;

typedef struct Element {
  ElementType type;
  union {
    struct Line {
      const char *start;
      size_t length;
      LineType type;
    } line;
  } data;
} Element;

typedef struct Markup {
  struct Element *elements;
  size_t numOfElements;
  size_t capacity;
} Markup;

void init_markup(Markup *markup);
const char *html_from_markup(const Markup *markup);
const char *html_from_element(const Element *element);
void add_line(Markup *markup, LineType type, const char *content, int length);
void free_markup(Markup *markup);

#endif
