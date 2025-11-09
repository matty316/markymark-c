#ifndef markup_h
#define markup_h

typedef enum ElementType {
  LINE,
} ElementType;

typedef struct Element {
  ElementType type;
} Element;

typedef struct Markup {
  struct Element *elements;
  int numOfElements;
} Markup;

const char *html_from_markup(const Markup *markup);
const char *html_from_element(const Element *element);

#endif
