#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/times.h>
#include "stm32f4xx.h"


#undef errno
extern int errno;

uint8_t *__env[1] = { 0 };
uint8_t **environ = __env;

void initialise_monitor_handles()
{
}

int _getpid(void)
{
  return 1;
}

int _kill(int32_t pid, int32_t sig)
{
  errno = EINVAL;
  return -1;
}

void _exit (int32_t status)
{
  _kill(status, -1);
  while (1) {}
}

int _write(int32_t file, uint8_t *ptr, int32_t len)
{
    // TODO: Implement printf
    return len;
}

int _close(int32_t file)
{
  return -1;
}

int _fstat(int32_t file, struct stat *st)
{
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int32_t file)
{
  return 1;
}

int _lseek(int32_t file, int32_t ptr, int32_t dir)
{
  return 0;
}

int _read(int32_t file, uint8_t *ptr, int32_t len)
{
  return 0;
}

int _open(uint8_t *path, int32_t flags, int32_t mode)
{
  /* Pretend like we always fail */
  return -1;
}

int _wait(int32_t *status)
{
  errno = ECHILD;
  return -1;
}

int _unlink(const uint8_t *name)
{
  errno = ENOENT;
  return -1;
}

int _times(struct tms *buf)
{
  return -1;
}

int _stat(const uint8_t *file, struct stat *st)
{
  st->st_mode = S_IFCHR;
  return 0;
}

int _link(const uint8_t *old, const uint8_t *new)
{
  errno = EMLINK;
  return -1;
}

int _fork(void)
{
  errno = EAGAIN;
  return -1;
}

int _execve(const uint8_t *name, uint8_t * const *argv, uint8_t * const *env)
{
  errno = ENOMEM;
  return -1;
}

caddr_t _sbrk(int incr)
{
  extern char end asm("end");
  static char * heap_end;
  char * prev_heap_end;

  if (heap_end == NULL)
  {
    heap_end = &end;
  }

  prev_heap_end = heap_end;

  if (heap_end + incr > (char * ) __get_MSP())
  {
    errno = ENOMEM;
    return (caddr_t) -1;
  }
  heap_end += incr;
  return (caddr_t) prev_heap_end;
}
