#include "line-processor.h"

#include <stdlib.h>
#include <string.h>

typedef struct LineProcessor {
  char* line;
  const char* start;
  const char* pos;
  size_t idx;
} LineProcessor;

void remove_at_index(char *line, size_t idx) {
  if (line == NULL || idx < 0)
    return;
  
  size_t len = strlen(line);
  for (size_t i = idx; i < len; i++)
    line[i] = line[i + 1];
}

void add_at_index(char *line, size_t idx, char c) {
  size_t len = strlen(line);
  
  line = realloc(line, len + 1);
  
  if (idx < 0 || idx > len || line == NULL)
    return;
  
  for (size_t i = len; i >= idx; i--)
    line[i + 1] = line[i];
  
  line[idx] = c;
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
  size_t idx = lp->idx;
  while (peek_char(lp) == c) {
    advance_char(lp);
    count++;
  }
  
  if (count > 3)
    return;
  
  for (size_t i = 0; i < count; i++)
    remove_at_index(lp->line, idx);
  
  while (peek_char(lp) != c) {
    advance_char(lp);
  }
  
  idx = lp->idx;
  
  for
}

void link(LineProcessor *lp) {
  
}

void code(LineProcessor *lp) {
  
}

void text(LineProcessor *lp) {
  
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
      default:
        text(&lineProcessor);
        break;
    }
  }
}
