#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <poll.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/inotify.h>

#define rsyslog_path "/etc/"
#define log_path "/var/log/"


char* date() {
    char full_date[1000] = { 0,};
    static char cut_date[19];
    FILE * code_result;
    
    if ((code_result=popen("date", "r")) == NULL) {
        printf("error");// error processing and return
    }
    
    fgets(full_date, 1000, code_result);
    
    for(int i=0;i<19;i++) {
        cut_date[i] = full_date[i];
    }

    return cut_date;
}

char* users(){
    char bfr[BUFSIZ] = {0,};
    char bfr2[BUFSIZ] = {0,};
    char bfr4[BUFSIZ] = {0,};
    static char bfr3[BUFSIZ] = {0,};
    static char per[BUFSIZ] = {0, };
    int blank = 0;
    FILE * fp;
    FILE * user;
// ...
    if ((fp=popen("users | wc -w", "r")) ==NULL) {
        printf("error");// error processing and return
    }
// ...
    fgets(bfr, BUFSIZ, fp) != NULL;

    static char now_user[1000] = {0,};
        
    user=popen("who am i", "r");
    fgets(bfr2, BUFSIZ, user) != NULL;
        
    for(int i=0;i<1000;i++) {
    if (bfr2[i] != ' ') {
        now_user[i] = bfr2[i];
    }
    else if (bfr2[i] == ' ') {
        break;
        }
    }

    if (bfr == "1") {
        return now_user;
    }
    else if (bfr >= "2") {
        
        if ((fp=popen("tail -1 /var/log/commands.log | awk '{if ($8 == \"vim\" || $8 == \"rm\" || $8 == \"chmod\") print $5}'", "r")) != NULL) {
            fgets(bfr3, BUFSIZ, fp);
        }

	for (int i=0;i<BUFSIZ;i++) {
	    if (bfr3[i] == ':') {
	        bfr3[i] = '\0';
	    }
	}

	if (now_user == bfr3) {
	    return now_user;
	}
	else if (now_user != bfr3){
	    return bfr3;
	}
    }
}

char* att() {
    FILE* att;
    static char bfr_att[BUFSIZ] = "root";

    if ((att=popen("tail -1 /var/log/commands.log | awk '{if ($8 == \"vim\" || $8 == \"rm\" || $8 == \"chmod\") print $6}'", "r")) != NULL) {
            fgets(bfr_att, BUFSIZ, att);
        }

    for(int i=0;i<BUFSIZ;i++) {
        if (bfr_att[i] == '\n') {
            bfr_att[i] = '\0';
        }
    }

    return bfr_att;
}

static void __handle_inotify_event(const struct inotify_event* event)
{
    char *date_save;
    char *user;
    char *now_att;
    const char *name = event->name;
    
    FILE* log_save;
    log_save = fopen("monitoring_log.log", "a");

    date_save = date();
    user = users();
    now_att = att();
    
    if (event->name[0] == '.') {
        return;
    }
    if (event->name[0] == '4' && event->name[1] == '9' && event->name[2] == '1' && event->name[3] == '3') {
        return;
    }
    if (event->name[12] == '~') {
        return;
    }
    if (event->name[0] == 'r' && event->name[1] == 'e' && event->name[2] == 's' && event->name[3] == 'o' && event->name[4] == 'l' && event->name[5] == 'v' && event->name[6] == '.') {
        return;
    }
    for(int i=0; i<=sizeof(name);i++) {
        if (event->name[i] == '~'){
            return;
        }
    }

    if (event->mask & IN_ATTRIB) {
        fprintf(log_save, "%s %s - %s %s [ATTRIB] : File attribute is changed\n", date_save, user, now_att, name);
    }
    if (event->mask & IN_CREATE) {
        fprintf(log_save, "%s %s - %s %s [CREATE] : File attribute is changed\n", date_save, user, now_att, name);
    }
    if (event->mask & IN_DELETE) {
        fprintf(log_save, "%s %s - %s %s [DELETE] : File attribute is changed\n", date_save, user, now_att, name);
    }
    if (event->mask & IN_CLOSE_WRITE) {
        fprintf(log_save, "%s %s - %s %s [CLOSE_WRITE] : File is changed\n", date_save, user, now_att, name);
    }
    
    fclose(log_save);
}

void file_save() {
    char string[] = "tail /etc/rsyslog.conf";
    system(string);
}
 
int main(int argc, char *argv[])
{
    int ret;
    int fd;
    int wd;
    int ex;
    char buf[4096] __attribute__ ((aligned(__alignof__(struct inotify_event))));
    char *ptr;
    ssize_t size;
    const struct inotify_event *event;
 
    fd = inotify_init();
    
    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }
 
    wd = inotify_add_watch(fd, rsyslog_path, IN_ATTRIB | IN_DELETE | IN_CLOSE_WRITE);
    wd = inotify_add_watch(fd, log_path, IN_CREATE | IN_DELETE | IN_ATTRIB);
    
    if (wd < 0) {
        fprintf(stderr, "Failed to add watch [%s] [%s]", rsyslog_path, strerror(errno));
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }
	
    FILE * log_save;
    log_save = fopen("monitoring_log.log", "a");
    
    fprintf(log_save, "# field\n");
    fprintf(log_save, "#     <Date>      <name> - <authority> <name> [<event>] : <etc>\n");
 
    while (1) {
        size = read(fd, buf, sizeof(buf));
        if (size == -1 && errno != EAGAIN) {
            perror("read");
            fprintf(stderr, "read : %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
 
        if (size <= 0)
            break;
 
        for (ptr = buf; ptr < buf + size; ptr += sizeof(struct inotify_event) + event->len) {
            event = (struct inotify_event *)ptr;
            if (event->mask == IN_CREATE | IN_DELETE | IN_ATTRIB | IN_CLOSE_WRITE ) {
		__handle_inotify_event(event);		
            }
        }
    }
 
    ret = inotify_rm_watch(fd, wd);
    if (ret < 0) {
        fprintf(stderr, "Failed to rm watch [fd : %d] [wd : %d] [%s]", fd, wd, strerror(errno));
        perror("inotify_rm_watch");
        exit(EXIT_FAILURE);
    }
 
    return 0;
}
