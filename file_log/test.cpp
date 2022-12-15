#include <bits/stdc++.h>
#include <dlfcn.h>
#include <fcntl.h>  // for open
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  char filename[256];
  if (argc > 1) {
    strcpy(filename, argv[1]);
  } else {
    strcpy(filename, "test.txt");
  }
  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  write(fd, "HI", 3);
}