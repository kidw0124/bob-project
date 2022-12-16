#include <bits/stdc++.h>
#include <dlfcn.h>
#include <fcntl.h>  // for open
#include <fts.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

string fd2path(int fd) {
  string path;
  FTS *file_system = nullptr;
  FTSENT *child = nullptr;
  FTSENT *parent = nullptr;
  vector<char *> root;
  char buffer[2];
  strcpy(buffer, "/");
  root.push_back(buffer);
  file_system = fts_open(&root[0], FTS_COMFOLLOW | FTS_NOCHDIR, nullptr);
  if (file_system) {
    while ((parent = fts_read(file_system))) {
      child = fts_children(file_system, 0);
      while (child && child->fts_link) {
        child = child->fts_link;
        struct stat info;
        if (!S_ISSOCK(child->fts_statp->st_mode)) {
          if (!fstat(fd, &info) && !S_ISSOCK(info.st_mode)) {
            if (child->fts_statp->st_dev == info.st_dev) {
              if (child->fts_statp->st_ino == info.st_ino) {
                path = child->fts_path + string(child->fts_name);
                goto finish;
              }
            }
          }
        }
      }
    }
  finish:
    fts_close(file_system);
  }
  return path;
}

int (*printf_real)(const char *, ...) = NULL;
FILE *(*fopen_real)(const char *, const char *) = NULL;
void (*exit_real)(int) = NULL;
ssize_t (*write_real)(int, const void *, size_t) = NULL;
int (*open_real)(const char *, int, ...) = NULL;

void __attribute__((constructor)) init_hooking() {
  printf_real = (int (*)(const char *, ...))(dlsym(RTLD_NEXT, "printf"));
  fopen_real =
      (FILE * (*)(const char *, const char *))(dlsym(RTLD_NEXT, "fopen"));
  exit_real = (void (*)(int))(dlsym(RTLD_NEXT, "exit"));
  write_real =
      (ssize_t(*)(int, const void *, size_t))(dlsym(RTLD_NEXT, "write"));
  open_real = (int (*)(const char *, int, ...))(dlsym(RTLD_NEXT, "open"));
}

ssize_t write(int fd, const void *buf, size_t count) {
  // printf_real("PID is %d PPID is %d This is HOOKED write!\n", getpid(),
  //             getppid());
  // printf_real("fd is %d buf is %s count is %d \n", fd, (char *)buf, count);
  int nfd;
  if ((nfd = open_real("./filelog.txt", O_WRONLY | O_APPEND)) < 0) {
    fprintf(stderr, "%s open error\n", "./filelog.txt");
    exit(1);
  } else {
    // printf_real("nfd is %d \n", nfd);
  }
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  string time =
      std::to_string(tm.tm_year + 1900) + "-" + std::to_string(tm.tm_mon + 1) +
      "-" + std::to_string(tm.tm_mday) + " " + std::to_string(tm.tm_hour) +
      ":" + std::to_string(tm.tm_min) + ":" + std::to_string(tm.tm_sec) + " ";

  string filePath = fd2path(fd) + ' ';
  struct stat info;
  fstat(fd, &info);
  string fileStatusFlag = std::to_string(info.st_mode) + ' ';
  string fileOwner = string(getpwuid(info.st_uid)->pw_name) +
                     "(uid:" + std::to_string(info.st_uid) + ") ";
  string fileGroup = string(getgrgid(info.st_gid)->gr_name) +
                     "(gid:" + std::to_string(info.st_gid) + ") ";
  stringstream filePermission;
  filePermission << (info.st_mode & S_IRUSR ? "r" : "-")
                 << (info.st_mode & S_IWUSR ? "w" : "-")
                 << (info.st_mode & S_IXUSR ? "x" : "-")
                 << (info.st_mode & S_IRGRP ? "r" : "-")
                 << (info.st_mode & S_IWGRP ? "w" : "-")
                 << (info.st_mode & S_IXGRP ? "x" : "-")
                 << (info.st_mode & S_IROTH ? "r" : "-")
                 << (info.st_mode & S_IWOTH ? "w" : "-")
                 << (info.st_mode & S_IXOTH ? "x" : "-") << ' ';
  string fileAtime = std::to_string(info.st_atime) + ' ';
  string fileMtime = std::to_string(info.st_mtime) + ' ';
  string fileCtime = std::to_string(info.st_ctime) + ' ';
  stringstream log;
  log << time << filePath << " write: " << (char *)buf << ' ' << fileStatusFlag
      << fileOwner << fileGroup << filePermission.str() << fileAtime
      << fileMtime << fileCtime << endl;
  string logstring = log.str();

  int w = write_real(nfd, logstring.c_str(), logstring.length());
  // printf_real("w is %d \n", w);
  if (w < 0) {
    fprintf(stderr, "%s write error\n", "./filelog.txt");
    exit(1);
  }

  return write_real(fd, buf, count);
}

