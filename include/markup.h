#ifndef markup_h
#define markup_h

typedef enum ElementType {
  LINE,
} ElementType;

typedef enum LineType {
  H1, H2, H3, H4, H5, H6
} LineType;

typedef struct Element {
  ElementType type;
  union{
    struct Line {
      const char* content;
      LineType type;
    } line;
  } data;
} Element;

typedef struct Markup {
  struct Element *elements;
  int numOfElements;
} Markup;

const char *html_from_markup(const Markup *markup);
const char *html_from_element(const Element *element);

#endif
