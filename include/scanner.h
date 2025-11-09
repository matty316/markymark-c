#ifndef scanner_h
#define scanner_h

#include "token.h"

void init_scanner(const char *input);
Token scan_token();

#endif
