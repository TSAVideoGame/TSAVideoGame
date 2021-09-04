#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

/* static FILE        *file; */
static unsigned int flags;

/*
 * JIN_logger_init
 *
 * @desc
 * @param f
 * @return
 */
int JIN_logger_init(unsigned int f)
{
  /* if (!(file = fopen("log.txt", "w"))) return -1; */
  flags = f;

  return 0;
}

/*
 * JIN_logger_quit
 *
 * @desc
 * @return
 */
int JIN_logger_quit(void)
{
  /* fclose(file); */

  return 0;
}

/*
 * JIN_logger_log
 *
 * @desc
 * @param mode
 * @param msg
 * @return
 */
int JIN_logger_log(enum JIN_Loggerm mode, const char *msg, ...)
{
  va_list args;
  va_start(args, msg);

  if (!(flags & mode)) return -1;
  switch (mode) {
    case JIN_LOGGER_LOG:
      fprintf(stdout, "LOG: ");
      vfprintf(stdout, msg, args);
      fprintf(stdout, "\n");
      break;
    case JIN_LOGGER_DBG:
      fprintf(stdout, "DEBUG: ");
      vfprintf(stdout, msg, args);
      fprintf(stdout, "\n");
      break;
    case JIN_LOGGER_ERR:
      fprintf(stderr, "ERROR: ");
      vfprintf(stderr, msg, args);
      fprintf(stderr, "\n");
      break;
  }

  va_end(args);

  return 0;
}

