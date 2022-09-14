#include "status.h"
#include "estop.h"
#include "usart.h"

#include <stdio.h>
#include <stdarg.h>


#define STAT_MSG(NAME, TEXT) static const char stat_##NAME[] PROGMEM = TEXT;
#include "messages.def"
#undef STAT_MSG


static const char *const stat_msg[] PROGMEM = {
#define STAT_MSG(NAME, TEXT) stat_##NAME,
#include "messages.def"
#undef STAT_MSG
};


const char *status_to_pgmstr(stat_t code) {
  return (const char *)pgm_read_ptr(&stat_msg[code]);
}


const char *status_level_pgmstr(status_level_t level) {
  switch (level) {
  case STAT_LEVEL_INFO: return PSTR("info");
  case STAT_LEVEL_DEBUG: return PSTR("debug");
  case STAT_LEVEL_WARNING: return PSTR("warning");
  default: return PSTR("error");
  }
}


stat_t status_message_P(const char *location, status_level_t level,
                        stat_t code, const char *msg, ...) {
  va_list args;

  // Type
  printf_P(PSTR("\n{\"level\":\"%" PRPSTR "\",\"msg\":\""),
           status_level_pgmstr(level));

  // Message
  printf_P(PSTR("%" PRPSTR), status_to_pgmstr(code));

  if (msg && pgm_read_byte(msg)) {
    putchar(':');
    putchar(' ');

    // TODO escape invalid chars
    va_start(args, msg);
    vfprintf_P(stdout, msg, args);
    va_end(args);
  }

  putchar('"');

  // Code
  if (code) printf_P(PSTR(",\"code\":%d"), code);

  // Location
  if (location) printf_P(PSTR(",\"where\":\"%" PRPSTR "\""), location);

  putchar('}');
  putchar('\n');

  return code;
}
