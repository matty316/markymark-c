#include "line-processor.h"

#include "markup.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  LE_EM_OPEN,
  LE_EM_CLOSE,
  LE_STRONG_OPEN,
  LE_STRONG_CLOSE,
  LE_CODE_OPEN,
  LE_CODE_CLOSE,
  LE_TEXT,
  LE_LINK_START,
  LE_LINK_MID,
  LE_LINK_CLOSE,
  LE_ESCAPE,
} LineElementType;

typedef struct {
  char *start;
  size_t length;
  LineElementType type;
} LineElement;

typedef struct {
  char *start;
  char *pos;
  LineElement *elements;
  size_t element_count;
  size_t element_capacity;
} LineProcessor;

char peek_char(LineProcessor *lp) {
  return *lp->pos;
}

bool is_at_line_end(LineProcessor *lp) {
  return peek_char(lp) == '\0';
}

char advance_char(LineProcessor *lp) {
  lp->pos++;
  return lp->pos[-1];
}

bool should_stop(LineProcessor *lp) {
  return peek_char(lp) == '*' || peek_char(lp) == '_' || peek_char(lp) == '`' || peek_char(lp) == '[' || peek_char(lp) == '\\' || is_at_line_end(lp);
}

void grow_line_elements(LineProcessor *lp) {
  size_t newCapacity = lp->element_capacity * 2;
  LineElement *newElements =
      realloc(lp->elements, sizeof(LineElement) * newCapacity);

  if (newElements == NULL)
    exit(1);

  lp->elements = newElements;
  lp->element_capacity = newCapacity;
}

void add_line_element(LineElement element, LineProcessor *lp) {
  if (lp->element_count >= lp->element_capacity)
    grow_line_elements(lp);
  
  lp->elements[lp->element_count] = element;
  lp->element_count++;
}

void add_text_element(LineProcessor *lp) {
  while (!should_stop(lp) && !is_at_line_end(lp))
    advance_char(lp);
  LineElement t;
  t.start = lp->start;
  t.length = lp->pos - lp->start;
  t.type = LE_TEXT;
  
  add_line_element(t, lp);
}

char *append_element(char *dest, char *src, size_t len) {
  if (dest == NULL) {
    dest = malloc(len + 1);
    for (size_t i = 0; i < len; i++) {
      dest[i] = src[i];
    }
    dest[len] = '\0';
  } else {
    size_t dest_len = strlen(dest);
    dest = realloc(dest, dest_len + len + 1);
    for (size_t i = 0; i < len; i++) {
      dest[i + dest_len] = src[i];
    }
    dest[dest_len + len] = '\0';
  }
  return dest;
}

char *build_line(LineProcessor *lp) {
  char *dest = NULL;
  for (size_t i = 0; i < lp->element_count; i++) {
    LineElement e = lp->elements[i];

    switch (e.type) {
      case LE_TEXT:
        dest = append_element(dest, e.start, e.length);
        break;
      case LE_EM_OPEN:
        dest = append_element(dest, "<em>", 4);
        break;
      case LE_EM_CLOSE:
        dest = append_element(dest, "</em>", 5);
        break;
      case LE_STRONG_OPEN:
        dest = append_element(dest, "<strong>", 8);
        break;
      case LE_STRONG_CLOSE:
        dest = append_element(dest, "</strong>", 9);
        break;
      case LE_CODE_OPEN:
        dest = append_element(dest, "<code>", 6);
        break;
      case LE_CODE_CLOSE:
        dest = append_element(dest, "</code>", 7);
        break;
      case LE_LINK_START:
        dest = append_element(dest, "<a href=\"", 9);
        break;
      case LE_LINK_MID:
        dest = append_element(dest, "\">", 2);
        break;
      case LE_LINK_CLOSE:
        dest = append_element(dest, "</a>", 4);
        break;
      case LE_ESCAPE:
        break;
    }
  }
  return dest;
}

