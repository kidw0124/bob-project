#include<bits/stdc++.h>
#include<dlfcn.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include "remove.h"
int (*printf_real)(const char *, ...) = NULL;
FILE* (*fopen_real)(const char *, const char *) = NULL;
void (*exit_real)(int) = NULL;

void __attribute__((constructor)) init_hooking(){
    printf_real = (int (*)(const char *, ...))(dlsym(RTLD_NEXT, "printf"));
    fopen_real = (FILE* (*)(const char *, const char *))(dlsym(RTLD_NEXT, "fopen"));
    exit_real = (void (*)(int))(dlsym(RTLD_NEXT, "exit"));
    fprintf (stderr, "real exit is at %p\n", exit_real);
}

int printf(const char *format, ...){
    return printf_real("PID is %d\nPPID is %d\nThis is HOOKED!\n",getpid(),getppid());
}

FILE* fopen(const char *path, const char *mode){
    printf_real("PID is %d\nPPID is %d\nThis is HOOKED fopen!\n",getpid(),getppid());
    return fopen_real(path,mode);
}

void exit(int status){
    printf_real("PID is %d\nPPID is %d\nThis is HOOKED fopen!\n",getpid(),getppid());
    return exit_real(status);
}