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
  if (argc != 2) {
    cout << "Usage: sudo ./extract"
         << " <filename> [-l <last filename>]" << endl;
    return -1;
  }
  ifstream fin(argv[1]);
}