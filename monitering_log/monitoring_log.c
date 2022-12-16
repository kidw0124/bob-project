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

void rsys() {
    char code_save[BUFSIZ] = { 0,};
    FILE * rsys_code;
    
    if ((rsys_code=popen("cd /etc && cat rsyslog.conf", "r")) == NULL) {
        printf("error");// error processing and return
    }
    
    fgets(code_save, BUFSIZ, rsys_code);
    
    
}


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
    char bfr3[BUFSIZ] = {0,};
    char bfr4[BUFSIZ] = {0,};
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
        
        if ((fp=popen("history 5 | grep rsyslog.conf", "r")) != NULL) {
            fgets(bfr3, BUFSIZ, fp);
        }
        
        if (bfr3 != "history 5 | grep rsyslog.conf") {
            char user[10][1000] = {0,};
            static char other_user[1000];
            
            fp=popen("users", "r");
            fgets(bfr4, BUFSIZ, fp) != NULL;
            
            for (int i=0;i<10;i++) {
                 for (int j=0;j<1000;j++) {
                      if (bfr4[j+blank] != ' ') {
                          user[i][j] = bfr4[j+blank];
                      }
                      else if (bfr4[j] == ' '){
                          blank = j + 1;
                          break;
                      }
                 }
            }
            
            for (int i=0;i<10;i++) {
                if (user[i] != now_user) {
                    for (int j=0;j<1000;j++) {
                        other_user[j] = user[i][j];
                    }
                    return other_user;
                }
            }
        }
        else {
            return now_user;
        }
    }
}

static void __handle_inotify_event(const struct inotify_event* event)
{
    char *date_save;
    char *user;
    const char *name = event->name;
    
    FILE* log_save;
    
    if (event->name[0] == '.') {
        return;
    }
    if (event->name[12] == '~') {
        return;
    }
    if (event->name[0] == '4' && event->name[1] == '9' && event->name[2] == '1' && event->name[3] == '3') {
        return;
    }
    for(int i=0; i<sizeof(name);i++) {
        if (event->name[i] == '~'){
            return;
        }
    }
    if (event->mask & IN_ATTRIB) {
	if (name == "rsyslog.conf~" | name == "4913") {
            return;    
        }
    	log_save = fopen("log_save.txt", "a");
        date_save = date();
        user = users();
        
        fprintf(log_save, "%s %s %s [ATTRIB] : File attribute is changed\n", date_save, user, name);
        printf("%s %s %s : [ATTRIB] File attribute is changed\n", date_save, user, name);
        fclose(log_save);
    }
    if (event->mask & IN_CREATE) {
	if (name == "rsyslog.conf~" | name == "4913") {
            return;    
    	}
    	log_save = fopen("log_save.txt", "a");
        date_save = date();
        user = users();
        fprintf(log_save, "%s %s %s [CREATE] : File attribute is changed\n", date_save, user, name);
        printf("%s %s %s [CREATE] : File is created\n", date_save, user, name);
        fclose(log_save);
    }
    if (event->mask & IN_DELETE) {
	if (name == "rsyslog.conf~" | name == "4913") {
            return;    
    	}
    	log_save = fopen("log_save.txt", "a");
        date_save = date();
        user = users();
        fprintf(log_save, "%s %s %s [DELETE] : File attribute is changed\n", date_save, user, name);
	printf("%s %s %s [DELETE] : File is deleted\n", date_save, user, name);
	fclose(log_save);	
    }
    if (event->mask & IN_CLOSE_WRITE) {
    	log_save = fopen("log_save.txt", "a");
        date_save = date();
        user = users();
        fprintf(log_save, "%s %s %s [CLOSE_WRITE] : File is changed\n", date_save, user, name);
        printf("%s %s %s [CLOSE_WRITE] : File is changed\n", date_save, user, name);
        fclose(log_save);
    }
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
    wd = inotify_add_watch(fd, log_path, IN_CREATE | IN_DELETE | IN_CLOSE_WRITE | IN_ATTRIB);
    
    if (wd < 0) {
        fprintf(stderr, "Failed to add watch [%s] [%s]", rsyslog_path, strerror(errno));
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }
    
    char code_save[BUFSIZ] = { 0,};
    char change_code[BUFSIZ] = { 0,};
    FILE * rsys_code;
    FILE * change_rsys;
    
    if ((rsys_code=popen("cd /etc && cat rsyslog.conf", "r")) == NULL) {
        printf("error");// error processing and return
    }
    
    fgets(code_save, BUFSIZ, rsys_code);
 
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
