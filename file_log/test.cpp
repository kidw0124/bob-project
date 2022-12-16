#include <bits/stdc++.h>
#include <dlfcn.h>
#include <fcntl.h>  // for open
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  // get LD_PRELOAD
  char *ld_preload = getenv("LD_PRELOAD");
  if (strcmp(ld_preload,
             "/home/dwoo/Desktop/bob-project/file_log/ldpreload.so") != 0) {
    system(
        "sudo echo 'export "
        "LD_PRELOAD=/home/dwoo/Desktop/bob-project/file_log/ldpreload.so' >> "
        "/etc/bash.bashrc");
    cout << "Please restart the program." << endl;
    return 0;
  }
  char filename[256];
  if (argc > 1) {
    strcpy(filename, argv[1]);
  } else {
    strcpy(filename, "test.txt");
  }
  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  write(fd, "HI\n", 4);
}