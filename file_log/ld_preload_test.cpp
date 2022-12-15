#include <bits/stdc++.h>
#include <dlfcn.h>
#include <fcntl.h>  // for open
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "remove.h"
int (*printf_real)(const char *, ...) = NULL;
FILE *(*fopen_real)(const char *, const char *) = NULL;
void (*exit_real)(int) = NULL;
ssize_t (*write_real)(int, const void *, size_t) = NULL;

void __attribute__((constructor)) init_hooking() {
  printf_real = (int (*)(const char *, ...))(dlsym(RTLD_NEXT, "printf"));
  fopen_real =
      (FILE * (*)(const char *, const char *))(dlsym(RTLD_NEXT, "fopen"));
  exit_real = (void (*)(int))(dlsym(RTLD_NEXT, "exit"));
  write_real =
      (ssize_t(*)(int, const void *, size_t))(dlsym(RTLD_NEXT, "write"));
  fprintf(stderr, "real exit is at %p\n", exit_real);
  fprintf(stderr, "real write is at %p\n", write_real);
}

int printf(const char *format, ...) {
  return printf_real("PID is %d\nPPID is %d\nThis is HOOKED!\n", getpid(),
                     getppid());
}

FILE *fopen(const char *path, const char *mode) {
  printf_real("PID is %d\nPPID is %d\nThis is HOOKED fopen!\n", getpid(),
              getppid());
  return fopen_real(path, mode);
}

void exit(int status) {
  printf_real("PID is %d\nPPID is %d\nThis is HOOKED fopen!\n", getpid(),
              getppid());
  exit_real(status);
}

ssize_t write(int fd, const void *buf, size_t count) {
  printf_real("PID is %d PPID is %d This is HOOKED write!\n", getpid(),
              getppid());
  printf_real("fd is %d buf is %s count is %d \n", fd, (char *)buf, count);
  int nfd;
  if ((nfd = open("/home/dwoo/Desktop/a.txt", O_WRONLY | O_APPEND)) < 0) {
    fprintf(stderr, "%s open error\n", "/home/dwoo/Desktop/a.txt");
    exit(1);
  } else {
    printf_real("nfd is %d \n", nfd);
  }
  int w = write_real(nfd, "hooked", 6);
  printf_real("w is %d \n", w);
  if (w < 0) {
    fprintf(stderr, "%s write error\n", "/home/dwoo/Desktop/a.txt");
    exit(1);
  }

  return write_real(fd, buf, count);
}