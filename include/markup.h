#ifndef markup_h
#define markup_h

#include <stddef.h>
typedef enum ElementType {
  LINE,
  BLANK,
  ULIST_START,
  ULIST_END,
  OLIST_START,
  OLIST_END,
  LIST_ITEM,
  BLOCK_QUOTE_START,
  BLOCK_QUOTE_END,
  CODE_BLOCK_START,
  CODE_BLOCK_END,
  TAB,
} ElementType;

typedef enum LineType {
  LINE_H1,
  LINE_H2,
  LINE_H3,
  LINE_H4,
  LINE_H5,
  LINE_H6,
  LINE_P,
  LINE_TEXT,
} LineType;

typedef struct Element {
  ElementType type;
  union {
    struct Line {
      const char *start;
      size_t length;
      LineType type;
    } line;
    struct ListItem {
      const char *start;
      size_t length;
    } listItem;
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
void add_line(Markup *markup, LineType type, const char *start, size_t length);
void add_blank(Markup *markup);
void add_list(Markup *markup, bool ordered);
void add_list_item(Markup *markup, const char* start, size_t length);
void end_list(Markup *markup, bool ordered);
void add_block_quote(Markup *markup);
void end_block_quote(Markup *markup);
void add_code_block(Markup *markup);
void end_code_block(Markup *markup);
void add_tab(Markup *markup);
void free_markup(Markup *markup);

#endif
