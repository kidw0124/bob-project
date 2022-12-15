#include <bits/stdc++.h>
using namespace std;

#define BUFFER_SIZE 1024

int main() {
  char buf[BUFFER_SIZE];
  int length;

  // 0번 파일(표준 입력) 에서 입력을 받아옴
  length = read(0, buf, BUFFER_SIZE);

  // 1번 파일(표준 출력)으로 출력함
  if (write(1, buf, length) != length) {
    fprintf(stderr, "write error\n");
    exit(1);
  }
  exit(0);
}