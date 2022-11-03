#include "plog.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

private FILE *bcp_output = NULL;

#define output stdout

// Amazing name
private void helper(const char *label, const char *color, const char *di, const char *fmt) {
  if (!isatty(output)) { // TODO(kay): FIX ME
    fprintf(
      output,
      "%s[%s (%s)]:%s ",
      color,
      label,
      di,
      t_RESET
    );
  } else {
    fprintf(
      output,
      "[DEBUG LOG (`%s`)]: ",
      di
    );
  }
}

void p_f_debugLog(const char *di, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  helper("DEBUG LOG", t_BLUE, di, fmt);
  vfprintf(output, fmt, args);

  va_end(args);
}

void p_f_log(const char *di, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  
  helper("LOG", t_MAGENTA, di, fmt);
  vfprintf(output, fmt, args);

  va_end(args);
}

void p_f_warn(const char *di, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  
  helper("WARN", t_YELLOW, di, fmt);
  vfprintf(output, fmt, args);

  va_end(args);
}

void p_f_error(const char *di, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  
  helper("ERROR", t_BR_YELLOW, di, fmt);
  vfprintf(output, fmt, args);

  va_end(args);
}

#undef output
