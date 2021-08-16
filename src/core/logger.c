#include "logger.h"
#include <stdio.h>

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
int JIN_logger_log(enum JIN_Loggerm mode, const char *msg)
{
  if (!(flags & mode)) return -1;
  switch (mode) {
    case JIN_LOGGER_LOG:
      printf("LOG: ");
      break;
    case JIN_LOGGER_DBG:
      printf("DEBUG: ");
      break;
    case JIN_LOGGER_ERR:
      fprintf(stderr, "ERROR: %s\n", msg);
      return 0;
  }
  printf("%s\n", msg);

  return 0;
}

