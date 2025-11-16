#include "line-processor.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct LineProcessor {
  char* line;
  char* start;
  char* pos;
  size_t idx;
} LineProcessor;

void replace(const char *start, const char *old, const char *new) {
  char *pos = strstr(start, old);
  if (pos != NULL) {
    size_t old_len = strlen(old);
    size_t new_len = strlen(new);
    
    size_t pos_len = strlen(pos);
    size_t remaining_len = pos_len - old_len;
    
    memmove(pos + new_len, pos + old_len, remaining_len + 1);
    memcpy(pos, new, new_len);
    start = pos + new_len;
  }
}

void remove_char(char *str, size_t index) {
  size_t len = strlen(str);
  
  if (index >= 0 && index < len) {
    for (size_t i = index; i < len - 1; i++) {
      str[i] = str[i + 1];
    }
    str[len - 1] = '\0';
  }
}

bool is_at_line_end(LineProcessor *lp) { return *lp->pos == '\0'; }

char peek_char(LineProcessor *lp) { return *lp->pos; }

char advance_char(LineProcessor *lp) {
  lp->pos++;
  lp->idx++;
  return lp->pos[-1];
}

void emphasis(LineProcessor *lp, char c) {
  size_t count = 1;
  while (peek_char(lp) == c && !is_at_line_end(lp)) {
    advance_char(lp);
    count++;
  }
    
  if (count <= 3) {
    while (peek_char(lp) != c && !is_at_line_end(lp))
      advance_char(lp);
    
    size_t endCount = 0;
    while (peek_char(lp) == c && !is_at_line_end(lp)) {
      advance_char(lp);
      endCount++;
    }
    
    //mismatched tokens
    if (count != endCount) return;
    
    char old[count + 1];
    
    for (size_t i = 0; i < count; i++)
      old[i] = c;
    
    old[count] = '\0';
    
    switch (count) {
      case 1:
        replace(lp->start, old, "<em>");
        replace(lp->start, old, "</em>");
        break;
      case 2:
        replace(lp->start, old, "<strong>");
        replace(lp->start, old, "</strong>");
        break;
      case 3:
        replace(lp->start, old, "<em><strong>");
        replace(lp->start, old, "</strong></em>");
        break;
      default:
        break;
    }
  }
}

void link(LineProcessor *lp) {
  const char *textStart = lp->pos;
  
  size_t textLen = 0;
  while (peek_char(lp) != ']' && !is_at_line_end(lp)) {
    advance_char(lp);
    textLen++;
  }
  advance_char(lp);
  
  if (peek_char(lp) == '(') {
    advance_char(lp);
    const char *hrefStart = lp->pos;
    
    size_t hrefLen = 0;
    while (peek_char(lp) != ')' && !is_at_line_end(lp)) {
      advance_char(lp);
      hrefLen++;
    }
    advance_char(lp);
    
    size_t oldLen = lp->pos - lp->start;
    char old[oldLen + 1];
    strncpy(old, lp->start, oldLen);
    old[oldLen] = '\0';
    
    char href[hrefLen + 1];
    strncpy(href, hrefStart, hrefLen);
    href[hrefLen] = '\0';
    
    char text[textLen + 1];
    strncpy(text, textStart, textLen);
    text[textLen] = '\0';
    
    char new[8 + hrefLen + 2 + textLen + 4];
    sprintf(new, "<a href=\"%s\">%s</a>", href, text);
    
    replace(lp->start, old, new);
  }
}

void code(LineProcessor *lp) {
  while (peek_char(lp) != '`' && !is_at_line_end(lp))
    advance_char(lp);
  
  replace(lp->start, "`", "<code>");
  replace(lp->start, "`", "</code>");
}

void process_line(char *line) {
  LineProcessor lineProcessor;
  lineProcessor.line = line;
  lineProcessor.pos = line;
  lineProcessor.idx = 0;
  while (*lineProcessor.pos != '\0') {
    lineProcessor.start = lineProcessor.pos;
    char c = advance_char(&lineProcessor);
    
    switch (c) {
      case '*':
      case '_':
        emphasis(&lineProcessor, c);
        break;
      case '[':
        link(&lineProcessor);
        break;
      case '`':
        code(&lineProcessor);
        break;
      case '\\':
        remove_char(lineProcessor.line, lineProcessor.idx - 1);
        break;
      default:
        break;
    }
  }
}
