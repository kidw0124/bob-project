#include <bits/stdc++.h>
using namespace std;

class ftraceSysCallsFileLog {
 public:
  string directory = "syscalls";
  map<string, vector<string>> event_map;
  void setup() {
    event_map["file created"] = {"creat", "open", "openat"};
    event_map["file read"] = {"read", "readv", "pread64", "preadv", "preadv2"};
    event_map["file write"] = {"write", "writev", "pwrite64", "pwritev",
                               "pwritev2"};
    event_map["file opened"] = {"open", "openat"};
    event_map["file closed"] = {"close"};
    event_map["file accessed"] = {"access", "faccessat"};
    event_map["file attributes/permissions/ownership changed"] = {
        "chmod",  "fchmod", "fchmodat", "chown",     "fchown",   "fchownat",
        "lchown", "utime",  "utimes",   "utimensat", "futimesat"};
    event_map["file timestamps changed"] = {"utime", "utimes", "utimensat",
                                            "futimesat"};
    event_map["file link created"] = {"link", "linkat"};
    event_map["symlink created"] = {"symlink", "symlinkat"};
    event_map["file or link/symlink deleted"] = {"unlink", "unlinkat"};
    event_map["file renamed"] = {"rename", "renameat", "renameat2"};
    event_map["file moved"] = {"rename", "renameat", "renameat2"};
    event_map["file copied"] = {"copy_file_range"};
    event_map["file truncated"] = {"truncate", "ftruncate"};
    event_map["file locked"] = {"flock"};
    event_map["file mapped"] = {"mmap", "mmap2", "mremap"};
    event_map["file unmapped"] = {"munmap"};
    event_map["file executed"] = {"execve", "execveat"};
    event_map["file descriptor duplicated"] = {"dup", "dup2", "dup3"};
    event_map["file modified"] = {
        "fallocate", "fadvise64", "fadvise64_64",    "fdatasync",
        "fsync",     "sync",      "sync_file_range", "syncfs"};
    event_map["directory created"] = {"mkdir", "mkdirat"};
    event_map["directory deleted"] = {"rmdir"};
    event_map["directory opened"] = {"opendir", "fdopendir"};
    event_map["directory closed"] = {"closedir"};
    event_map["directory read"] = {"readdir", "readdir_r", "readdir64_r"};
  }
  ftraceSysCallsFileLog() { setup(); }
};

class ftraceSetting {
 public:
  void set_all_event_status(bool enable = false) {
    ofstream fout("/sys/kernel/debug/tracing/events/enable");
    fout << enable;
    fout.close();
  }
  void set_all_syscalls_event_status(bool enable = false) {
    ofstream fout("/sys/kernel/debug/tracing/events/syscalls/enable");
    fout << enable;
    fout.close();
  }
  void set_all_file_event_status(bool enable = false) {
    ofstream fout("/sys/kernel/debug/tracing/events/file/enable");
    fout << enable;
    fout.close();
  }
  void set_file_event_status(bool enable = false) {
    ftraceSysCallsFileLog file_log;
    for (auto event : file_log.event_map) {
      for (auto syscall : event.second) {
        ofstream fout("/sys/kernel/debug/tracing/events/" + file_log.directory +
                      "/sys_enter_" + syscall + "/enable");
        fout << enable;
        fout.close();
        fout.open("/sys/kernel/debug/tracing/events/" + file_log.directory +
                  "/sys_exit_" + syscall + "/enable");
        fout << enable;
        fout.close();
      }
    }
  }
};

int main(int argc, char *argv[]) {
  bool enable = true;
  if (argc == 2 && string(argv[1]) == "disable") enable = false;
  ftraceSetting setting;
  setting.set_all_event_status(false);
  setting.set_all_syscalls_event_status(false);
  setting.set_file_event_status(enable);
  return 0;
}