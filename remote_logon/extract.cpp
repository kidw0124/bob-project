#include <bits/stdc++.h>
using namespace std;
class AuditLogRow {
 private:
  string time;
  vector<string> row;

 public:
  AuditLogRow(vector<string> row) { this->row = row; }
};
int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: sudo ./extract"
         << " <filename> [-l <last filename>]" << endl;
    return -1;
  }
  ifstream fin(argv[1]);
  ifstream fin2;
  if (argc > 2 && strcmp(argv[2], "-l") == 0) {
    fin2.open(argv[3]);
  } else {
    system("sudo last -F > last.txt");
    fin2.open("last.txt");
  }
}