void add_emphasis(LineProcessor *lp, char c) {
  char *open_start = lp->start;
  size_t count = 1;
  while (peek_char(lp) == c && !is_at_line_end(lp)) {
    advance_char(lp);
    count++;
  }
    
  if (count <= 3) {
    char *text_start = lp->pos;
    size_t text_len = 0;
    while (peek_char(lp) != c && !is_at_line_end(lp)) {
      advance_char(lp);
      text_len++;
    }
    
    char *close_start = lp->pos;
    advance_char(lp);
    
    size_t closingCount = 1;
    while (peek_char(lp) == c && !is_at_line_end(lp)) {
      advance_char(lp);
      closingCount++;
    }
    LineElement open_element;
    LineElement close_element;
    
    LineElement text_element;
    text_element.start = text_start;
    text_element.length = text_len;
    text_element.type = LE_TEXT;

    if (count == closingCount) {
      switch (count) {
        case 1:
          open_element.start = open_start;
          open_element.length = 1;
          open_element.type = LE_EM_OPEN;
          
          close_element.start = close_start;
          close_element.length = 1;
          close_element.type = LE_EM_CLOSE;
          
          add_line_element(open_element, lp);
          add_line_element(text_element, lp);
          add_line_element(close_element, lp);
          break;
        case 2:
          open_element.start = open_start;
          open_element.length = 2;
          open_element.type = LE_STRONG_OPEN;
          
          close_element.start = close_start;
          close_element.length = 2;
          close_element.type = LE_STRONG_CLOSE;
          
          add_line_element(open_element, lp);
          add_line_element(text_element, lp);
          add_line_element(close_element, lp);
          break;
        case 3:
          open_element.start = open_start;
          open_element.length = 1;
          open_element.type = LE_EM_OPEN;
          
          LineElement open_strong;
          open_strong.start = open_start;
          open_strong.length = 2;
          open_strong.type = LE_STRONG_OPEN;
          
          LineElement close_strong;
          close_strong.start = close_start;
          close_strong.length = 2;
          close_strong.type = LE_STRONG_CLOSE;
          
          close_element.start = close_start;
          close_element.length = 1;
          close_element.type = LE_EM_CLOSE;
          
          add_line_element(open_element, lp);
          add_line_element(open_strong, lp);
          add_line_element(text_element, lp);
          add_line_element(close_strong, lp);
          add_line_element(close_element, lp);
          break;
        default:
          break;
      }
    }
  }
}

void add_code(LineProcessor *lp) {
  char *text_start = lp->pos;
  size_t text_len = 0;
  while (peek_char(lp) != '`' && !is_at_line_end(lp)) {
    advance_char(lp);
    text_len++;
  }
  
  LineElement code_start = {
    .type = LE_CODE_OPEN
  };
  add_line_element(code_start, lp);
  
  LineElement te = {
    .start = text_start,
    .length = text_len,
    .type = LE_TEXT
  };
  
  add_line_element(te, lp);
  
  LineElement code_close = {
    .type = LE_CODE_CLOSE
  };
  add_line_element(code_close, lp);
  advance_char(lp);
}

void add_link(LineProcessor *lp) {
  char *text_start = lp->pos;
  size_t text_len = 0;
  while (peek_char(lp) != ']' && !is_at_line_end(lp)) {
    advance_char(lp);
    text_len++;
  }
  
  advance_char(lp);
  advance_char(lp);
  
  char *href_start = lp->pos;
  size_t href_len = 0;
  while (peek_char(lp) != ')' && !is_at_line_end(lp)) {
    advance_char(lp);
    href_len++;
  }
  advance_char(lp);
  
  LineElement a_start = {
    .type = LE_LINK_START
  };
  LineElement a_mid = {
    .type = LE_LINK_MID
  };
  LineElement a_close = {
    .type = LE_LINK_CLOSE
  };
  
  LineElement te = {
    .start = text_start,
    .length = text_len,
    .type = LE_TEXT
  };
  
  LineElement he = {
    .start = href_start,
    .length = href_len,
    .type = LE_TEXT
  };
  
  add_line_element(a_start, lp);
  add_line_element(he, lp);
  add_line_element(a_mid, lp);
  add_line_element(te, lp);
  add_line_element(a_close, lp);
}

void add_escape(LineProcessor *lp) {
  LineElement e = {.type = LE_ESCAPE};
  add_line_element(e, lp);
  char *text_start = lp->pos;
  size_t text_len = 1;

  advance_char(lp);
  
  while (!should_stop(lp) && !is_at_line_end(lp)) {
    advance_char(lp);
    text_len++;
  }
  
  LineElement te = {
    .start = text_start,
    .length = text_len,
    .type = LE_TEXT
  };
  add_line_element(te, lp);
}

char *process_line(struct Line line) {
  LineProcessor lp;
  
  char *buffer = malloc(line.length + 1);
  strncpy(buffer, line.start, line.length);
  buffer[line.length] = '\0';
  
  lp.start = buffer;
  lp.pos = buffer;
  
  lp.element_count = 0;
  lp.element_capacity = 8;
  lp.elements = malloc(sizeof(LineElement) * lp.element_capacity);
  
  while (!is_at_line_end(&lp)) {
    lp.start = lp.pos;
    char c = advance_char(&lp);
    
    switch (c) {
      case '*':
      case '_':
        add_emphasis(&lp, c);
        break;
      case '`':
        add_code(&lp);
        break;
      case '[':
        add_link(&lp);
        break;
      case '\\':
        add_escape(&lp);
        break;
      default:
        add_text_element(&lp);
        break;
    }
  }
  
  char *line_string = build_line(&lp);
  
  free(buffer);
  
  return line_string;
}
