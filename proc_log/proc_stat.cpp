#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <pwd.h>
#include <sys/wait.h>

int Isdigit(char *str)
{
        int i;
        for (i=0; i<strlen(str); i++)
        {
                if (isdigit(str[i])==0)
                        return 0;
        }
        return 1;
}

int getcmdlined(char *file, char *buf)
{
    FILE *srcFp;
    int i;
    srcFp = fopen(file, "r");

    memset(buf, 0, sizeof(buf));
    if(srcFp!=NULL)
    {   
        fgets(buf, 256, srcFp);
    }
    else
    {
        fgets(buf, 256, NULL);
    }
    fclose(srcFp);
}

void Eliminate(char *str, char ch)
{
    unsigned len = strlen(str) + 1;
    for (; *str != '\0'; str++,len--)
    {
        if (*str == ch)
        {
            strcpy(str, str + 1);
            str--;
        }
    }
}

int main(void)
{
        DIR *dir_info;
        struct dirent *dir_entry;
        char proc_file[40];
        char proccm_file[100];
        char cmdlined[256];
        char stimed[100];
        char buff[1048576];
        FILE *fp, *fp2, *fp3;
        int len;
        char p_id[256];
        char p_pid[256];
        char named[256];
        char name[256];
        struct stat lstat;
        char buffer[256];
        char tic[256];
        char i[20];
        char j[20];
        char k[20];
        char p[20];
        char q[20];
        char w[20];
        char strc[256];
        char strtext[256];
        FILE *pp, *pn;
        char buffe[2048576];
        int statuss;
        

        system("echo 0 > /sys/kernel/debug/tracing/tracing_on");
        system("echo 0 > /sys/kernel/debug/tracing/events/enable");
        system("echo  __startup_secondary_64  > /sys/kernel/debug/tracing/set_ftrace_filter");
        system("echo function > /sys/kernel/debug/tracing/current_tracer");
        system("echo 1 > /sys/kernel/debug/tracing/events/sched/sched_switch/enable");
        system("echo 1 > /sys/kernel/debug/tracing/events/sched/sched_wakeup/enable");
        system("echo 1 > /sys/kernel/debug/tracing/events/sched/sched_process_fork/enable");
        system("echo 1 > /sys/kernel/debug/tracing/events/sched/sched_process_exit/enable");
        system("echo 1 > /sys/kernel/debug/tracing/events/signal/enable");
        system("echo SyS_exit_group do_exit > /sys/kernel/debug/tracing/set_ftrace_filter");
        system("echo copy_process* >> /sys/kernel/debug/tracing/set_ftrace_filter");
        system("echo 1 > /sys/kernel/debug/tracing/options/func_stack_trace");
        system("echo 1 > /sys/kernel/debug/tracing/options/sym-offset");
        system("echo 1 > /sys/kernel/debug/tracing/tracing_on");

        dir_info = opendir("/proc");

        if (dir_info != NULL)
        {
            if((pp = fopen("/tmp/content01.txt", "r")) == NULL)
            {
                pn = fopen("/tmp/content01.txt", "w");
                
                while(dir_entry = readdir(dir_info))
                {
                        if (Isdigit(dir_entry->d_name))
                        {
                                system("netstat -ntlpu | sed -e '1,2d' -e 's/:::/0 /g' -e 's/[:]/ /g' -e 's/[\\/]/ /g'| grep LISTEN | awk '{ print $1,$5,$9,$10 }' > /tmp/portedd.txt");
                                sprintf(proc_file, "/proc/%s/stat", dir_entry->d_name);
                                sprintf(proccm_file, "/proc/%s/cmdline", dir_entry->d_name);

                                system("ps -eo pid,lstart | sed 1d > /tmp/start_time.txt");
                                getcmdlined(proccm_file, cmdlined);
                                struct passwd *upasswd;
                                stat(proc_file,&lstat);
                                upasswd = getpwuid(lstat.st_uid);
                                strcpy(name, upasswd->pw_name);

                                if ((fp3 = fopen("/tmp/start_time.txt", "r")) == NULL)
                                {
                                    printf("파일를 열 수 없습니다.\n");
                                    return 0;
                                }

                                while(!feof(fp3))
                                {
                                    fscanf(fp3, "%s %s %s %s %s %s", i, j, k, p, q, w);

                                    if(strncmp(i, dir_entry->d_name, sizeof(i))==0)
                                    {
                                        fprintf(pn,"------------------------------\n");
                                        fprintf(pn,"%s %s %s %s %s \n", j, k, p, q, w);
                                    }
                                }
                                fclose(fp3);
                                memset(i,'x',sizeof(i));
                                memset(j,'x',sizeof(j));
                                memset(k,'x',sizeof(k));
                                memset(p,'x',sizeof(p));

                                fp = fopen(proc_file, "r");
                                while(fgets(buff, 1048576, fp))
                                {
                                        sscanf(buff, "%s %s %*s %s", p_id, named, p_pid);
                                        Eliminate(named,'(');
                                        Eliminate(named,')');
                                        fprintf(pn, "%s %s %s %s %s\n", p_id, p_pid, name, named, cmdlined);

                                }
                                fclose(fp);

                                if ((fp2 = fopen("/tmp/portedd.txt", "r")) == NULL)
                                {
                                    printf("파일를 열 수 없습니다.\n");
                                    return 0;
                                }

                                while(!feof(fp2))
                                {
                                    fscanf(fp2, "%s %s %s %s", i, j, k, p);
                                    if(strcmp(k, dir_entry->d_name)==0)
                                    {
                                        fprintf(pn, "%s %s\n", i, j);
                                    }
                                }
                                fclose(fp2);
                        }
                }
                closedir(dir_info);
                fclose(pn);
            }
            else if((pp = fopen("/tmp/content01.txt", "r")) != NULL)
            {
                pn = fopen("/tmp/content01.txt", "a");
                
                while(dir_entry = readdir(dir_info))
                {
                    if (Isdigit(dir_entry->d_name))
                    {
                        system("netstat -ntlpu | sed -e '1,2d' -e 's/:::/0 /g' -e 's/[:]/ /g' -e 's/[\\/]/ /g'| grep LISTEN | awk '{ print $1,$5,$9,$10 }' > /tmp/portedd.txt");
                        sprintf(proc_file, "/proc/%s/stat", dir_entry->d_name);
                        sprintf(proccm_file, "/proc/%s/cmdline", dir_entry->d_name);

                        system("ps -eo pid,lstart | sed 1d > /tmp/start_time.txt");
                        getcmdlined(proccm_file, cmdlined);
                        struct passwd *upasswd;
                        stat(proc_file,&lstat);
                        upasswd = getpwuid(lstat.st_uid);
                        strcpy(name, upasswd->pw_name);

                        fp = fopen(proc_file, "r");
                        if((fgets(buff, 1048576, fp) != NULL))
                        {
                            sscanf(buff, "%s %s %*s %s", p_id, named, p_pid);
                            Eliminate(named,'(');
                            Eliminate(named,')');
                        }
                        fclose(fp);

                        memset(i,'x',sizeof(i));
                        memset(j,'x',sizeof(j));
                        memset(k,'x',sizeof(k));
                        memset(p,'x',sizeof(p));

                        while(1)
                        {
                            char buffer5[1048567];
                            char c_name[100];
                            char c_pd[50];
                            char *c1 = fgets(buffer5, 1048567, pp);
                            if( c1 == NULL ) break;
                            sscanf(buffer5, "%s %*s %*s %s", c_pd, c_name);
                            if(strcmp(c_pd, p_id)==0)
                            {
                                if(strcmp(c_name, named)==0)
                                {
                                    if ((fp3 = fopen("/tmp/start_time.txt", "r")) == NULL)
                                    {
                                        printf("파일를 열 수 없습니다.\n");
                                        return 0;
                                    }
                                    while(!feof(fp3))
                                    {
                                        fscanf(fp3, "%s %s %s %s %s %s", i, j, k, p, q, w);

                                        if(strncmp(i, dir_entry->d_name, sizeof(i))==0)
                                        {
                                            fprintf(pn,"------------------------------\n");
                                            fprintf(pn ,"%s %s %s %s %s\n", j, k, p, q, w);
                                        }
                                    }
                                    fclose(fp3);
                                    memset(i,'x',sizeof(i));
                                    memset(j,'x',sizeof(j));
                                    memset(k,'x',sizeof(k));
                                    memset(p,'x',sizeof(p));
                                    fprintf(pn, "%s %s %s %s %s\n", p_id, p_pid, name, named, cmdlined);

                                    if ((fp2 = fopen("/tmp/portedd.txt", "r")) == NULL)
                                    {
                                        printf("파일를 열 수 없습니다.\n");
                                        return 0;
                                    }
                                    while(!feof(fp2))
                                    {
                                        fscanf(fp2, "%s %s %s %s", i, j, k, p);
                                        if(strcmp(k, dir_entry->d_name)==0)
                                        {
                                            fprintf(pn, "%s %s\n", i, j);                                    
                                        }
                                        
                                    }
                                    fclose(fp2);

                                }
                            }
                        }
                    }
                }   
                closedir(dir_info);
            }
        }
    sleep(30);
    system("echo 0 > /sys/kernel/debug/tracing/tracing_on");
    system("cp /sys/kernel/debug/tracing/trace .");
    system("mv trace ftrace_log.c");
    system("cat ftrace_log.c | grep -e do_exit | sed -e 's/-/ /g' -e 's/+/ /g' -e 's/://g' | awk '{print $1,$2,$5,$6}' >>  /tmp/ftrace01.txt");
    sleep(30);
    FILE *fp_1, *fp_2, *fp_3;
    
    char buffer1[1000];
    char buffer2[1000];
    char third[100];
    char fine[100];
    char fifth[100];
    char six[100];
    char e_ti[2000];
    int d;
    
    if( (fp_1 = fopen("/tmp/content01.txt", "r")) == NULL ) exit(1);
    if( (fp_2 = fopen("/tmp/ftrace01.txt", "r")) == NULL ) exit(1);
    if( (fp_3 = fopen("/tmp/fiene.txt", "w")) == NULL ) exit(1);
    while(1)
    {
        d = 0;
        char buffer1[1000];
        char *p1 = fgets(buffer1, 1000, fp_1);
        if( p1 == NULL ) break;
        sscanf(buffer1, "%s %*s %*s %s", third, six);
        while(1)
        {
            char buffer2[1000];
            char *p2 = fgets(buffer2, 1000, fp_2);
            sscanf(buffer2, "%s %s %s", fifth, fine, e_ti);
            if( p2 == NULL )
            {
                fseek(fp_2, 0, SEEK_SET);
                break;
            }
            if( strcmp(third, fine)==0 )
            {
                if(strcmp(six, fifth)==0)
                {
                    fprintf(fp_3, "%s", p1);
                    fprintf(fp_3, "%s\n", e_ti);
                    d++;
                }
            }
            if(d == 0) fprintf(fp_3, "%s", p1);
        }

    }

    fclose(fp_1);
    fclose(fp_2);
    fclose(fp_3);

}
