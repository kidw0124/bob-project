#include <stdio.h>

int main() {
    FILE* bashrc;
    bashrc = fopen("/etc/bashrc", "a"); 
    fprintf(bashrc, "export PROMPT_COMMAND='RETRN_VAL=$?;logger -p local6.debug \"$(whoami) [$$]: $(history 1 | sed \"s/^[ ]*[0-9]\\+[ ]*//\" ) [$RETRN_VAL]\"'");
    
    FILE* bash;
    bash = fopen("/etc/rsyslog.d/bash.conf", "a");
    fprintf(bash, "local6.*	/var/log/command_1.log");

    system("service rsyslog restart");

    FILE* logrotate;
    logrotate = fopen("/etc/logrotate.d/syslog", "a");
    fprintf(logrotate, "/var/log/command_1.log");

    system("cat /dev/null > /var/log/command_1.log");

    return 0;
}