FILE *fopen(const char *path, const char *mode) {
  // printf_real("PID is %d PPID is %d This is HOOKED fopen!\n", getpid(),
  //             getppid());
  // printf_real("path is %s mode is %s \n", path, mode);
  int nfd;
  if ((nfd = open_real("./filelog.txt", O_WRONLY | O_APPEND)) < 0) {
    fprintf(stderr, "%s open error\n", "./filelog.txt");
    exit(1);
  } else {
    // printf_real("nfd is %d \n", nfd);
  }
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  string time =
      std::to_string(tm.tm_year + 1900) + "-" + std::to_string(tm.tm_mon + 1) +
      "-" + std::to_string(tm.tm_mday) + " " + std::to_string(tm.tm_hour) +
      ":" + std::to_string(tm.tm_min) + ":" + std::to_string(tm.tm_sec) + " ";

  string filePath = path + ' ';
  struct stat info;
  stat(path, &info);
  string fileStatusFlag = std::to_string(info.st_mode) + ' ';
  string fileOwner = string(getpwuid(info.st_uid)->pw_name) +
                     "(uid:" + std::to_string(info.st_uid) + ") ";
  string fileGroup = string(getgrgid(info.st_gid)->gr_name) +
                     "(gid:" + std::to_string(info.st_gid) + ") ";
  stringstream filePermission;
  filePermission << (info.st_mode & S_IRUSR ? "r" : "-")
                 << (info.st_mode & S_IWUSR ? "w" : "-")
                 << (info.st_mode & S_IXUSR ? "x" : "-")
                 << (info.st_mode & S_IRGRP ? "r" : "-")
                 << (info.st_mode & S_IWGRP ? "w" : "-")
                 << (info.st_mode & S_IXGRP ? "x" : "-")
                 << (info.st_mode & S_IROTH ? "r" : "-")
                 << (info.st_mode & S_IWOTH ? "w" : "-")
                 << (info.st_mode & S_IXOTH ? "x" : "-") << ' ';
  string fileAtime = std::to_string(info.st_atime) + ' ';
  string fileMtime = std::to_string(info.st_mtime) + ' ';
  string fileCtime = std::to_string(info.st_ctime) + ' ';
  stringstream log;
  log << time << filePath << " open: " << mode << ' ' << fileStatusFlag
      << fileOwner << fileGroup << filePermission.str() << fileAtime
      << fileMtime << fileCtime << endl;
  string logstring = log.str();

  int w = write_real(nfd, logstring.c_str(), logstring.length());
  // printf_real("w is %d \n", w);
  if (w < 0) {
    fprintf(stderr, "%s write error\n", "./filelog.txt");
    exit(1);
  }
  return fopen_real(path, mode);
}

int open(const char *pathname, int flags, mode_t mode = 0) {
  // printf_real("PID is %d PPID is %d This is HOOKED open!\n", getpid(),
  //             getppid());
  // printf_real("pathname is %s flags is %d mode is %d \n", pathname, flags,
  // mode);
  int nfd;
  if ((nfd = open_real("./filelog.txt", O_WRONLY | O_APPEND)) < 0) {
    fprintf(stderr, "%s open error\n", "./filelog.txt");
    exit(1);
  } else {
    // printf_real("nfd is %d \n", nfd);
  }
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  string time =
      std::to_string(tm.tm_year + 1900) + "-" + std::to_string(tm.tm_mon + 1) +
      "-" + std::to_string(tm.tm_mday) + " " + std::to_string(tm.tm_hour) +
      ":" + std::to_string(tm.tm_min) + ":" + std::to_string(tm.tm_sec) + " ";

  string filePath = pathname + ' ';
  struct stat info;
  stat(pathname, &info);
  string fileStatusFlag = std::to_string(info.st_mode) + ' ';
  string fileOwner = string(getpwuid(info.st_uid)->pw_name) +
                     "(uid:" + std::to_string(info.st_uid) + ") ";
  string fileGroup = string(getgrgid(info.st_gid)->gr_name) +
                     "(gid:" + std::to_string(info.st_gid) + ") ";
  stringstream filePermission;
  filePermission << (info.st_mode & S_IRUSR ? "r" : "-")
                 << (info.st_mode & S_IWUSR ? "w" : "-")
                 << (info.st_mode & S_IXUSR ? "x" : "-")
                 << (info.st_mode & S_IRGRP ? "r" : "-")
                 << (info.st_mode & S_IWGRP ? "w" : "-")
                 << (info.st_mode & S_IXGRP ? "x" : "-")
                 << (info.st_mode & S_IROTH ? "r" : "-")
                 << (info.st_mode & S_IWOTH ? "w" : "-")
                 << (info.st_mode & S_IXOTH ? "x" : "-") << ' ';
  string fileAtime = std::to_string(info.st_atime) + ' ';
  string fileMtime = std::to_string(info.st_mtime) + ' ';
  string fileCtime = std::to_string(info.st_ctime) + ' ';
  stringstream log;
  log << time << filePath << " open: " << flags << ' ' << fileStatusFlag
      << fileOwner << fileGroup << filePermission.str() << fileAtime
      << fileMtime << fileCtime << endl;
  string logstring = log.str();

  int w = write_real(nfd, logstring.c_str(), logstring.length());
  // printf_real("w is %d \n", w);
  if (w < 0) {
    fprintf(stderr, "%s write error\n", "./filelog.txt");
    exit(1);
  }
  return open_real(pathname, flags, mode);
}