#include <bits/stdc++.h>
using namespace std;

class ftraceSetting {
 public:
  void set_all_event_status(bool enable = false) {
    ofstream fout("/sys/kernel/debug/tracing/events/enable");
    fout << enable;
  }
};